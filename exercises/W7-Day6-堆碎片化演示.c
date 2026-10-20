/* ============================================================
 *  W7-Day6 演示：堆碎片化为什么在嵌入式里致命
 *
 *  编译运行：
 *      b exercises/W7-Day6-堆碎片化演示.c
 *
 *  ═══════════════════════════════════════════════════════════
 *  为什么在 PC 上模拟，而不是在板子上跑？
 *  ═══════════════════════════════════════════════════════════
 *
 *  因为板子上你【看不到】堆的内部。
 *  这个程序把堆的布局画出来，让"碎片化"从抽象概念变成可见的图。
 *
 *  堆大小用 C8T6 的真实值：20 KB SRAM。
 *
 *  模型说明（故意简化）：
 *    把堆切成 160 个 128 字节的"段"，用位图记录每段是否被占用。
 *    分配时找【连续】的空闲段。真实 malloc 是块式管理，
 *    但"必须连续"这个约束完全一样 —— 碎片化就来自这里。
 * ============================================================ */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEAP_SIZE   20480                    /* 20 KB = C8T6 的 SRAM */
#define SEG_SIZE    128                      /* 每段 128 字节 */
#define NSEG        (HEAP_SIZE / SEG_SIZE)   /* 160 段 */

static uint8_t seg_used[NSEG];

/* 分配记录（模拟 malloc 返回的指针） */
typedef struct
{
    int start;      /* 起始段号 */
    int nseg;       /* 占用段数 */
} Alloc;

/* ---------- 初始化 ---------- */
static void heap_init(void)
{
    memset(seg_used, 0, sizeof(seg_used));
}

/* ---------- 分配：找连续的 n 段空闲 ---------- */
static int heap_alloc(int bytes, Alloc *out)
{
    int n = (bytes + SEG_SIZE - 1) / SEG_SIZE;    /* 需要几段 */
    int run = 0;

    for (int i = 0; i < NSEG; i++)
    {
        if (!seg_used[i])
        {
            run++;
            if (run == n)
            {
                int start = i - n + 1;
                for (int k = start; k <= i; k++) seg_used[k] = 1;
                if (out) { out->start = start; out->nseg = n; }
                return 0;                          /* 成功 */
            }
        }
        else
        {
            run = 0;                               /* 断了，重新计 */
        }
    }
    return -1;                                     /* 失败（模拟返回 NULL） */
}

/* ---------- 释放 ---------- */
static void heap_free(const Alloc *a)
{
    if (!a) return;
    for (int k = 0; k < a->nseg; k++)
    {
        int idx = a->start + k;
        if (idx >= 0 && idx < NSEG) seg_used[idx] = 0;
    }
}

/* ---------- 统计 ---------- */
static void heap_stats(int *total_free_seg, int *largest_free_seg)
{
    int total = 0, largest = 0, run = 0;
    for (int i = 0; i < NSEG; i++)
    {
        if (!seg_used[i])
        {
            total++;
            run++;
            if (run > largest) largest = run;
        }
        else
        {
            run = 0;
        }
    }
    *total_free_seg   = total;
    *largest_free_seg = largest;
}

/* ---------- 打印堆布局 ---------- */
static void heap_dump(const char *tag, int show_map)
{
    int tf, lf;
    heap_stats(&tf, &lf);

    printf("  %s\n", tag);
    printf("    总空闲 = %5d B（%3d 段）   最大连续空闲 = %5d B（%3d 段）\n",
           tf * SEG_SIZE, tf, lf * SEG_SIZE, lf);

    if (show_map)
    {
        printf("    布局: ");
        for (int i = 0; i < NSEG; i++)
        {
            printf("%c", seg_used[i] ? '#' : '.');
        }
        printf("\n");
        printf("          (# = 已占用, . = 空闲，每字符 = 128 字节)\n");
    }
    printf("\n");
}

