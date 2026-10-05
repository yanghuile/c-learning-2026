/* ============================================================
 *  W5 指针回炉验收（函数指针 / 多级指针 / 指针运算）
 *
 *  为什么现在做：你之前选"用到时再补"，而明天开始写链表正是"用到的时候"。
 *                链表的增删查改全靠指针，基础不牢时写错看不出错在哪。
 *
 *  怎么用：
 *    1. 先【全部写完答案】，不要边写边运行
 *    2. 写完运行 `b exercises/W5-指针回炉验收.c`，对照输出
 *    3. 把结果发我
 *
 *  不会的就写「不会」—— 那比瞎猜有价值。
 *  本文件保持零警告通过，可放心用 chk 检查。
 * ============================================================ */

#include <stdio.h>

/* 函数声明（先声明后使用，你上周踩过这个坑） */
static int  add(int a, int b);
static int  sub(int a, int b);
static int  mul(int a, int b);
static void modify_pointer(int **pp, int *target);
static void swap_int(int *a, int *b);

/* 链表节点（最后一题用） */
typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

static Node g_node1 = { 10, NULL };
static Node g_node2 = { 20, NULL };

int main(void)
{
    /* ==========================================================
     * 第一部分：函数指针
     * ========================================================== */

    printf("========== 第一部分：函数指针 ==========\n");

    /* ---- 1-1 声明与调用 ---- */
    int (*p)(int, int) = add;        /* p 是指向"接收两个 int、返回 int"的函数 */

    printf("p(3, 4)        = %d\n", p(3, 4));        /* 用指针调用 */
    printf("(*p)(3, 4)     = %d\n", (*p)(3, 4));     /* 先解引用再调用 */

    p = mul;                          /* 换一个函数 */
    printf("换 mul 后 p(3,4) = %d\n\n", p(3, 4));

    /* 我的答案（先预测再跑）：
     *   p(3,4) = 7          (*p)(3,4) = ? （*p）解引用得到函数的值？？？不会
     *   换 mul 后 = 12
     *   1-1a  `int (*p)(int, int)` 和 `int *p(int, int)` 有什么区别？
     *   `int (*p)(int, int)`是一个函数指针，本质是一个指针，指向一个函数，这个函数接收两个int,返回一个int参数； `int *p(int, int)`是一个指针函数，本质是一个函数，接收两个int参数，返回int *指针
     */


    /* ---- 1-2 函数指针数组 ---- */
    int (*ops[3])(int, int) = { add, sub, mul };

    printf("ops[0](10,3) = %d   (add)\n", ops[0](10, 3));
    printf("ops[1](10,3) = %d   (sub)\n", ops[1](10, 3));
    printf("ops[2](10,3) = %d   (mul)\n\n", ops[2](10, 3));

    /* 我的答案：
     *   ops[0](10,3) = 13   ops[1](10,3) = 7   ops[2](10,3) = 30
     *   1-2a  为什么不写成 `int *ops[3](int, int)` ？
     *   []与()的优先级都高于*，如果写成`int *ops[3](int, int)`，ops[3]就和后面的(int ,int)结合，返回int *，变成一个指针函数。而数组元素不能是函数，语法错误。
     */


    /* ==========================================================
     * 第二部分：多级指针
     * ========================================================== */

    printf("========== 第二部分：多级指针 ==========\n");

    /* ---- 2-1 基础 ---- */
    int   a  = 10;
    int  *p1 = &a;          /* 一级指针 */
    int **p2 = &p1;         /* 二级指针：指向"指针"的指针 */

    printf(" a     = %d\n", a);
    printf("*p1    = %d\n", *p1);
    printf("**p2   = %d\n\n", **p2);

    **p2 = 99;              /* 通过二级指针改 a */

    printf("把 **p2 改成 99 之后：\n");
    printf(" a     = %d\n", a);
    printf("*p1    = %d\n", *p1);
    printf("**p2   = %d\n\n", **p2);

    /* 我的答案（先预测再跑）：
     *   第一组三个 printf 输出：10 10 10
     *   改完之后三个 printf 输出：99 99 99
     *   2-1a  `p2` 的类型是什么？`*p2` 的类型是什么？
         p2是二级指针，指向int*类型的指针变量；*p2对二级指针做解引用，等价于p1。
    */  


    /* ---- 2-2 二级指针的用途：修改"指针本身" ---- */
    int  x = 100;
    int  y = 200;
    int *ptr = &x;                 /* ptr 现在指向 x */

    printf("修改前: *ptr = %d\n", *ptr);

    modify_pointer(&ptr, &y);      /* 传 ptr 的地址，让函数能改 ptr 本身 */


    printf("修改后: *ptr = %d\n\n", *ptr);

    /* 我的答案：
     *   修改前 *ptr = 100     修改后 *ptr = 200
     *   2-2a  如果 modify_pointer 的参数是 `int *pp`（一级指针），
     *         能改掉 main 里的 ptr 吗？为什么？
     * 不能。函数参数是值传递，若形参是int *pp,调用时只是把ptr的值（x的地址）复制给局部变量pp;函数内仅修改局部副本，main里的指针变量ptr不受影响。想要修改main中的指针变量本身，必须传入ptr的地址，也就是使用二级指针int **pp
     */


    /* ---- 2-3 【链表场景】用二级指针改 head ---- */
    Node *head = &g_node1;         /* head 指向第一个节点 */
    g_node1.next = &g_node2;

    printf("========== 2-3 链表场景 ==========\n");
    printf("初始 head->data = %d\n", head->data);

    /* 模拟"删除第一个节点"：把 head 改成指向第二个节点 */
    Node **pp = &head;             /* pp 是"指向 head 的指针" */
    *pp = (*pp)->next;             /* 通过 pp 改 head 本身 */

    printf("删除第一个后 head->data = %d\n\n", head->data);

    /* 我的答案：
     *   初始 head->data = 10    删除后 head->data = 20
     *   2-3a  为什么链表删除第一个节点时，函数参数要写成 `Node **head`？
     *         （提示：写成 `Node *head` 会怎样？）  哪里写了`Node **head`？我没懂
     */


    /* ==========================================================
     * 第三部分：指针运算
     * ========================================================== */

    printf("========== 第三部分：指针运算 ==========\n");

    /* ---- 3-1 sizeof 数组 vs 指针 ---- */
    int arr[5] = { 1, 2, 3, 4, 5 };
    int *pa = arr;

    printf("sizeof(arr) = %d    (数组)\n", (int)sizeof(arr));
    printf("sizeof(pa)  = %d    (指针)\n\n", (int)sizeof(pa));

    /* 我的答案：
     *   sizeof(arr) = 20      sizeof(pa) = 8
     */


    /* ---- 3-2 指针加减的单位 ---- */
    printf("arr     = %p\n", (void *)arr);
    printf("arr + 1 = %p\n", (void *)(arr + 1));
    printf("字节差  = %d\n", (int)((char *)(arr + 1) - (char *)arr));
    printf("*(arr+2) = %d\n\n", *(arr + 2));

    /* 我的答案：
     *   字节差 = 4      *(arr+2) = 3
     *   3-2a  arr+1 走多少字节？如果是 double 数组呢？
     *  走int个字节，也就是4个字节；如果是double，那就走8个字节
     */


    /* ---- 3-3 指针相减（求元素个数） ---- */
    int *pstart = arr;
    int *pend   = arr + 5;

    printf("pend - pstart = %d    (这是元素个数，不是字节数)\n\n", (int)(pend - pstart));

    /* 我的答案：pend - pstart = 5
     */


    /* ---- 3-4 交换两个变量（指针参数） ---- */
    int m = 1;
    int n = 2;

    swap_int(&m, &n);
    printf("swap 后: m=%d, n=%d\n\n", m, n);

    /* 我的答案：m = 2   n = 1
     */


    /* ==========================================================
     * 第四部分：找 bug
     *
     *  下面这段代码想"释放链表第一个节点"，有什么问题？
     *
     *      void delete_first(Node *head)
     *      {
     *          if (head == NULL) return;
     *          Node *temp = head;
     *          head = head->next;      // 想让 head 指向下一个
     *          free(temp);
     *      }
     *
     *      int main(void)
     *      {
     *          Node *head = ...;       // 已建好的链表
     *          delete_first(head);
     *          // 这里 head 还是指向被释放的节点！
     *      }
     * ========================================================== */

    /* 我的答案：
     *   问题：head的指向没有变，还是指向原来的节点
     *   为什么：函数参数是值传递，传入head仅修改函数里的head,并不能改变main里的head
     *   怎么改：传入二级指针void delete_first(Node **head)
     */


    printf("========== 结束 ==========\n");
    return 0;
}


/* ==========================================================
 *  下面是已给出的函数实现（不用你写）
 * ========================================================== */

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }
static int mul(int a, int b) { return a * b; }

static void swap_int(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* 通过二级指针修改调用方的指针 */
static void modify_pointer(int **pp, int *target)
{
    *pp = target;
}
