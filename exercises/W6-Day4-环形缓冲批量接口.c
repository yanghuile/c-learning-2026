/* ============================================================
 *  W6-Day4 环形缓冲区（三）：批量接口 + 边界测试
 *
 *  方案：计数器（count）—— 你昨天分析出的结论（256 字节缓冲选 B）
 *
 *  今天要做两件事：
 *    1.【默写】基础 5 个函数（rb_init / is_empty / is_full / rb_put / rb_get）
 *       —— 昨天在"两方案"文件里写过，这次不看参考，直接写
 *    2.【新学】批量接口 4 个函数：
 *         rb_write_buf  —— 一次写多个字节，返回【实际写入数】
 *         rb_read_buf   —— 一次读多个字节，返回【实际读出数】
 *         rb_count      —— 当前元素个数
 *         rb_clear      —— 清空
 *
 *  运行：b exercises/W6-Day4-环形缓冲批量接口.c
 *
 *  ┌──────────────────────────────────────────────────────────┐
 *  │  为什么批量接口重要？                                     │
 *  │                                                          │
 *  │   串口收发、DMA 传输、I2C 读传感器 —— 都是"一批数据"，     │
 *  │   不是一个字节。                                          │
 *  │                                                          │
 *  │  而批量接口的【关键】是：返回【实际处理了多少】。          │
 *  │   因为空间可能不够，只能处理一部分。                      │
 *  │   如果只返回"成功/失败"，调用方就不知道处理了几个。        │
 *  │                                                          │
 *  │  这是真实驱动的写法（W13 UART 不定长接收会直接用）。      │
 *  └──────────────────────────────────────────────────────────┘
 * ============================================================ */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RB_CAP 8            /* 缓冲容量（故意设小，方便测回绕） */

typedef struct
{
    uint8_t  buf[RB_CAP];
    uint32_t read;          /* 下一个要读的位置 */
    uint32_t write;         /* 下一个要写的位置 */
    uint32_t count;         /* 当前元素个数 */
} RingBuf;

/* ---------- 函数声明 ---------- */
void     rb_init(RingBuf *rb);
int      rb_is_empty(const RingBuf *rb);
int      rb_is_full(const RingBuf *rb);
int      rb_put(RingBuf *rb, uint8_t data);
int      rb_get(RingBuf *rb, uint8_t *data);

/* 今天新学的 4 个 */
uint32_t rb_write_buf(RingBuf *rb, const uint8_t *src, uint32_t len);
uint32_t rb_read_buf(RingBuf *rb, uint8_t *dst, uint32_t len);
uint32_t rb_count(const RingBuf *rb);
void     rb_clear(RingBuf *rb);

/* ============================================================
 *  打印辅助（已写好）
 * ============================================================ */
static void rb_dump(const RingBuf *rb)
{
    printf("     read=%u write=%u count=%u | ",
           (unsigned)rb->read, (unsigned)rb->write, (unsigned)rb->count);
    if (rb->count == 0) { printf("（空）\n"); return; }
    printf("数据: ");
    for (uint32_t i = 0; i < rb->count; i++)
        printf("%c", rb->buf[(rb->read + i) % RB_CAP]);
    printf("\n");
}

/* 打印一段字节（用于批量读出的结果） */
static void dump_bytes(const uint8_t *p, uint32_t n)
{
    if (n == 0) { printf("（无）"); return; }
    for (uint32_t i = 0; i < n; i++) printf("%c", p[i]);
}

/* ============================================================
 *  main：测试（已写好，不用改）
 * ============================================================ */