int main(void)
{
    printf("==========================================================\n");
    printf("  堆碎片化演示（%d 字节 = C8T6 的 SRAM，切成 %d 段 × %d B）\n",
           HEAP_SIZE, NSEG, SEG_SIZE);
    printf("==========================================================\n\n");

    /* ==========================================================
     * 场景 1：干净分配
     * ========================================================== */
    printf("========== 场景 1：从空堆里分配 4 块 × 1 KB ==========\n\n");
    heap_init();
    heap_dump("初始状态", 1);

    Alloc p[4];
    for (int i = 0; i < 4; i++)
    {
        int r = heap_alloc(1024, &p[i]);
        printf("    分配 1 KB → %s (起始段 %d, 占 %d 段)\n",
               r == 0 ? "成功" : "失败", p[i].start, p[i].nseg);
    }
    heap_dump("分配后", 1);

    /* ==========================================================
     * 场景 2：制造空洞
     * ========================================================== */
    printf("========== 场景 2：释放 p[1] 和 p[3]（中间挖出两个洞）==========\n\n");
    heap_free(&p[1]);
    heap_free(&p[3]);
    heap_dump("释放后", 1);

    printf("  ★ 两块 1 KB 的空洞出现了，但尾部还有 16 KB 连续空间，\n");
    printf("     所以现在申请大块还是能成功 —— 碎片化【还没致命】。\n\n");

    /* ==========================================================
     * 场景 3：把堆填满后再挖洞 —— 碎片化开始咬人
     * ========================================================== */
    printf("========== 场景 3：堆几乎填满，只在中间留下两个 1 KB 空洞 ==========\n\n");
    {
        heap_init();
        Alloc big[19];

        /* 先把堆塞满：19 块 × 1 KB = 152 段，只剩尾部 8 段（1 KB） */
        for (int i = 0; i < 19; i++) heap_alloc(1024, &big[i]);
        heap_dump("填满后", 1);

        /* 释放中间两块，制造两个 1 KB 空洞 */
        heap_free(&big[1]);
        heap_free(&big[3]);
        heap_dump("释放 big[1]、big[3] 后", 1);

        int tf, lf;
        heap_stats(&tf, &lf);
        printf("    当前总空闲 %d B，请求 1.5 KB = 需要 %d 段连续空间\n\n",
               tf * SEG_SIZE, (1536 + SEG_SIZE - 1) / SEG_SIZE);

        int r = heap_alloc(1536, NULL);
        printf("    分配 1.5 KB → %s\n\n", r == 0 ? "成功" : "【失败！】");

        if (r != 0)
        {
            printf("  ★★ 这就是碎片化的本质：\n");
            printf("       总空闲 %d B（3 KB）明明够 1.5 KB，\n", tf * SEG_SIZE);
            printf("       但最大连续块只有 %d B —— 装不下。\n", lf * SEG_SIZE);
            printf("       堆看起来还有很多空间，却分配不出来。\n\n");
        }
    }

    /* ==========================================================
     * 场景 4：长期运行的典型碎片模式（先全分配，再隔一个释放）
     * ========================================================== */
    printf("========== 场景 4：堆填满后「隔一个释放一个」（典型长期运行碎片）==========\n\n");
    {
        heap_init();
        Alloc keep[80];

        /* 200 B = 2 段，80 块正好占满 160 段 */
        for (int i = 0; i < 80; i++)
        {
            if (heap_alloc(200, &keep[i]) != 0)
            {
                printf("    第 %d 次分配失败（堆满）\n", i);
                break;
            }
        }
        heap_dump("全部占用", 0);

        /* 隔一个释放一个 → 40 个 2 段的空洞 */
        for (int i = 1; i < 80; i += 2) heap_free(&keep[i]);
        heap_dump("隔一个释放后", 1);

        int tf, lf;
        heap_stats(&tf, &lf);
        printf("  ★ 总空闲 %d B，最大连续块只有 %d B → 【碎片浪费 %.0f%%】\n",
               tf * SEG_SIZE, lf * SEG_SIZE,
               (1.0 - (double)lf / (double)(tf ? tf : 1)) * 100.0);
        printf("     现在连 512 B（4 段连续）都分配不出来了：\n\n");

        int r = heap_alloc(512, NULL);
        printf("    分配 512 B → %s\n\n", r == 0 ? "成功" : "【失败！】");
    }

    /* ==========================================================
     * 结论
     * ========================================================== */
    printf("==========================================================\n");
    printf("  结论：为什么嵌入式怕 malloc\n");
    printf("==========================================================\n\n");
    printf("  1. RAM 太小（C8T6 只有 20 KB），碎片化没有余量可浪费\n");
    printf("     → PC 有 8 GB，浪费几百 MB 无所谓；这里浪费 1 KB 就是 5%%\n\n");
    printf("  2. 堆会越用越碎：长期运行后「明明有空间却分配不出来」\n");
    printf("     → 设备跑几小时/几天后才出问题，是最难调的那类 bug\n\n");
    printf("  3. malloc 的执行时间【不确定】（要遍历找空闲块）\n");
    printf("     → 实时控制不能接受「这一行耗时忽长忽短」\n\n");
    printf("  4. malloc 会失败（返回 NULL），很多代码不检查\n");
    printf("     → 空指针解引用 → 硬件异常 → 死机，现场无法定位\n\n");
    printf("  替代方案：\n");
    printf("     - 静态分配：编译期确定大小（最安全，本项目都用这个）\n");
    printf("     - 固定块内存池：预先切好等大的块，分配/释放 O(1) 且无碎片\n");
    printf("     - 栈上分配：局部变量（默认栈只有 1 KB 左右，小心溢出）\n");

    return 0;
}
