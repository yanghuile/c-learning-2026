/* ============================================================
 *  W5 struct / typedef / enum 验收题
 *
 *  怎么用：
 *    1. 先【全部写完答案】（包括第三部分的函数实现），不要边写边运行
 *    2. 写完运行 `b notes/W5-struct验收.c`
 *    3. 把结果发我
 *
 *  重点考：内存对齐 padding、sizeof、嵌入式里的实际用法
 *  不会的就写「不会」，那也是有价值的信息。
 *
 *  本文件保持零警告通过，可放心用 chk 检查。
 *
 *  注意：结构体和 enum 的类型定义我替你写好了（否则 main 里用不了），
 *        但【全部函数实现留给你】，这才是要练的部分。
 * ============================================================ */

#include <stdio.h>
#include <stddef.h>     /* offsetof */
#include <string.h>     /* memset */

/* ============================================================
 *  类型定义（已给出，你要看懂）
 * ============================================================ */

/* 3-1 用到的结构体 */
typedef struct
{
    char  name[20];     /* 姓名，20 字节 */
    int   age;          /* 年龄 */
    float score;        /* 成绩 */
} Student;

/* 3-2 用到的错误码 */
typedef enum
{
    ERR_OK = 0,         /* 成功 */
    ERR_PARAM,          /* 参数错误 */
    ERR_TIMEOUT,        /* 超时 */
    ERR_BUSY            /* 忙 */
} ErrCode;

/* ============================================================
 *  函数声明（实现在文件末尾，由你完成）
 * ============================================================ */

void         make_student(Student *s, const char *name, int age, float score);
const char  *err_desc(ErrCode code);
unsigned int build_frame(unsigned char *buf, unsigned char id, const char *payload);

/* ============================================================
 *  main
 * ============================================================ */

