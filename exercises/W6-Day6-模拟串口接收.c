/* ============================================================
 *  W6-Day6 环形缓冲区实战：模拟串口接收 ★
 *
 *  ┌────────────────────────────────────────────────────────────┐
 *  │  串口字节来得又快又不可预测，而主循环处理可能很慢。             │
 *  │  若在中断里直接处理 → 中断耗时过长 → 丢字节、系统卡死。         │
 *  │  解法：中断只做"收下 + 塞进缓冲"（几微秒），                   │
 *  │        真正的解析交给主循环（可以慢慢做）。                    │
 *  └────────────────────────────────────────────────────────────┘
 *
 *
 *  用【虚拟时间 virt_us】代替真实时间，单位微秒：
 *
 *    - 发送端每 BYTE_INTERVAL_US 微秒产生一个字节 → 调用 isr_rx_byte()
 *    - 主循环每 POLL_PERIOD_US 微秒调用一次 main_loop_poll()
 *    - main_loop_poll() 里【每处理一个字节】消耗 g_consumer_us_per_byte 微秒
 *      （模拟"解析、计算、打印"要花的时间）
 * ============================================================ */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ============================================================
 *  一、环形缓冲
 * ============================================================ */
#define RB_CAP 64               /* 缓冲容量（字节） */

typedef struct
{
    uint8_t  buf[RB_CAP];
    uint32_t read;
    uint32_t write;
    uint32_t count;
    uint32_t overflow_drop;     /* 满了丢掉的字节数（真实固件里的溢出自检计数） */
} RingBuf;

enum ParseState {
    WAIT_HASH,
    IN_PAYLOAD,
    IN_CHECKSUM,
    WAIT_LF
};

static void     rb_init(RingBuf *rb);
static int      rb_is_empty(const RingBuf *rb);
static int      rb_put(RingBuf *rb, uint8_t data);
static int      rb_get(RingBuf *rb, uint8_t *data);

/* ============================================================
 *  二、虚拟时间与"硬件参数"
 * ============================================================ */

/* 字节到达间隔（微秒）。
 * ★ 计算：1 字节 = 起始位 + 8 数据位 + 停止位 = 10 位
 *         T = 10 / 波特率 秒 = 10 / 波特率 × 1e6 微秒
 *   9600 bps   → 10/9600 s = 1.0417 ms = 1042 us
 *   115200 bps → 10/115200 s = 86.8 us
 */
static long g_byte_interval_us = 1042;   /* 9600 波特率 */

/* 主循环被调用的周期（微秒）—— 相当于 while(1) 里轮询的间隔 */
static long g_poll_period_us   = 1000;   /* 1 ms */

/* ★ 消费者每处理一个字节花的微秒数（模拟解析/计算/打印的耗时） */
static long g_consumer_us_per_byte = 5;

/* ★ 每次主循环轮询最多处理几个字节（0 = 不限制）
*/
static long g_max_bytes_per_poll = 0;

/* 统计：主循环被调用了几次 */
static uint32_t stat_poll_calls = 0;

/* 是否打印每一帧（场景 1 打开看细节，场景 2/3 关掉避免刷屏） */
static int g_print_frames = 1;

static uint64_t virt_us = 0;

/* 统计 */
static uint32_t stat_bytes_sent     = 0;  /* 发送端产生多少字节 */
static uint32_t stat_bytes_lost     = 0;  /* 生产侧因缓冲满丢弃多少 */
static uint32_t stat_bytes_consumed = 0;  /* 主循环取走多少字节 */
static uint32_t stat_frames_ok      = 0;  /* 成功解析帧数 */
static uint32_t stat_frames_bad     = 0;  /* 校验/格式错误帧数 */

/* 假串口字节流 */
static const char *g_stream = NULL;
static size_t      g_stream_len = 0;
static size_t      g_stream_pos = 0;

/* ============================================================
 *  三、帧格式（主循环要解析的协议）
 *
 *  帧： '#' + payload + '*' + 校验(2 位十六进制) + '\n'
 *  例： #LED=ON*7A\n          payload = "LED=ON"
 *  校验：payload 各字节求和取低 8 位
 * ============================================================ */
