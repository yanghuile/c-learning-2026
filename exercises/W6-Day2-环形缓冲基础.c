/* ============================================================
 *  W6-Day2 环形缓冲区（一）：基本结构
 *
 *  今天实现 5 个函数（见文件末尾 TODO）。
 *  方案：用【计数器 count】记录元素个数（周三再对比"牺牲一个空间"的方案）
 *
 *  运行：
 *      b exercises/W6-Day2-环形缓冲基础.c
 *
 *  ⚠️ 环形缓冲是【边界密集型】数据结构。写每个函数前先问：
 *     空的时候？满的时候？容量为 1 的时候？读写指针正好在末尾回绕的时候？
 *     （这是你 W5 反复踩坑的地方，这周正好练这个习惯）
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdint.h>     /* uint8_t / uint32_t —— 嵌入式里用固定宽度类型 */

/* ============================================================
 *  环形缓冲区结构体
 *
 *  设计说明（想清楚这几个问题再往下看代码）：
 *    - 为什么用 uint8_t 而不是 char？
 *      因为"字节"在嵌入式里是无符号数据，uint8_t 语义更准（且你 W5 学过无符号的好处）
 *    - 为什么缓冲区数组要开 RB_MAX_SIZE 而不是动态分配？
 *      嵌入式怕 malloc 碎片，静态分配最安全（这是嵌入式与 PC 编程的重要区别）
 *    - read 和 write 是"下一个要读/写的位置"，不是"最后一个读/写过的位置"
 *      这个约定很重要，写代码时时刻记住
 * ============================================================ */

#define RB_MAX_SIZE 8               /* 缓冲区最大容量（静态分配） */

typedef struct
{
    uint8_t  buf[RB_MAX_SIZE];      /* 存放数据的数组 */
    uint32_t capacity;              /* 实际容量（<= RB_MAX_SIZE） */
    uint32_t read;                  /* 下一个要【读】的位置 */
    uint32_t write;                 /* 下一个要【写】的位置 */
    uint32_t count;                 /* 当前已有的元素个数 ★ 方案 B 的关键 */
} RingBuf;

/* ============================================================
 *  函数声明
 * ============================================================ */
void rb_init(RingBuf *rb, uint32_t capacity);
int  rb_is_empty(const RingBuf *rb);
int  rb_is_full(const RingBuf *rb);
int  rb_put(RingBuf *rb, uint8_t data);
int  rb_get(RingBuf *rb, uint8_t *data);

/* ============================================================
 *  辅助：打印缓冲区当前状态（已写好，方便你观察）
 * ============================================================ */
static void rb_dump(const RingBuf *rb)
{
    printf("    [状态] count=%u  read=%u  write=%u  |  ",
           (unsigned)rb->count, (unsigned)rb->read, (unsigned)rb->write);

    /* 防御：capacity 为 0 时取余会崩，先挡住 */
    if (rb->count == 0 || rb->capacity == 0)
    {
        printf("（空）\n");
        return;
    }

    /* 从 read 开始，按顺序打印 count 个元素 */
    printf("数据: ");
    for (uint32_t i = 0; i < rb->count; i++)
    {
        uint32_t idx = (rb->read + i) % rb->capacity;
        printf("[%u]=%c ", (unsigned)idx, rb->buf[idx]);
    }
    printf("\n");
}

/* ============================================================
 *  main：测试代码（已写好，不用改）
 * ============================================================ */
