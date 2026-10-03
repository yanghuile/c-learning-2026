/* ============================================================
 *  W5 位运算验收题（第 1 组）
 *
 *  怎么用：
 *    1. 先【全部写完答案】，不要边写边运行
 *    2. 写完运行 `b notes/W5-位运算验收.c`，对照输出
 *    3. 把结果发我，我判断哪里理解有偏差
 *
 *  说明：这是验收题，不是教学题 —— 我不会在文件里给提示。
 *        不会的就写「不会」，那也是有价值的信息。
 *
 *  注意：本文件保持零警告通过（连 (void) 抑制都做了），
 *        所以你可以放心用 chk 检查它。
 * ============================================================ */

#include <stdio.h>

/* 先声明，再使用 —— 上周你就是因为漏了声明导致链接失败 */
static void print_binary8(unsigned char v);

unsigned char set_bit(unsigned char value, int n);
unsigned char clear_bit(unsigned char value, int n);
int           get_bit(unsigned char value, int n);
unsigned char toggle_bit(unsigned char value, int n);
unsigned char get_bits(unsigned char value, int hi, int lo);

int main(void)
{
    /* ==========================================================
     * 第一部分：概念辨析（写出答案 + 一句话理由）
     * ========================================================== */

    /* 1-1  `~` 和 `!` 有什么区别？对 a=0x0F 运算，结果分别是多少？
     *
     * 注意：下面用 volatile 只是为了不让编译器把结果优化掉，
     *       你暂时不用管 volatile 是什么（W5 周四会讲）。
     */
    volatile unsigned char va = 0x0F;
    unsigned char a = va;

    printf("========== 第一部分：概念辨析 ==========\n");
    printf("a    = 0x%02X  (%d)\n", a, a);
    printf("~a   = 0x%02X  (%d)\n", (unsigned char)~a, (unsigned char)~a);
    printf("!a   = 0x%02X  (%d)\n\n", (unsigned char)(!a), !a);
    /* 我的答案：
     *   ~a =0xF0
     *   !a =0x00
     *   区别：~a是按位取反，每一位二进制数全部取反；！a是逻辑非，非零数都认为是1，！a=0
     */


    /* 1-2  `&` 和 `&&` 有什么区别？(5 & 3) 和 (5 && 3) 分别是多少？
     */
    printf("5 & 3  = %d\n", 5 & 3);
    printf("5 && 3 = %d\n\n", 5 && 3);
    /* 我的答案：
     *   5 & 3  =1或0x01
     *   5 && 3 =1
     *   区别：&是按位与，1&1=1，其余都是0；&&是逻辑与，如果左边为0，右边不执行
     */


    /* ==========================================================
     * 第二部分：运算结果预测（先算再跑）
     * ========================================================== */

    /* 2-1  有符号数的右移
     */
    int neg = -8;
    printf("========== 第二部分：结果预测 ==========\n");
    printf("-8 >> 1   = %d\n", neg >> 1);
    /* 我的答案：
     *   值 =-4
     *   这是算术右移还是逻辑右移？ 算数右移
     */

    /* 2-2  无符号数的右移（和 2-1 对比）
     */
    unsigned int uneg = 0xFFFFFFF8u;
    printf("0xFFFFFFF8 >> 1 = 0x%X\n", uneg >> 1);
    /* 我的答案：值 =0xFFFFFFFC
     */

    /* 2-3  位运算不会"进位"，这一点和加法完全不同
     */
    printf("0x0F + 0x01 = 0x%02X\n", 0x0F + 0x01);
    printf("0x0F | 0x01 = 0x%02X\n\n", 0x0F | 0x01);
    /* 我的答案：
     *   0x0F + 0x01 = 0x10
     *   0x0F | 0x01 = 0x0F
     *   为什么不同：+是普通整数加法，低位加满会向高位进位；|是按位或运算，对应二进制单独计算，不会产生进位传递
     */


    /* ==========================================================
     * 第三部分：动手写代码（重点）
     *
     *  下面 5 个函数要你实现（在文件末尾的空函数体里写）。
     *  写完运行，对照每行后面的「期望」值。
     * ========================================================== */

    printf("========== 第三部分：动手实现 ==========\n");

    /* 3-1  置位：把 value 的第 n 位设为 1，其余位不变
     *      例：set_bit(0x00, 3) 应得到 0x08
     */
    printf("set_bit(0x00, 3)   = 0x%02X   （期望 0x08）\n", set_bit(0x00, 3));
    printf("set_bit(0xF0, 0)   = 0x%02X   （期望 0xF1）\n", set_bit(0xF0, 0));

    /* 3-2  清位：把 value 的第 n 位设为 0，其余位不变
     *      例：clear_bit(0xFF, 3) 应得到 0xF7
     */
    printf("clear_bit(0xFF,3)  = 0x%02X   （期望 0xF7）\n", clear_bit(0xFF, 3));
    printf("clear_bit(0x0F,0)  = 0x%02X   （期望 0x0E）\n", clear_bit(0x0F, 0));

    /* 3-3  读位：返回 value 的第 n 位（0 或 1）
     */
    printf("get_bit(0x08,3)    = %d     （期望 1）\n", get_bit(0x08, 3));
    printf("get_bit(0x08,2)    = %d     （期望 0）\n", get_bit(0x08, 2));

    /* 3-4  翻转：把 value 的第 n 位取反
     */
    printf("toggle_bit(0xFF,3) = 0x%02X   （期望 0xF7）\n", toggle_bit(0xFF, 3));
    printf("toggle_bit(0x00,3) = 0x%02X   （期望 0x08）\n", toggle_bit(0x00, 3));

    /* 3-5  掩码取值：取出 value 的第 hi 位到第 lo 位（含两端）
     *      例：get_bits(0xAB, 3, 0) 应得到 0x0B（低 4 位）
     *          get_bits(0xAB, 7, 4) 应得到 0x0A（高 4 位）
     *
     *      提示：这题比前 4 个难，需要"造掩码 + 移位"
     */
    printf("get_bits(0xAB,3,0) = 0x%02X   （期望 0x0B）\n", get_bits(0xAB, 3, 0));
    printf("get_bits(0xAB,7,4) = 0x%02X   （期望 0x0A）\n\n", get_bits(0xAB, 7, 4));


    /* ==========================================================
     * 第四部分：实战场景（模拟 STM32 寄存器操作）
     *
     *  8 位寄存器 REG 初始为 0，按顺序操作，写出每步之后的值
     * ========================================================== */
    unsigned char REG = 0x00;

    printf("========== 第四部分：寄存器操作 ==========\n");
    printf("初始:            "); print_binary8(REG);

    REG |= (1 << 2);
    printf("① 置位第 2 位:   "); print_binary8(REG);
    /* 我的答案：0000 0100
     */

    REG |= (1 << 5);
    printf("② 置位第 5 位:   "); print_binary8(REG);
    /* 我的答案：0010 0100
     */

    REG &= ~(1 << 2);
    printf("③ 清掉第 2 位:   "); print_binary8(REG);
    /* 我的答案：0010 0000
     */

    REG ^= (1 << 5);
    printf("④ 翻转第 5 位:   "); print_binary8(REG);
    /* 我的答案：0000 0000
     */

    REG = (unsigned char)((REG & 0xF0) | 0x03);
    printf("⑤ 低4位设为0011: "); print_binary8(REG);
    /* 我的答案：0000 0011
     */


    /* ==========================================================
     * 第五部分：找 bug
     *
     *  下面三段代码都想"清掉第 3 位"，哪段对、哪段错？
     *  错的错在哪？会导致什么后果？
     *
     *  代码 A：  REG = REG & (1 << 3);
     *  代码 B：  REG &= ~(1 << 3);
     *  代码 C：  REG = REG | (1 << 3);
     * ========================================================== */
    /* 我的答案：
     *   A:第三位被保留，其他位全为0了
     *   B:正确
     *   C:把第三位置1了
     */


    printf("\n========== 结束 ==========\n");
    return 0;
}

/* 打印一个字节的二进制形式（已写好，方便肉眼对照） */
static void print_binary8(unsigned char v)
{
    for (int i = 7; i >= 0; i--)
    {
        putchar((v & (1 << i)) ? '1' : '0');
        if (i == 4) putchar('_');
    }
    putchar('\n');
}


/* ==========================================================
 *  在下面实现这 5 个函数
 * ========================================================== */

unsigned char set_bit(unsigned char value, int n)
{
    return value|(1<<n);
}

unsigned char clear_bit(unsigned char value, int n)
{
    return value&~(1<<n);
}

int get_bit(unsigned char value, int n)
{
    if((value&(1<<n))!=0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char toggle_bit(unsigned char value, int n)
{
    return value^(1<<n);
}

unsigned char get_bits(unsigned char value, int hi, int lo)
{
    int width =hi-lo+1;
    unsigned char mask=(unsigned char)(((1<<width)-1)<<lo);
    return ((mask&value)>>lo);
}