static uint8_t calc_checksum(const char *payload, size_t len)
{
    uint8_t sum = 0;
    for (size_t i = 0; i < len; i++) sum = (uint8_t)(sum + (uint8_t)payload[i]);
    return sum;
}

/* 造一帧，返回帧长度*/
static size_t make_frame(char *out, const char *payload)
{
    size_t n = strlen(payload);
    out[0] = '#';
    memcpy(out + 1, payload, n);
    out[1 + n] = '*';
    sprintf(out + 2 + n, "%02X", calc_checksum(payload, n));
    out[4 + n] = '\n';
    out[5 + n] = '\0';
    return 5 + n;
}

/* ============================================================
 *  四、★ 两个主体函数
 * ============================================================ */

/* ------------------------------------------------------------
 *  4.1 isr_rx_byte —— 串口接收中断服务函数（ISR）
 * ------------------------------------------------------------ */
static void isr_rx_byte(RingBuf *rb, uint8_t b)
{
    if(rb_put(rb,b)==0)
    {
        rb->overflow_drop++;
        stat_bytes_lost++;
    }
}

static uint8_t hex2val(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';
    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0;
}

/* ------------------------------------------------------------
 *  4.2 main_loop_poll —— 主循环周期调用的处理函数
 *
 *  每次被调用时，从环形缓冲【尽量多地】取字节，用【状态机】解析帧。
 * ------------------------------------------------------------ */
static void main_loop_poll(RingBuf *rb)
{
    static enum ParseState s_state = WAIT_HASH;
    static uint8_t s_payload_buf[64];
    static int s_payload_len = 0;
    static uint8_t s_cs_buf[2];
    static int s_cs_idx = 0;

    uint8_t b;
    long    processed_this_poll = 0;

    while(rb_get(rb, &b) == 1)
    {
        /* ★ 每处理一个字节消耗虚拟时间（模拟解析/计算/打印的耗时） */
        virt_us += (uint64_t)g_consumer_us_per_byte;
        stat_bytes_consumed++;
        processed_this_poll++;

        switch(s_state)
        {
            case WAIT_HASH:
                if(b == '#')
                {
                    s_payload_len = 0;
                    s_cs_idx = 0;
                    s_state = IN_PAYLOAD;
                }
                break;
            case IN_PAYLOAD:
                if(b == '*')
                {
                    s_state = IN_CHECKSUM;
                }
                else
                {
                    if(s_payload_len < 63)
                    {
                        s_payload_buf[s_payload_len++] = b;
                    }
                }
                break;
            case IN_CHECKSUM:
                s_cs_buf[s_cs_idx++] = b;
                if(s_cs_idx >= 2)
                {
                    uint8_t high = hex2val(s_cs_buf[0]);
                    uint8_t low  = hex2val(s_cs_buf[1]);
                    uint8_t recv_cs = (high <<4) | low;
                    uint8_t calc_sum = calc_checksum((const char *)s_payload_buf, s_payload_len);
                    if(calc_sum == recv_cs)
                    {
                        stat_frames_ok++;
                        s_payload_buf[s_payload_len] = '\0';
                        if (g_print_frames)
                        {
                            printf("    [帧] payload=%s\n", s_payload_buf);
                        }
                    }
                    else
                    {
                        stat_frames_bad++;
                    }
                    s_state = WAIT_LF;
                }
                break;
            case WAIT_LF:
                if(b == '\n')
                {
                    //收到换行，一帧彻底结束
                }
                s_state = WAIT_HASH;
                break;
            default:
                s_state = WAIT_HASH;
                break;
        }

        /* ★ 处理能力上限：本次轮询处理够多就停下，把控制权还给仿真引擎，
         *   让生产者的"字节到达"事件有机会插入（否则永远不会溢出） */
        if (g_max_bytes_per_poll > 0 && processed_this_poll >= g_max_bytes_per_poll)
        {
            break;
        }
    }
}

/* ============================================================
 *  五、仿真引擎
 * ============================================================ */

static void sender_produce_one_byte(RingBuf *rb)
{
    if (g_stream_pos >= g_stream_len) return;
    uint8_t b = (uint8_t)g_stream[g_stream_pos++];
    stat_bytes_sent++;
    isr_rx_byte(rb, b);
}