int main(void)
{
    uint32_t n;

    /* ==========================================================
     * 测试 1：基础函数（默写检查）
     * ========================================================== */
    printf("========== 测试 1：基础函数 ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        printf("  初始化: "); rb_dump(&rb);
        printf("  empty=%d（期望 1）  full=%d（期望 0）  count=%u（期望 0）\n",
               rb_is_empty(&rb), rb_is_full(&rb), (unsigned)rb_count(&rb));

        for (int i = 0; i < RB_CAP; i++) rb_put(&rb, (uint8_t)('A' + i));
        printf("  写满 %d 个: ", RB_CAP); rb_dump(&rb);
        printf("  empty=%d（期望 0）  full=%d（期望 1）  count=%u（期望 %d）\n",
               rb_is_empty(&rb), rb_is_full(&rb), (unsigned)rb_count(&rb), RB_CAP);

        n = rb_put(&rb, 'X');
        printf("  满了再 put → %u（期望 0）\n", (unsigned)n);
        printf("  （注意：单个 put 返回 int，这里赋给 uint32_t 也能看出 0/1）\n\n");
    }

    /* ==========================================================
     * 测试 2：批量写入（空间足够）
     * ========================================================== */
    printf("========== 测试 2：rb_write_buf（空间足够）==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        const uint8_t src[] = "ABC";

        n = rb_write_buf(&rb, src, 3);
        printf("  写入 \"ABC\"(3 字节) → 返回 %u（期望 3）\n", (unsigned)n);
        rb_dump(&rb);

        n = rb_write_buf(&rb, (const uint8_t *)"DEFG", 4);
        printf("  再写入 \"DEFG\"(4 字节) → 返回 %u（期望 4）\n", (unsigned)n);
        rb_dump(&rb);
        printf("  count=%u（期望 7）\n\n", (unsigned)rb_count(&rb));
    }

    /* ==========================================================
     * 测试 3：批量写入（空间不够，只能写一部分）★关键
     * ========================================================== */
    printf("========== 测试 3：rb_write_buf（空间不够）★ ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        const uint8_t src[] = "ABCDEFGHIJ";   /* 10 字节，但缓冲只有 8 格 */

        n = rb_write_buf(&rb, src, 10);
        printf("  缓冲 %d 格，写 10 字节 → 返回 %u（期望 %d）\n",
               RB_CAP, (unsigned)n, RB_CAP);
        rb_dump(&rb);
        printf("  ★ 关键：返回的是【实际写入数】，不是 len，也不是 0/1\n");
        printf("  → 调用方靠它知道「还有多少没写进去」，可以下次再写\n\n");
    }

    /* ==========================================================
     * 测试 4：批量读出 + 回绕（写入跨越末尾）★最难
     * ========================================================== */
    printf("========== 测试 4：批量读写 + 回绕 ★ ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);

        /* 先写 6 个，读出 4 个，让 write 靠近末尾 */
        rb_write_buf(&rb, (const uint8_t *)"ABCDEF", 6);
        uint8_t tmp[4];
        rb_read_buf(&rb, tmp, 4);
        printf("  写 ABCDEF 再读 4 个后: "); rb_dump(&rb);

        /* 现在 write 在末尾附近，再批量写 4 个 → 会跨越末尾回绕 */
        n = rb_write_buf(&rb, (const uint8_t *)"WXYZ", 4);
        printf("  再批量写 \"WXYZ\" → 返回 %u（期望 4）\n", (unsigned)n);
        rb_dump(&rb);
        printf("  ★ 这次写入【跨越了数组末尾】（write 从 6 绕到 2）\n");
        printf("     所以 rb_write_buf 内部要分【两段】拷贝\n\n");

        /* 全部读出，验证顺序 */
        uint8_t all[RB_CAP + 4];
        n = rb_read_buf(&rb, all, sizeof(all));
        printf("  全部读出 %u 个: ", (unsigned)n);
        dump_bytes(all, n);
        printf("\n  期望: EFWXYZ（4 个剩余 + 4 个新写的）\n\n");
    }

    /* ==========================================================
     * 测试 5：批量读（数据不够）★
     * ========================================================== */
    printf("========== 测试 5：rb_read_buf（数据不够）★ ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        rb_write_buf(&rb, (const uint8_t *)"AB", 2);

        uint8_t dst[8];
        memset(dst, '.', sizeof(dst));      /* 先用 '.' 填满，方便看出哪几个被写了 */
        n = rb_read_buf(&rb, dst, 8);
        printf("  缓冲里只有 2 字节，想读 8 字节 → 返回 %u（期望 2）\n", (unsigned)n);
        printf("  dst 内容: ");
        dump_bytes(dst, 8);
        printf("\n  期望: AB......（只写了前 2 个，后面没动）\n");
        rb_dump(&rb);
        printf("  读完后 empty=%d（期望 1）\n\n", rb_is_empty(&rb));
    }

    /* ==========================================================
     * 测试 6：边界 —— 空读、len=0、NULL
     * ========================================================== */
    printf("========== 测试 6：边界情况 ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        uint8_t dst[4] = {0};

        n = rb_read_buf(&rb, dst, 4);
        printf("  空缓冲区批量读 → 返回 %u（期望 0）\n", (unsigned)n);

        n = rb_write_buf(&rb, (const uint8_t *)"AB", 0);
        printf("  len=0 批量写 → 返回 %u（期望 0）\n", (unsigned)n);
        rb_dump(&rb);

        n = rb_read_buf(&rb, dst, 0);
        printf("  len=0 批量读 → 返回 %u（期望 0）\n", (unsigned)n);

        n = rb_write_buf(&rb, NULL, 0);
        printf("  src=NULL, len=0 → 返回 %u（期望 0，不能崩）\n", (unsigned)n);
        n = rb_read_buf(&rb, NULL, 0);
        printf("  dst=NULL, len=0 → 返回 %u（期望 0，不能崩）\n\n", (unsigned)n);
    }

    /* ==========================================================
     * 测试 7：rb_clear
     * ========================================================== */
    printf("========== 测试 7：rb_clear ==========\n");
    {
        RingBuf rb;
        rb_init(&rb);
        rb_write_buf(&rb, (const uint8_t *)"HELLO", 5);
        printf("  清空前: "); rb_dump(&rb);

        rb_clear(&rb);
        printf("  清空后: "); rb_dump(&rb);
        printf("  empty=%d（期望 1）  count=%u（期望 0）\n", rb_is_empty(&rb), (unsigned)rb_count(&rb));

        /* 清空后必须还能正常写入（检验 read/write 有没有正确复位） */
        n = rb_write_buf(&rb, (const uint8_t *)"NEW", 3);
        printf("  清空后再写 \"NEW\" → 返回 %u（期望 3）\n", (unsigned)n);
        rb_dump(&rb);
        printf("\n");
    }

    /* ==========================================================
     * 测试 8：压力测试 —— 反复批量读写 3000 轮
     *
     * ⚠️ 这里有两个设计要点（都是踩过坑总结的）：
     *
     *  要点 1：【生产 = 消费】时缓冲永不填满，才能严格验证"不丢数据"。
     *          所以用"写 3 读 3"。
     *
     *  要点 2：如果想测"生产快于消费"，必须【正确处理部分写入】——
     *          写入返回 w 后，下一轮要从 src + w 继续，而不是无条件前进。
     *          否则会出现"跳号"：生产者以为写了 3 个，实际只写了 2 个，
     *          第 3 个值从没进过缓冲 → 消费者看到不连续（这是测试的错，
     *          不是缓冲的错）。
     * ========================================================== */
    printf("========== 测试 8A：写 3 读 3（生产=消费，严格不丢）==========\n");
    {
        RingBuf rb;
        rb_init(&rb);

        uint32_t produced = 0, consumed = 0;
        int      order_ok = 1, first = 1;
        uint8_t  prev = 0;
        uint8_t  wbuf[3], rbuf[3];
        int      fail_round = -1;

        for (int round = 0; round < 3000; round++)
        {
            for (int k = 0; k < 3; k++) { wbuf[k] = (uint8_t)(produced & 0xFF); produced++; }
            if (rb_write_buf(&rb, wbuf, 3) != 3) { fail_round = round; break; }
            if (rb_read_buf(&rb, rbuf, 3) != 3)  { fail_round = round; break; }

            for (int k = 0; k < 3; k++)
            {
                if (!first && (uint8_t)(prev + 1) != rbuf[k]) order_ok = 0;
                prev = rbuf[k]; first = 0; consumed++;
            }
            if (!order_ok) { fail_round = round; break; }
        }

        if (fail_round >= 0) printf("  第 %d 轮出错\n", fail_round);
        printf("  生产 %u，消费 %u\n", (unsigned)produced, (unsigned)consumed);
        printf("  顺序连续 = %s（期望 是）\n", order_ok ? "是" : "否");
        printf("  最终 count = %u（期望 0 —— 生产=消费，缓冲应读空）\n\n",
               (unsigned)rb_count(&rb));
    }

    printf("========== 测试 8B：想写 3 读 2（正确处理部分写入）==========\n");
    {
        RingBuf rb;
        rb_init(&rb);

        uint32_t next_val = 0;      /* 下一个要写的值 —— 只在真正写进去后才推进 */
        uint32_t consumed = 0, total_written = 0;
        int      order_ok = 1, first = 1;
        uint8_t  prev = 0;
        int      partial_rounds = 0;

        for (int round = 0; round < 3000; round++)
        {
            uint8_t wbuf[3];
            for (int k = 0; k < 3; k++) wbuf[k] = (uint8_t)((next_val + k) & 0xFF);

            uint32_t w = rb_write_buf(&rb, wbuf, 3);
            if (w < 3) partial_rounds++;
            next_val += w;              /* ★ 关键：只推进实际写进去的数量 */
            total_written += w;

            uint8_t rbuf[2];
            uint32_t g = rb_read_buf(&rb, rbuf, 2);
            for (uint32_t k = 0; k < g; k++)
            {
                if (!first && (uint8_t)(prev + 1) != rbuf[k]) order_ok = 0;
                prev = rbuf[k]; first = 0; consumed++;
            }
        }

        printf("  实际写入 %u，消费 %u\n", (unsigned)total_written, (unsigned)consumed);
        printf("  发生部分写入的轮数 = %d（正常：生产快于消费，缓冲会满）\n", partial_rounds);
        printf("  顺序连续 = %s（期望 是）\n", order_ok ? "是" : "否");
        printf("  校验：写入 - 消费 = %u，count = %u → %s\n",
               (unsigned)(total_written - consumed), (unsigned)rb_count(&rb),
               (total_written - consumed == rb_count(&rb)) ? "一致 ✅" : "不一致 ❌");
        printf("  ★ 要点：部分写入时，调用方必须从 src + w 继续，否则会丢数据\n\n");
    }

    printf("========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 基础 5 个（默写：昨天写过，不看参考）---------- */

void rb_init(RingBuf *rb)
{
    rb->count=0;
    rb->read=0;
    rb->write=0;
}

int rb_is_empty(const RingBuf *rb)
{
    return (rb->count==0);
}

int rb_is_full(const RingBuf *rb)
{
    return ( rb->count==RB_CAP);
}

int rb_put(RingBuf *rb, uint8_t data)
{
    if( rb->count!=RB_CAP)
    {
        rb->buf[rb->write]=data;
        rb->write=(rb->write+1)%RB_CAP;
        rb->count++;
        return 1;
    }
    return 0;
}

int rb_get(RingBuf *rb, uint8_t *data)
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

/* ---------- 今天新学的 4 个 ---------- */

/* 1. rb_count —— 当前元素个数
 * 方案 B 直接返回 count 就行（一行）
 */
uint32_t rb_count(const RingBuf *rb)
{
    return rb->count;
}

/* 2. rb_clear —— 清空缓冲区
 * 要求：read / write / count 都复位（buf 内容不用清）
 * 提示：清空后必须还能正常写入
 */
void rb_clear(RingBuf *rb)
{
    rb->read=0;
    rb->write=0;
    rb->count=0;
}

/* 3. rb_write_buf —— 一次写多个字节 ★
 * 参数：src 源数据，len 想写几个字节
 * 返回：【实际写入】了几个字节
 *
 * 要求：
 *   - 能写几个就写几个（空间不够就少写，不要覆盖已有数据）
 *   - 返回实际写入数（0 ~ len）
 *   - src 为 NULL 或 len 为 0 → 返回 0
 */
uint32_t rb_write_buf(RingBuf *rb, const uint8_t *src, uint32_t len)
{
    if(src==NULL||len==0)
    {
        return 0;
    }
    uint32_t can_write=RB_CAP-rb->count;
    uint32_t real_write=(can_write<len)?can_write:len;
    uint32_t wr=rb->write;
    uint32_t first=RB_CAP-wr;
    if(first>real_write)
    {
        first=real_write;
    }
    uint32_t second=real_write-first;
    memcpy(rb->buf+wr,src,first);
    if(second>0)
    {
        memcpy(rb->buf,src+first,second);
    }
    rb->write =(rb->write+real_write)%RB_CAP;
    rb->count+=real_write;
    return real_write;
}

/* 4. rb_read_buf —— 一次读多个字节 ★
 * 参数：dst 目标缓冲区，len 想读几个字节
 * 返回：【实际读出】了几个字节
 *
 * 要求：
 *   - 有多少读多少（数据不够就少读）
 *   - 返回实际读出数（0 ~ len）
 *   - dst 为 NULL 或 len 为 0 → 返回 0
 *
 * 实现要点（和 write 对称）：
 *   ① 算能读几个：min(rb->count, len)
 *   ② 分两段拷贝（从 rd 到数组末尾 + 从 0 开始）
 *         uint32_t rd    = rb->read;
 *         uint32_t first = RB_CAP - rd;
 *         if (first > n) first = n;
 *         uint32_t second = n - first;
 *   ③ 更新 read（回绕）和 count
 */
uint32_t rb_read_buf(RingBuf *rb, uint8_t *dst, uint32_t len)
{
    if(dst==NULL||len==0)
    {
        return 0;
    }
    uint32_t can_read=rb->count;
    uint32_t real_read=(can_read<len)?can_read:len;
    uint32_t rd=rb->read;
    uint32_t first=RB_CAP-rd;
    if(first>real_read)
    {
        first=real_read;
    }
    uint32_t second=real_read-first;
    memcpy(dst,rb->buf+rd,first);
    if(second>0)
    {
        memcpy(dst+first,rb->buf,second);
    }
    rb->read =(rb->read+real_read)%RB_CAP;
    rb->count-=real_read;
    return real_read;
}
