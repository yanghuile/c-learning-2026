/* ============================================================
 *  W6-Day3 环形缓冲区（二）：满/空判定的两种方案 ★本周重点
 *
 *  今天实现【两套】环形缓冲，然后对比它们。
 *
 *  运行：b exercises/W6-Day3-满空判定两方案.c
 *
 *  ┌──────────────────────────────────────────────────────────┐
 *  │  核心问题（先想清楚再写代码）：                            │
 *  │                                                          │
 *  │    read == write  时，到底是【空】还是【满】？              │
 *  │                                                          │
 *  │    - 一个元素都没写：read=0, write=0                      │
 *  │    - 写了整整一轮：  read=0, write=0                      │
 *  │                                                          │
 *  │    两种状态【完全一样】！光看两个指针分不出来。            │
 *  │    → 所以必须引入【额外信息】                             │
 *  └──────────────────────────────────────────────────────────┘
 *
 *  两套方案：
 *    方案 A：牺牲一个空间   —— (write+1)%CAP == read 就算满
 *    方案 B：加计数器 count —— count == 0 空，count == CAP 满
 *
 *  ⚠️ 边界提醒（你 W5 的老毛病）：
 *    空、满、容量 1、回绕、连续读写几百次 —— 每个函数都要想这几种情况。
 * ============================================================ */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CAP 8               /* 两个方案用同样的数组容量，才好对比 */

/* ============================================================
 *  方案 A：牺牲一个空间（没有 count 字段）
 *  CAP 个格子，最多只能存 CAP-1 个元素
 * ============================================================ */
typedef struct
{
    uint8_t  buf[CAP];
    uint32_t write;         /* 下一个要写的位置 */
    uint32_t read;          /* 下一个要读的位置 */
} RingBufA;

/* ============================================================
 *  方案 B：计数器（你昨天写的就是这个）
 *  CAP 个格子，能存满 CAP 个元素
 * ============================================================ */
typedef struct
{
    uint8_t  buf[CAP];
    uint32_t write;
    uint32_t read;
    uint32_t count;         /* 当前元素个数 ★ 多出来的 4 字节 */
} RingBufB;

/* ---------- 函数声明 ---------- */
void a_init(RingBufA *rb);
int  a_is_empty(const RingBufA *rb);
int  a_is_full(const RingBufA *rb);
int  a_put(RingBufA *rb, uint8_t data);
int  a_get(RingBufA *rb, uint8_t *data);
int  a_count(const RingBufA *rb);       /* 方案 A 想求个数得【算】出来 */

void b_init(RingBufB *rb);
int  b_is_empty(const RingBufB *rb);
int  b_is_full(const RingBufB *rb);
int  b_put(RingBufB *rb, uint8_t data);
int  b_get(RingBufB *rb, uint8_t *data);

/* 覆盖式写入（满了就丢最旧的）—— 课后题用，先声明 */
void  b_put_overwrite(RingBufB *rb, uint8_t data);

/* ============================================================
 *  打印辅助（已写好）
 * ============================================================ */
static void a_dump(const RingBufA *rb)
{
    printf("     [A] read=%u write=%u full=%d empty=%d count=%d | 数据: ",
           (unsigned)rb->read, (unsigned)rb->write,
           a_is_full(rb), a_is_empty(rb), a_count(rb));
    if (a_is_empty(rb)) { printf("（空）\n"); return; }
    uint32_t i = rb->read;
    while (i != rb->write)
    {
        printf("[%u]=%c ", (unsigned)i, rb->buf[i]);
        i = (i + 1) % CAP;
    }
    printf("\n");
}

static void b_dump(const RingBufB *rb)
{
    printf("     [B] read=%u write=%u count=%u full=%d empty=%d | 数据: ",
           (unsigned)rb->read, (unsigned)rb->write, (unsigned)rb->count,
           b_is_full(rb), b_is_empty(rb));
    if (rb->count == 0) { printf("（空）\n"); return; }
    for (uint32_t i = 0; i < rb->count; i++)
    {
        uint32_t idx = (rb->read + i) % CAP;
        printf("[%u]=%c ", (unsigned)idx, rb->buf[idx]);
    }
    printf("\n");
}