/* 按虚拟时间推进，模拟 duration_us 微秒 */
static void run_sim(RingBuf *rb, uint64_t duration_us)
{
    uint64_t end = virt_us + duration_us;

    /* ★ 时间驱动，统一基准：所有时刻都从 virt_us 派生。*/
    uint64_t next_rx   = virt_us + (uint64_t)g_byte_interval_us;
    uint64_t next_poll = virt_us + (uint64_t)g_poll_period_us;

    for (;;)
    {
        int can_rx   = (g_stream_pos < g_stream_len) && (next_rx   <= end);
        int can_poll = (next_poll <= end);

        if (!can_rx && !can_poll) break;

        if (can_rx && (!can_poll || next_rx <= next_poll))
        {
            /* 字节到达：中断把它塞进缓冲 */
            virt_us = next_rx;
            sender_produce_one_byte(rb);
            next_rx = virt_us + (uint64_t)g_byte_interval_us;
        }
        else
        {
            /* 轮询：推进到轮询时刻，主循环处理（处理耗时记入 virt_us） */
            virt_us = next_poll;
            main_loop_poll(rb);
            stat_poll_calls++;
            next_poll = virt_us + (uint64_t)g_poll_period_us;
        }
    }
    virt_us = end;
}

/* 把整个字节流按虚拟时间灌完，再清空残留 */
static void run_until_done(RingBuf *rb)
{
    while (g_stream_pos < g_stream_len)
    {
        /* ★ 每次推进 64 个字节的时间，让"到达 / 轮询 / 处理"充分交错。
         *   如果只推进 1 个字节的时间，轮询周期比它大就不会触发轮询*/
        run_sim(rb, (uint64_t)g_byte_interval_us * 64);
    }

    /* 收完后解除处理能力上限，让主循环把缓冲里剩的处理完*/
    long saved_cap = g_max_bytes_per_poll;
    g_max_bytes_per_poll = 0;

    for (int i = 0; i < 500; i++)
    {
        if (rb_is_empty(rb)) break;
        virt_us += (uint64_t)g_poll_period_us;
        main_loop_poll(rb);
        stat_poll_calls++;
    }

    g_max_bytes_per_poll = saved_cap;
}

static void reset_all(RingBuf *rb, const char *stream)
{
    rb_init(rb);
    virt_us = 0;
    stat_bytes_sent = stat_bytes_lost = 0;
    stat_bytes_consumed = stat_frames_ok = stat_frames_bad = 0;
    stat_poll_calls = 0;
    g_stream = stream;
    g_stream_len = strlen(stream);
    g_stream_pos = 0;
}

static void print_stats(const char *title)
{
    printf("  ---- %s ----\n", title);
    printf("    发送字节数          : %u\n", (unsigned)stat_bytes_sent);
    printf("    生产侧丢弃(缓冲满)  : %u\n", (unsigned)stat_bytes_lost);
    printf("    主循环消费字节数    : %u\n", (unsigned)stat_bytes_consumed);
    printf("    主循环被调用次数    : %u\n", (unsigned)stat_poll_calls);
    printf("    成功解析帧数        : %u\n", (unsigned)stat_frames_ok);
    printf("    解析失败帧数        : %u\n", (unsigned)stat_frames_bad);
    printf("    虚拟耗时            : %lu us (%.1f ms)\n",
       (unsigned long)virt_us, (double)virt_us / 1000.0);
    printf("\n");
}

/* ============================================================
 *  六、main（测试场景）
 * ============================================================ */