int main(void)
{
    /* ==========================================================
     * 第一部分：概念题（写答案 + 理由，这部分不用跑代码）
     * ========================================================== */

    /* 1-1  `p.name` 和 `p->name` 分别用在什么场景？
     */
    /* `p.name`是变量p用.引出name成员；`p->name`是指针p用->引出成员
     */

    /* 1-2  `typedef struct { ... } Student;` 里的 typedef 起什么作用？
     *      如果去掉 typedef，这个结构体该怎么声明变量？
     */
    /* typedef是为了给这个结构体起一个别名。
        struct Student s1;
     */

    /* 1-3  enum 和 #define 都能定义常量，用 enum 的好处是什么？（至少两条）
     */
    /* enum可以一次性定义一组相关常量；枚举常量是有类型的，编译器可以做类型检查
     */


    /* ==========================================================
     * 第二部分：sizeof 与内存对齐（重点：先预测，再跑）
     * ========================================================== */

    printf("========== 第二部分：内存对齐 ==========\n");

    /* 这三个结构体成员完全一样，只是顺序不同 */
    struct A { char a; int  b; char c; };
    struct B { int  a; char b; char c; };
    struct C { char a; char b; int  c; };

    printf("sizeof(struct A) = %d\n", (int)sizeof(struct A));
    printf("sizeof(struct B) = %d\n", (int)sizeof(struct B));
    printf("sizeof(struct C) = %d\n", (int)sizeof(struct C));
    printf("\n");
    /* 我的答案（先预测，再跑）：
     *   成员之和是 1 + 4 + 1 = 6 字节
     *   struct A = 4+4+4=12
     *   struct B = 4+1+3=8
     *   struct C = 1+3+4=8
     *   为什么三者不同：每个成员的起始偏移，必须是该成员对齐数的整数倍，不满足就前面补填充字节padding；结构体整体的总大小，必须是最大对齐数的整数倍，末尾不够就尾部补padding
     */

    /* 2-4  用 offsetof 验证你的预测 */
    printf("--- struct A 的成员偏移 ---\n");
    printf("  offsetof(A, a) = %d\n", (int)offsetof(struct A, a));
    printf("  offsetof(A, b) = %d\n", (int)offsetof(struct A, b));
    printf("  offsetof(A, c) = %d\n", (int)offsetof(struct A, c));
    printf("\n");

    printf("--- struct C 的成员偏移 ---\n");
    printf("  offsetof(C, a) = %d\n", (int)offsetof(struct C, a));
    printf("  offsetof(C, b) = %d\n", (int)offsetof(struct C, b));
    printf("  offsetof(C, c) = %d\n", (int)offsetof(struct C, c));
    printf("\n");
    /* 我的答案（预测各偏移量）：
     *   A: a=0  b=4  c=8
     *   C: a=0  b=1  c=4
     */

    /* 2-5  内存对齐的两个规则是什么？（用你自己的话说）
     */
    /* 我的答案：
     *   规则1：每个成员的起始地址是自己大小的整数倍
     *   规则2：整个结构体必须是最大成员的整数倍 
     */


    /* ==========================================================
     * 第三部分：动手实现（在文件末尾写函数体）
     * ========================================================== */

    printf("========== 第三部分：动手实现 ==========\n");

    /* 3-1  调用 make_student() 填数据，然后用【指针 + ->】打印
     */
    Student s1;
    make_student(&s1, "ZhangSan", 20, 87.5f);
    printf("学生: %s, 年龄: %d, 成绩: %.1f\n", s1.name, s1.age, s1.score);
    printf("sizeof(Student) = %d\n\n", (int)sizeof(Student));

    /* 3-2  实现 err_desc()，返回对应中文描述
     */
    printf("err_desc(ERR_OK)      = %s\n", err_desc(ERR_OK));
    printf("err_desc(ERR_PARAM)   = %s\n", err_desc(ERR_PARAM));
    printf("err_desc(ERR_TIMEOUT) = %s\n", err_desc(ERR_TIMEOUT));
    printf("err_desc(ERR_BUSY)    = %s\n", err_desc(ERR_BUSY));
    printf("ERR_BUSY 的值 = %d\n\n", ERR_BUSY);

    /* 3-3  通信协议帧打包
     *
     *  帧格式： [0]=帧头 0xAA  [1]=长度  [2]=ID  [3..]=数据  [末位]=校验和
     *  长度字段 = 从第 2 字节(ID)到数据末尾的字节数
     *  校验和   = 从第 1 字节(长度)到数据末尾所有字节之和 & 0xFF
     *
     *  例：build_frame(buf, 0x10, "AB") 应产生
     *       AA 03 10 41 42 96
     *       │  │  │  └──┴── "AB" 的 ASCII
     *       │  │  └── ID
     *       │  └── 长度 = ID(1) + 数据(2) = 3
     *       └── 帧头
     *       校验和 = (0x03 + 0x10 + 0x41 + 0x42) & 0xFF = 0x96
     *
     *  要求：返回【整帧总长度】
     */
    unsigned char frame[64];
    memset(frame, 0, sizeof(frame));
    unsigned int len = build_frame(frame, 0x10, "AB");

    printf("帧长度 = %u\n", len);
    printf("帧内容 = ");
    for (unsigned int i = 0; i < len; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("\n\n");


    /* ==========================================================
     * 第四部分：嵌入式实战（解释代码，不用运行）
     *
     *  下面是从 STM32 头文件里摘出来的真实写法：
     *
     *      typedef struct
     *      {
     *          volatile uint32_t MODER;
     *          volatile uint32_t OTYPER;
     *          volatile uint32_t ODR;
     *          volatile uint32_t IDR;
     *      } GPIO_TypeDef;
     *
     *  然后这样用：
     *      #define GPIOA  ((GPIO_TypeDef *)0x40020000UL)
     *      GPIOA->ODR |= (1 << 5);
     * ========================================================== */

    /* 4-1  为什么用 struct 描述寄存器，而不是定义 4 个单独的变量？
     */
    /* 我的答案：便于管理一组相关寄存器
     */

    /* 4-2  `((GPIO_TypeDef *)0x40020000UL)` 这个强转在做什么？
     */
    /* 我的答案：强转为GPIO_TypeDef类型的结构体指针
     */

    /* 4-3  `GPIOA->ODR |= (1 << 5);` 这行的完整含义是什么？（拆开说）
     */
    /* 我的答案：把GPIOA的ODR第五位置1
     */


    /* ==========================================================
     * 第五部分：找 bug
     *
     *  下面这段代码想"把学生成绩加 5 分"，有什么问题？
     *
     *      void add_score(Student s)
     *      {
     *          s.score += 5.0f;
     *      }
     *
     *      int main(void)
     *      {
     *          Student s1 = { "LiSi", 21, 80.0f };
     *          add_score(s1);
     *          printf("%.1f\n", s1.score);    // 期望 85.0
     *      }
     * ========================================================== */

    /* 我的答案：
     *   问题：学生成绩不会增加
     *   为什么：函数参数是结构体值传递，形参s是实参s1的拷贝。函数只能修改副本里的score，main中的s1没有被修改
     *   怎么改：void add_score(Student *s)
                {
                    s->score += 5.0f;
                }
                //main里面调用
                add_score(&s1);
     */


    printf("========== 结束 ==========\n");
    return 0;
}


/* ==========================================================
 *  下面三个函数由你实现
 * ========================================================== */

/* 3-1  把 name / age / score 填进 s 指向的结构体
 *      注意：name 是 char 数组，要用 strcpy 或逐字符拷贝
 *            s 是指针，访问成员要用 ->
 */
void make_student(Student *s, const char *name, int age, float score)
{
    strcpy(s->name, name);
    s->age = age;
    s->score = score;
}


/* 3-2  返回错误码对应的中文描述
 *      用 switch 或 if-else 都行
 */
const char *err_desc(ErrCode code)
{
    switch(code)
    {
        case ERR_OK:        return "成功";
        case ERR_PARAM:     return "参数错误";
        case ERR_TIMEOUT:   return "超时";
        case ERR_BUSY:      return "忙";
        default:            return "未知";
    }
}


/* 3-3  打包一帧，返回整帧总长度
 *      提示：
 *        - 用 strlen 求 payload 长度
 *        - 长度字段 = 1(ID) + payload 长度
 *        - 校验和从"长度字节"累加到"数据末字节"
 */
unsigned int build_frame(unsigned char *buf, unsigned char id, const char *payload)
{
    /* 第 1 步：求 payload 长度 */
    unsigned int plen = (unsigned int)strlen(payload);      /* "AB" → 2 */

    /* 第 2 步：填前三个字节 */
    buf[0] = 0xAA;                                          /* 帧头 */
    buf[1] = (unsigned char)(1 + plen);                     /* 长度 = ID(1) + 数据(2) = 3 */
    buf[2] = id;                                            /* ID */

    /* 第 3 步：把 payload 拷进去 */
    for (unsigned int i = 0; i < plen; i++)
    {
        buf[3 + i] = (unsigned char)payload[i];             /* 'A' → buf[3], 'B' → buf[4] */
    }

    /* 第 4 步：算校验和（从 buf[1] 累加到 buf[3+plen-1]） */
    unsigned char sum = 0;
    for (unsigned int i = 1; i < 3 + plen; i++)             /* 注意：不包含 3+plen */
    {
        sum = (unsigned char)(sum + buf[i]);
    }
    buf[3 + plen] = sum;                                    /* 校验和放在数据后面 */

    /* 第 5 步：返回整帧总长度 */
    return 4 + plen;                                        /* 帧头1 + 长度1 + ID1 + 数据plen + 校验和1 */
}