/* ============================================================
 *  main：测试（已写好，不用改）
 * ============================================================ */
int main(void)
{
    uint8_t out;
    int r;

    /* ==========================================================
     * 测试 1【今天的核心】歧义演示
     * ========================================================== */
    printf("========== 测试 1：read==write 的歧义 ==========\n");
    {
        RingBufB e, f;
        b_init(&e);
        printf("  空缓冲区:   read=%u write=%u  → read==write ? %s\n",
               (unsigned)e.read, (unsigned)e.write,
               e.read == e.write ? "是" : "否");

        b_init(&f);
        for (int i = 0; i < CAP; i++) b_put(&f, (uint8_t)('A' + i));
        printf("  写满一整轮: read=%u write=%u  → read==write ? %s\n",
               (unsigned)f.read, (unsigned)f.write,
               f.read == f.write ? "是" : "否");

        printf("\n  >>> 两种状态的 read/write【完全相同】，但一个空一个满！\n");
        printf("  >>> 所以必须靠额外信息区分 —— 这就是今天两种方案存在的理由\n\n");
    }

    /* ==========================================================
     * 测试 2：两个方案的容量对比（同样 CAP，能存几个？）
     * ========================================================== */
    printf("========== 测试 2：容量对比（数组都是 %d 字节）==========\n", CAP);
    {
        RingBufA a; a_init(&a);
        RingBufB b; b_init(&b);
        int ca = 0, cb = 0;

        for (int i = 0; i < 20; i++) { if (a_put(&a, (uint8_t)('A' + i))) ca++; else break; }
        for (int i = 0; i < 20; i++) { if (b_put(&b, (uint8_t)('A' + i))) cb++; else break; }

        printf("  方案 A 连续 put 成功 %d 次   （期望 %d = CAP-1）\n", ca, CAP - 1);
        printf("  方案 B 连续 put 成功 %d 次   （期望 %d = CAP）\n", cb, CAP);
        printf("  → 方案 A 浪费了 1 个格子（%.1f%%），方案 B 没有浪费\n",
               100.0 / CAP);
        printf("  → 代价：方案 A 少存 1 个，方案 B 多占 4 字节（一个 uint32 的 count）\n\n");
        a_dump(&a);
        b_dump(&b);
        printf("\n");
    }

    /* ==========================================================
     * 测试 3：回绕后两个方案都要正确
     * ========================================================== */
    printf("========== 测试 3：回绕（写满 → 读几个 → 再写）==========\n");
    {
        RingBufA a; a_init(&a);
        RingBufB b; b_init(&b);

        /* 各写满 */
        for (int i = 0; i < CAP; i++) { a_put(&a, (uint8_t)('1' + i)); b_put(&b, (uint8_t)('1' + i)); }
        printf("  写满后:\n"); a_dump(&a); b_dump(&b);

        /* 各读 3 个 */
        for (int i = 0; i < 3; i++) { a_get(&a, &out); b_get(&b, &out); }
        printf("  读 3 个后:\n"); a_dump(&a); b_dump(&b);

        /* 再各写 3 个（触发回绕） */
        for (int i = 0; i < 3; i++) { a_put(&a, (uint8_t)('x' + i)); b_put(&b, (uint8_t)('x' + i)); }
        printf("  再写 3 个后（write 回绕）:\n"); a_dump(&a); b_dump(&b);

        /* 全部读出，验证 FIFO 顺序一致 */
        printf("  方案 A 全部读出: ");
        while (!a_is_empty(&a)) { a_get(&a, &out); printf("%c ", out); }
        printf("\n  方案 B 全部读出: ");
        while (!b_is_empty(&b)) { b_get(&b, &out); printf("%c ", out); }
        printf("\n  （两个方案的输出顺序应该完全一样）\n\n");
    }

    /* ==========================================================
     * 测试 4：容量为 1 和容量为 2 的极端情况
     * ========================================================== */
    printf("========== 测试 4：极端容量 ==========\n");
    {
        /* 注意：这个测试用 CAP=8 的数组，但我们要模拟"只有 1 个格子" */
        printf("  ---- 用 RingBufA/RingBufB，但容量概念是 CAP=%d ----\n", CAP);
        printf("  问题：如果 CAP=1，方案 A 会怎样？\n");
        printf("    方案 A 满的条件: (write+1)%%1 == read  →  0 == read  →  【永远是满】\n");
        printf("    → 方案 A 在 CAP=1 时【一个都存不了】！这是它的致命缺陷\n");
        printf("    （这个结论你自己用计算器验一下：(0+1)%%1 = 0，而 read 始终是 0）\n\n");

        printf("  问题：CAP=2 时方案 A 能存几个？\n");
        printf("    答案：1 个。存 1 个就满了。\n\n");
    }

    /* ==========================================================
     * 测试 5：压力测试（两个方案都不丢数据、顺序不乱）
     * ========================================================== */
    printf("========== 测试 5：反复读写 5000 轮 ==========\n");
    {
        RingBufA a; a_init(&a);
        RingBufB b; b_init(&b);

        uint32_t na = 0, nb = 0, ga = 0, gb = 0;
        int ordA = 1, ordB = 1, fA = 1, fB = 1;
        uint8_t pA = 0, pB = 0;

        for (int i = 0; i < 5000; i++)
        {
            /* 各写 3 个（能写多少写多少） */
            for (int k = 0; k < 3; k++) { if (a_put(&a, (uint8_t)na)) na++; else break; }
            for (int k = 0; k < 3; k++) { if (b_put(&b, (uint8_t)nb)) nb++; else break; }

            /* 各读 2 个 */
            for (int k = 0; k < 2; k++)
            {
                if (a_get(&a, &out)) {
                    if (!fA && (uint8_t)(pA + 1) != out) ordA = 0;
                    pA = out; fA = 0; ga++;
                } else break;
            }
            for (int k = 0; k < 2; k++)
            {
                if (b_get(&b, &out)) {
                    if (!fB && (uint8_t)(pB + 1) != out) ordB = 0;
                    pB = out; fB = 0; gb++;
                } else break;
            }
        }

        printf("  方案 A: 写入 %u，读出 %u，顺序连续 = %s\n",
               (unsigned)na, (unsigned)ga, ordA ? "是" : "否");
        printf("  方案 B: 写入 %u，读出 %u，顺序连续 = %s\n",
               (unsigned)nb, (unsigned)gb, ordB ? "是" : "否");
        printf("  （A 比 B 少写一些是正常的 —— A 少一个格子）\n\n");
    }

    /* ==========================================================
     * 测试 6：溢出策略对比 —— 丢新数据 vs 覆盖最旧
     * ========================================================== */
    printf("========== 测试 6：满了还继续写，两种策略 ==========\n");

    printf("  策略 1【丢新数据】—— 用 b_put（满了返回 0）:\n");
    {
        RingBufB b; b_init(&b);
        for (int i = 0; i < CAP; i++) b_put(&b, (uint8_t)('1' + i));    /* 1..8 */
        r = b_put(&b, 'X');                                            /* 想写 X */
        printf("     存满 1..8 后写 'X' → 返回 %d（拒绝）\n", r);
        printf("     读出: ");
        while (!b_is_empty(&b)) { b_get(&b, &out); printf("%c", out); }
        printf("   → 得到 12345678（'X' 丢了，【最旧的保住了】）\n\n");
    }

    printf("  策略 2【覆盖最旧】—— 用 b_put_overwrite:\n");
    {
        RingBufB b; b_init(&b);
        for (int i = 0; i < CAP; i++) b_put_overwrite(&b, (uint8_t)('1' + i));
        b_put_overwrite(&b, 'X');
        printf("     存满 1..8 后写 'X'（覆盖式，不会失败）\n");
        printf("     count 应该还是 %d（满了也不增长，只是换了个元素）: %u\n",
               CAP, (unsigned)b.count);
        printf("     读出: ");
        while (!b_is_empty(&b)) { b_get(&b, &out); printf("%c", out); }
        printf("   → 得到 2345678X（【最新的保住了，最旧的 '1' 被挤掉】）\n");
        printf("     ★ 注意 count 始终 <= CAP —— 这是覆盖式的特点\n\n");
    }

    printf("========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面由你实现 ↓↓↓
 *
 *  实现要点：
 *    - a_is_full 用「牺牲一格」的思路：(write + 1) % CAP == read
 *    - b_is_full 用计数器：count == CAP
 *    - a_count 需要【算】出来：(write - read + CAP) % CAP
 *      ★ 这一行很关键：注意加 CAP 再取余，否则负数取余结果是负的！
 *    - b_put_overwrite：满了就先丢最旧的（read 前移一格、count--），再写入
 * ============================================================ */

/* ---------- 方案 A：牺牲一个空间 ---------- */
void a_init(RingBufA *rb)
{
    rb->read=0;
    rb->write=0;
    return;
}

int a_is_empty(const RingBufA *rb)
{
    if(rb->read==rb->write)
    {
        return 1;
    }
    return 0;
}

int a_is_full(const RingBufA *rb)
{
    if(((rb->write+1)%CAP)==rb->read)
    {
        return 1;
    }
    return 0;
}

int a_put(RingBufA *rb, uint8_t data)
{
    if(((rb->write+1)%CAP)!=rb->read)
    {
        rb->buf[rb->write]=data;
        rb->write=(rb->write+1)%CAP;
        return 1;
    }
    return 0;
}

int a_get(RingBufA *rb, uint8_t *data)
{
    if(rb->read!=rb->write)
    {
        *data=rb->buf[rb->read];
        rb->read=(rb->read+1)%CAP;
        return 1;
    }
    return 0;
}

int a_count(const RingBufA *rb)
{
    if(rb->read!=rb->write)
    {
        return (rb->write-rb->read+CAP)%CAP;
    }
    return 0;
}

/* ---------- 方案 B：计数器 ---------- */
void b_init(RingBufB *rb)
{
    rb->count=0;
    rb->read=0;
    rb->write=0;
    return;
}

int b_is_empty(const RingBufB *rb)
{
    return (rb->count==0);
}

int b_is_full(const RingBufB *rb)
{
    return (rb->count==CAP);
}

int b_put(RingBufB *rb, uint8_t data)
{
    if(rb->count!=CAP)
    {
        rb->buf[rb->write]=data;
        rb->write=(rb->write+1)%CAP;
        rb->count++;
        return 1;
    }
    return 0;
}

int b_get(RingBufB *rb, uint8_t *data)
{
    /* TODO: 空 → 0（不改 *data）；否则取值 → read 回绕 → count-- */
    if(rb->count!=0)
    {
        *data=rb->buf[rb->read];
        rb->read=(rb->read+1)%CAP;
        rb->count--;
        return 1;
    }
    return 0;
}

/* ---------- 覆盖式写入（课后题）---------- */
void b_put_overwrite(RingBufB *rb, uint8_t data)
{
    /* TODO: 如果满了，先丢最旧的（read 前移一格 + count--），再正常写入
     * 提示：可以复用 b_put，但要注意 b_put 会检查满 */
    if(b_is_full(rb)==1)
    {
        rb->read=(rb->read+1)%CAP;
        rb->count--;
    }
    rb->buf[rb->write]=data;
    rb->write=(rb->write+1)%CAP;
    rb->count++;
}


/*1.为什么不能只靠 read == write 判断空/满？
答：缓冲区为空时，read==write;缓冲区写满绕一圈后，还是read==write;空和满会有歧义，都是read==write，不能判断
2.如果要给 STM32 写一个"串口接收缓冲"（256 字节），你选哪个方案？为什么？
答：我选B。如果用A，实际只能存255字节，直接损失1字节的数据容量，浪费数组存储空间；而B只是结构体多占用4字节的count变量，256字节可以全部拿来存储数据。
3.方案A在CAP=1时会怎么样？为什么？
答：一个数据都不能写入。方案A要用一个空位区分空和满，物理数组大小为1，预留一个空位后，已经没有位置存放有效数据。
*/