int main(void)
{
    RingBuf rb;
    static char stream[32768];

    printf("========== 场景 1：9600 波特率，主循环很轻（理想）==========\n");
    printf("  字节间隔 1042us，消费者每字节只花 5us → 完全跟得上\n\n");
    {
        size_t off = 0;
        for (int i = 0; i < 10; i++)
        {
            char payload[32];
            sprintf(payload, "CMD%02d=OK", i);
            off += make_frame(stream + off, payload);
        }

        g_byte_interval_us = 1042;
        g_poll_period_us   = 1000;
        g_consumer_us_per_byte = 5;
        g_max_bytes_per_poll   = 0;   /* 不限制 */
        g_print_frames = 1;

        reset_all(&rb, stream);
        run_until_done(&rb);
        print_stats("9600 + 轻量主循环");

        printf("  期望：丢弃=0，消费=%u，成功帧=10，失败=0\n\n",
               (unsigned)strlen(stream));
    }

    printf("  按回车继续...");
    (void)getchar();

    printf("\n========== 场景 2：115200 波特率，主循环很重 ★持续溢出 ==========\n");
    printf("  字节间隔 87us（生产者 11494 字节/秒）\n");
    printf("  消费者：每字节 150us，且每 300us 才轮询一次、每次最多处理 2 个\n");
    printf("  → 消费者上限 = 2/300us = 6667 字节/秒，【明显低于】生产者\n");
    printf("  → 缓冲必然填满并【持续】丢数据\n\n");
    {
        size_t off = 0;
        for (int i = 0; i < 1000; i++)
        {
            char payload[32];
            sprintf(payload, "DATA%03d", i);
            off += make_frame(stream + off, payload);
        }

        g_byte_interval_us = 87;
        g_poll_period_us   = 300;
        g_consumer_us_per_byte = 150;
        g_max_bytes_per_poll   = 2;
        g_print_frames = 0;          /* 帧太多，关掉打印 */

        reset_all(&rb, stream);
        run_until_done(&rb);
        print_stats("115200 + 重量主循环");

        printf("  ★ 关键观察：\n");
        printf("    - 生产侧丢弃 > 0  → 缓冲满了，中断里丢字节\n");
        printf("    - 成功帧数远小于 1000 → 大量帧被破坏（丢字节导致不完整）\n");
        printf("    - 根因：主循环【平均】处理速度 < 字节到达速度，\n");
        printf("            缓冲只能吸收突发，不能弥补平均速度不足\n\n");
    }

    printf("  按回车继续...");
    (void)getchar();

    printf("\n========== 场景 3：115200 但主循环优化了 ★对比场景 2 ==========\n");
    printf("  同样的字节流、同样的波特率，只把主循环优化：\n");
    printf("    每字节耗时 150us → 30us（例如去掉循环里的 printf / 减少浮点运算）\n");
    printf("    轮询周期 300us → 1000us，每次最多处理 64 个\n");
    printf("  → 消费者上限 = 64/1000us = 64000 字节/秒，【远高于】生产者\n");
    printf("  → 丢弃降到 0\n\n");
    {
        size_t off = 0;
        for (int i = 0; i < 1000; i++)
        {
            char payload[32];
            sprintf(payload, "FAST%03d", i);
            off += make_frame(stream + off, payload);
        }

        g_byte_interval_us = 87;
        g_poll_period_us   = 1000;
        g_consumer_us_per_byte = 30;
        g_max_bytes_per_poll   = 64;
        g_print_frames = 0;

        reset_all(&rb, stream);
        run_until_done(&rb);
        print_stats("115200 + 优化后主循环");

        printf("  ★ 对比场景 2：同样的波特率，只优化主循环的处理耗时，\n");
        printf("     丢包就从大量变成 0 —— 这就是\"中断只做最少的事\"的价值\n\n");
    }

    printf("  按回车继续...");
    (void)getchar();

    printf("\n========== 场景 4：理论时间预算 ★（面试会问）==========\n\n");
    printf("  模型：\n");
    printf("    1 字节 = 10 位（起始 1 + 数据 8 + 停止 1）\n");
    printf("    字节到达间隔    T = 10 / 波特率 × 1e6  (微秒)\n");
    printf("    主循环每字节耗时 P\n");
    printf("\n");
    printf("  【不丢数据的条件】：P <= T\n");
    printf("    即：主循环处理一个字节的平均耗时，必须小于字节到达间隔\n\n");

    printf("  %-12s %-16s %-24s\n", "波特率", "字节间隔 T", "主循环时间预算");
    printf("  %-12s %-16s %-24s\n", "----------", "--------------", "------------------------");
    long bauds[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800 };
    for (int i = 0; i < 7; i++)
    {
        double T = 10.0 / (double)bauds[i] * 1e6;
        printf("  %-12ld %-16.1f P <= %.1f us\n", bauds[i], T, T);
    }
    printf("\n");

    printf("  【缓冲容量的作用】：吸收突发\n");
    printf("    若主循环偶尔卡顿 t 微秒，积压字节数 ≈ t / T，\n");
    printf("    只要 C > t/T，短时卡顿就不丢数据。\n\n");

    printf("  %-12s %-14s %-16s %-16s\n", "波特率", "卡顿t(ms)", "积压字节数", "需最小缓冲C");
    printf("  %-12s %-14s %-16s %-16s\n", "----------", "----------", "--------------", "--------------");
    struct { long baud; long stall_ms; } case2[] = {
        { 9600,   5 }, { 9600,   50 },
        { 115200, 1 }, { 115200, 5 }, { 115200, 50 },
    };
    for (int i = 0; i < 5; i++)
    {
        double T = 10.0 / (double)case2[i].baud * 1e6;      /* us */
        double stall = (double)case2[i].stall_ms * 1000.0;  /* us */
        double backlog = stall / T;
        printf("  %-12ld %-14ld %-16.0f %-16.0f\n",
               case2[i].baud, case2[i].stall_ms, backlog, backlog);
    }
    printf("\n");

    printf("  ★ 两条结论（记住这两句就够）：\n");
    printf("    1. 缓冲只能【吸收突发】，不能【弥补平均速度不足】。\n");
    printf("       平均处理不过来 → 再大的缓冲也迟早填满并持续丢数据。\n");
    printf("    2. 所以中断里只做最少的事（塞进缓冲），\n");
    printf("       把解析/计算/打印这些慢活全部挪到主循环 —— \n");
    printf("       而主循环本身也要保证平均每字节耗时 < 字节到达间隔。\n");

    printf("\n========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  环形缓冲实现
 * ============================================================ */

static void rb_init(RingBuf *rb)
{
    rb->count=0;
    rb->read=0;
    rb->write=0;
}

static int rb_is_empty(const RingBuf *rb)
{
    return (rb->count==0);
}

static int rb_put(RingBuf *rb, uint8_t data)
{
    if(rb->count!=RB_CAP)
    {
        rb->buf[rb->write]=data;
        rb->write=(rb->write+1)%RB_CAP;
        rb->count++;
        return 1;
    }
    return 0;
}

static int rb_get(RingBuf *rb, uint8_t *data)
{
    if(rb->count!=0)
    {
        *data=rb->buf[rb->read];
        rb->read=(rb->read+1)%RB_CAP;
        rb->count--;
        return 1;
    }
    return 0;
}


/*1. 为什么中断里只做最少的事？如果中断里直接解析会怎样？
    答：中断的作用是快速响应硬件信号。中断服务函数执行时间越短越好。如果在中断内直接解析串口数据，中断持续时间变长，会阻塞其他中断，造成丢失数据、系统实时性下降，甚至程序崩溃；所以中断仅完成数据缓存，解析工作放到主循环执行。
  2. "生产快于消费"时会发生什么？缓冲满了以后，两种满策略（丢新 / 覆盖最旧）分别造成什么后果？
    答：当生产速度大于消费速度，有限缓冲区会被填满。
    1）丢新（丢弃新来数据）：缓冲区满时，新数据丢弃，已有旧数据保留。会丢失后续新报文，但缓存内已有数据完好，适合串口通信。
    2）覆盖最旧：缓冲区满时，新数据直接覆盖最早存入的数据。缓存永远保留最新数据；但未消费的旧帧容易被破坏，出现帧错乱，不适合串口帧解析，适合只需要最新采样的场景。
  3. 缓冲容量能"弥补"主循环处理速度不够吗？为什么？
    答：缓冲区只能应对短时突发流量，不能解决长期主循环处理能力不足。
       原因：缓冲区容量是有限的。
        - 若只是短暂突发，缓冲区可以临时存放数据，平滑流量，等待主循环慢慢处理；
        - 如果平均生产速率持续大于消费速率，无论缓冲区多大，最终都会被填满，发生丢包 / 数据损坏。
        缓冲区不提升主循环平均处理速度，只是临时缓存。
*/