/* ============================================================
 *  W5 struct 阶梯练习（从零开始，一步一步来）
 *
 *  设计思路：每一级都在上一级基础上加一点点。
 *            不要跳级，写完一级运行一次，看结果对了再往下。
 *
 *  用法：
 *    每写完一级，跑一次：
 *      b notes\W5-struct练习.c
 *    看输出对不对，对了再做下一级。
 *
 *  注意：本文件保持零警告通过，可放心用 chk 检查。
 * ============================================================ */

#include <stdio.h>
#include <string.h>     /* strcpy */
#include <stddef.h>     /* offsetof */

/* ============================================================
 *  第 1 级：最简单的结构体
 * ============================================================ */

/* 结构体的作用：把"描述同一个东西的几个数据"打包在一起。
 *
 * 比如一个学生有姓名、年龄、成绩 —— 三个数据本来要三个变量：
 *     char name[20];
 *     int  age;
 *     float score;
 * 用结构体可以打包成一个"学生"类型。
 */

/* 定义结构体类型（注意末尾的分号不能少） */
struct Student
{
    char  name[20];
    int   age;
    float score;
};

/* ============================================================
 *  第 2 级：用 typedef 起别名
 * ============================================================ */

/* 上面定义完，声明变量要写 struct Student s1; —— 每次都要写 struct，很烦。
 * typedef 就是给这个类型起个短名字：
 */

typedef struct Student Student;     /* 以后可以只写 Student s1; */

/* ============================================================
 *  第 3 级：一步到位写法（嵌入式最常用）
 * ============================================================ */

/* 定义的同时起别名，省掉中间的 struct Student 名字：
 *
 *     typedef struct
 *     {
 *         char  name[20];
 *         int   age;
 *         float score;
 *     } Student2;
 *
 * 这样定义出来的类型没有"结构体标记名"，只能用 Student2。
 */

typedef struct
{
    char   id[8];       /* 工号 */
    int    level;       /* 等级 */
    double salary;      /* 薪水 */
} Employee;

/* ============================================================
 *  第 4 级：enum 枚举
 * ============================================================ */

/* enum 的作用：给一组整数值起名字。
 * 不写值时默认从 0 开始递增。
 */
typedef enum
{
    STATE_IDLE = 0,     /* 空闲 */
    STATE_RUN,          /* 运行 */
    STATE_ERROR,        /* 错误 */
    STATE_STOP          /* 停止 */
} State;

/* ============================================================
 *  main：逐级验证
 * ============================================================ */

int main(void)
{
    /* ---------- 第 1 级：声明变量 + 用 . 访问成员 ---------- */
    printf("========== 第 1 级：基本结构体 ==========\n");

    struct Student s1;

    /* 给成员赋值：用 变量名.成员名 */
    strcpy(s1.name, "ZhangSan");    /* 字符数组用 strcpy，不能直接 = */
    s1.age   = 20;
    s1.score = 87.5f;

    printf("姓名: %s\n", s1.name);
    printf("年龄: %d\n", s1.age);
    printf("成绩: %.1f\n", s1.score);
    printf("\n");

    /* ---------- 第 2 级：typedef 之后的写法 ---------- */
    printf("========== 第 2 级：typedef 别名 ==========\n");

    Student s2;                     /* 注意：不用写 struct 了 */
    strcpy(s2.name, "LiSi");
    s2.age   = 21;
    s2.score = 92.0f;
    printf("姓名: %s, 年龄: %d, 成绩: %.1f\n", s2.name, s2.age, s2.score);
    printf("\n");

    /* ---------- 第 3 级：指针访问（重点！） ---------- */
    printf("========== 第 3 级：指针与 -> ==========\n");

    /* 定义一个指向结构体的指针 */
    Student   *p  = &s2;            /* p 指向 s2 */
    Employee   e;
    Employee  *pe = &e;

    /* 两种访问方式，效果完全一样： */
    printf("用 . 访问:  s2.name  = %s\n", s2.name);
    printf("用 -> 访问: p->name  = %s\n", p->name);

    /* 关键理解：
     *   s2.name    —— s2 是【变量】，用 .
     *   p->name    —— p 是【指针】，用 ->
     *   (*p).name  —— 等价于 p->name（先解引用，再用 .）
     */
    printf("用 (*p).name: %s   （和 p->name 完全等价）\n", (*p).name);

    /* 通过指针修改成员 */
    p->age = 22;                    /* 相当于 s2.age = 22 */
    printf("通过指针改了年龄后: s2.age = %d\n", s2.age);
    printf("\n");

    /* 初始化 Employee（用 {} 按顺序赋值） */
    strcpy(e.id, "E001");
    e.level  = 3;
    e.salary = 12345.67;
    printf("工号: %s, 等级: %d, 薪水: %.2f\n", pe->id, pe->level, pe->salary);
    printf("\n");

    /* ---------- 第 4 级：enum ---------- */
    printf("========== 第 4 级：enum ==========\n");

    State st = STATE_RUN;
    printf("STATE_IDLE  = %d\n", STATE_IDLE);
    printf("STATE_RUN   = %d\n", STATE_RUN);
    printf("STATE_ERROR = %d\n", STATE_ERROR);
    printf("STATE_STOP  = %d\n", STATE_STOP);
    printf("当前状态 st = %d\n", st);

    /* enum 最常见的用法：switch 判断 */
    switch (st)
    {
    case STATE_IDLE:  printf("状态：空闲\n"); break;
    case STATE_RUN:   printf("状态：运行中\n"); break;
    case STATE_ERROR: printf("状态：错误\n");  break;
    case STATE_STOP:  printf("状态：已停止\n"); break;
    default:          printf("状态：未知\n"); break;
    }
    printf("\n");

    /* ---------- 第 5 级：sizeof 与内存布局 ---------- */
    printf("========== 第 5 级：sizeof 与偏移 ==========\n");

    printf("sizeof(Student)  = %d\n", (int)sizeof(Student));
    printf("  成员之和 = 20 + 4 + 4 = 28，刚好相等（没有 padding）\n");

    printf("sizeof(Employee) = %d\n", (int)sizeof(Employee));
    printf("  成员之和 = 8 + 4 + 8 = 20，但实际是 24 —— 多了 4 字节 padding！\n");
    printf("  为什么？看下面的偏移量就明白了\n");

    /* offsetof：求某个成员距离结构体开头的字节数 */
    printf("\nStudent 各成员偏移:\n");
    printf("  name  = %d\n", (int)offsetof(Student, name));
    printf("  age   = %d\n", (int)offsetof(Student, age));
    printf("  score = %d\n", (int)offsetof(Student, score));

    printf("\nEmployee 各成员偏移:\n");
    printf("  id     = %d\n", (int)offsetof(Employee, id));
    printf("  level  = %d\n", (int)offsetof(Employee, level));
    printf("  salary = %d   <-- 注意：不是 12，而是 16！\n", (int)offsetof(Employee, salary));
    printf("\n  解释：\n");
    printf("    id     占 0~7    （8 字节 char 数组）\n");
    printf("    level  占 8~11   （4 字节 int）\n");
    printf("    [空洞] 占 12~15  （4 字节 padding，什么都没放）\n");
    printf("    salary 占 16~23  （8 字节 double，必须从 8 的倍数地址开始）\n");
    printf("    所以 sizeof = 24 而不是 20\n");

    printf("\n========== 完成 ==========\n");
    return 0;
}