int main(void)
{
    RingBuf rb;
    uint8_t out = 0;

    /* ---------- 测试 1：初始化 + 空判断 ---------- */
    printf("========== 测试 1：初始化 ==========\n");
    rb_init(&rb, 8);
    printf("初始化后: ");
    rb_dump(&rb);
    printf("rb_is_empty = %d   （期望 1）\n", rb_is_empty(&rb));
    printf("rb_is_full  = %d   （期望 0）\n\n", rb_is_full(&rb));

    /* ---------- 测试 2：写入直到满 ---------- */
    printf("========== 测试 2：写入直到满 ==========\n");
    const char *msg = "ABCDEFGH";
    for (int i = 0; i < 8; i++)
    {
        int r = rb_put(&rb, (uint8_t)msg[i]);
        printf("put('%c') -> 返回 %d   ", msg[i], r);
        rb_dump(&rb);
    }
    printf("rb_is_full  = %d   （期望 1）\n", rb_is_full(&rb));
    printf("rb_is_empty = %d   （期望 0）\n\n", rb_is_empty(&rb));

    /* ---------- 测试 3：满了还继续写 ---------- */
    printf("========== 测试 3：满了继续写（应该失败）==========\n");
    int r = rb_put(&rb, (uint8_t)'X');
    printf("put('X') 在满缓冲区上 -> 返回 %d   （期望 0 = 失败）\n", r);
    rb_dump(&rb);
    printf("\n");

    /* ---------- 测试 4：读出，验证 FIFO 顺序 ---------- */
    printf("========== 测试 4：读出（FIFO）==========\n");
    for (int i = 0; i < 3; i++)
    {
        int r2 = rb_get(&rb, &out);
        printf("get() -> 返回 %d, 读出 '%c'   （期望依次 A B C）\n", r2, out);
    }
    rb_dump(&rb);
    printf("\n");

    /* ---------- 测试 5：回绕（写到末尾后回到开头）---------- */
    printf("========== 测试 5：回绕 ==========\n");
    printf("现在写入 I J K（write 会绕回下标 0）:\n");
    rb_put(&rb, (uint8_t)'I');
    rb_put(&rb, (uint8_t)'J');
    rb_put(&rb, (uint8_t)'K');
    rb_dump(&rb);
    printf("  验证顺序: get 出来应该是 D E F G H I J K\n");
    printf("  实际: ");
    while (!rb_is_empty(&rb))
    {
        rb_get(&rb, &out);
        printf("%c ", out);
    }
    printf("\n");
    printf("读空后: ");
    rb_dump(&rb);
    printf("\n");

    /* ---------- 测试 6：空缓冲区读（应该失败）---------- */
    printf("========== 测试 6：空缓冲区读（应该失败）==========\n");
    r = rb_get(&rb, &out);
    printf("get() 在空缓冲区上 -> 返回 %d   （期望 0 = 失败）\n", r);
    printf("  并且 out 不应该被修改（out 此时还是 %c）\n\n", out);

    /* ---------- 测试 7：边界 —— 容量为 1 的缓冲区 ---------- */
    printf("========== 测试 7：容量为 1 ==========\n");
    RingBuf rb1;
    rb_init(&rb1, 1);
    printf("put('Z') -> %d   （期望 1）\n", rb_put(&rb1, (uint8_t)'Z'));
    rb_dump(&rb1);
    printf("put('Y') -> %d   （期望 0，已满）\n", rb_put(&rb1, (uint8_t)'Y'));

    /* ⚠️ 注意：不要写成 printf(..., rb_get(&rb1, &out), out)
     *    因为函数参数的求值顺序是【未指定的】，out 可能在 rb_get 之前就被读取，
     *    打印出来的是旧值。必须拆成两条语句，保证顺序。 */
    {
        int gr = rb_get(&rb1, &out);
        printf("get(&out) -> %d, out='%c'   （期望 1 和 Z）\n", gr, out);
    }

    printf("读空后再 put('W') -> %d   （期望 1）\n", rb_put(&rb1, (uint8_t)'W'));
    rb_dump(&rb1);
    printf("\n");

    /* ---------- 测试 7b：capacity 传 0（边界）---------- */
    printf("========== 测试 7b：capacity 传 0 ==========\n");
    RingBuf rb0;
    rb_init(&rb0, 0);
    printf("rb_init(&rb0, 0) 之后: ");
    rb_dump(&rb0);
    /* 关键：不能出现 % 0（除零会崩）。rb_init 应该把 0 改成 1 */
    printf("  capacity = %u   （期望 >= 1，不能是 0，否则 %% 运算会崩）\n",
           (unsigned)rb0.capacity);
    printf("  put('M') -> %d   （期望 1）\n", rb_put(&rb0, (uint8_t)'M'));
    {
        int gr = rb_get(&rb0, &out);
        printf("  get() -> %d, out='%c'   （期望 1 和 M）\n", gr, out);
    }
    printf("\n");

    /* ---------- 测试 7c：capacity 超过最大值（防越界）---------- */
    printf("========== 测试 7c：capacity 传 100（超过 RB_MAX_SIZE=%d）==========\n",
           RB_MAX_SIZE);
    RingBuf rbbig;
    rb_init(&rbbig, 100);
    printf("  capacity = %u   （期望被截断成 %d，否则 buf[] 会越界写）\n",
           (unsigned)rbbig.capacity, RB_MAX_SIZE);
    /* 如果没截断，下面写 100 个字节会踩坏栈上的其他变量 */
    for (int i = 0; i < 100; i++)
    {
        if (!rb_put(&rbbig, (uint8_t)('a' + (i % 26)))) break;
    }
    printf("  连续写了 100 次后: ");
    rb_dump(&rbbig);
    printf("  （如果 capacity 没截断，这里会看到内存被破坏或程序崩溃）\n\n");

    /* ---------- 测试 8：反复读写 10000 次（不丢数据）---------- */
    printf("========== 测试 8：反复读写 10000 次 ==========\n");
    rb_init(&rb, 8);
    uint32_t put_ok = 0, get_ok = 0;
    int      mismatch = 0;
    uint32_t expect_val = 0;

    for (int i = 0; i < 10000; i++)
    {
        /* 写几个 */
        for (int k = 0; k < 3 && !rb_is_full(&rb); k++)
        {
            if (rb_put(&rb, (uint8_t)(expect_val & 0xFF))) put_ok++;
            expect_val++;
        }
        /* 读几个 */
        uint8_t tmp;
        for (int k = 0; k < 2 && !rb_is_empty(&rb); k++)
        {
            uint32_t before = rb.count;
            if (rb_get(&rb, &tmp)) get_ok++;
            /* 检查 count 是否正确递减 */
            if (rb.count != before - 1) mismatch++;
        }
    }
    printf("累计写入成功 %u 次，读出成功 %u 次\n", (unsigned)put_ok, (unsigned)get_ok);
    printf("count 计数错误次数 = %d   （期望 0）\n", mismatch);
    printf("最后状态: ");
    rb_dump(&rb);
    printf("  验证：把剩下的全部读出来，检查顺序是否连续递增\n");

    int order_ok = 1;
    uint32_t prev = 0;
    int      first = 1;
    while (!rb_is_empty(&rb))
    {
        rb_get(&rb, &out);
        if (!first && (uint32_t)out != ((prev + 1) & 0xFF)) { order_ok = 0; break; }
        prev = out;
        first = 0;
    }
    printf("  剩余数据顺序连续: %s\n", order_ok ? "是（正确）" : "否（错误！）");
    printf("\n");

    printf("========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面 5 个函数由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 1. 初始化 ----------
 * 要求：
 *   - 把 capacity 存进结构体
 *   - read / write / count 都清零
 *   - 缓冲区内容不用清（反正 count=0 表示没有数据）
 *   - 如果 capacity > RB_MAX_SIZE，要截断成 RB_MAX_SIZE（防止越界！）
 *   - 如果 capacity == 0，也要处理（你可以设成 1，或者让后续函数判 0）
 */
void rb_init(RingBuf *rb, uint32_t capacity)
{
    rb->read=0;
    rb->write=0;
    rb->count=0;
    if(capacity==0)
    {
        capacity=1;
    }
    if(capacity > RB_MAX_SIZE)
    {
        capacity=RB_MAX_SIZE;
    }
    rb->capacity=capacity;
}


/* ---------- 2. 判断空 ----------
 * 返回 1 表示空，0 表示非空
 * 空的条件：count == 0
 */
int rb_is_empty(const RingBuf *rb)
{
    return (rb->count==0);
}


/* ---------- 3. 判断满 ----------
 * 返回 1 表示满，0 表示未满
 * 满的条件：count == capacity
 */
int rb_is_full(const RingBuf *rb)
{
    return (rb->count==rb->capacity);
}


/* ---------- 4. 写入一个字节 ----------
 * 成功返回 1，失败（缓冲区满）返回 0
 *
 * 步骤：
 *   1. 满了就返回 0（不能覆盖！今天的策略是"丢新数据"）
 *   2. 把 data 存到 buf[write] 位置
 *   3. write 前进一格，到底了就回绕：write = (write + 1) % capacity
 *   4. count 加 1
 *
 * ⚠️ 注意顺序：先存、再移动 write、最后加 count
 */
int rb_put(RingBuf *rb, uint8_t data)
{
    if(rb->count==rb->capacity)
    {
        return 0;
    }
    rb->buf[rb->write]=data;
    rb->write=(rb->write+1)%rb->capacity;
    rb->count++;
    return 1;
}


/* ---------- 5. 读出一个字节 ----------
 * 成功返回 1 并把数据写到 *data；失败（缓冲区空）返回 0
 *
 * 步骤：
 *   1. 空了就返回 0（并且【不要修改 *data】）
 *   2. 把 buf[read] 的值取出来，写到 *data
 *   3. read 前进一格，到底了就回绕：read = (read + 1) % capacity
 *   4. count 减 1
 *
 * ⚠️ 注意顺序：先取值、再移动 read、最后减 count
 */
int rb_get(RingBuf *rb, uint8_t *data)
{
    if(rb->count==0)
    {
        return 0;
    }
    *data=rb->buf[rb->read];
    rb->read=(rb->read+1)%rb->capacity;
    rb->count--;
    return 1;
}
