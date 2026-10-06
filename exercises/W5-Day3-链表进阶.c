/* ============================================================
 *  W5-Day3 链表进阶操作
 *
 *  今天要实现 6 个函数（见下方 TODO）。
 *  约定：单链表，【不带头节点】
 *
 *  写完后运行：
 *      b exercises/W5-Day3-链表进阶.c
 *  看输出是否和每处的「期望」一致。
 *
 *  ⚠️ 今天大量考边界情况。写每个函数前先问：
 *        空链表？只有一个节点？删/插的是头？是尾？位置越界？
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

/* ---------- 函数声明 ---------- */
Node *create_node(int data);
void  insert_tail(Node **head, int data);
void  print_list(Node *head);
void  free_list(Node **head);

int   list_length(Node *head);
int   delete_value(Node **head, int value);
int   insert_at(Node **head, int pos, int value);
Node *find_middle(Node *head);
int   has_cycle(Node *head);
Node *merge_sorted(Node *a, Node *b);

/* 下面两个是昨天写过的基础函数（直接给你，不用重写） */
Node *create_node(int data)
{
    Node *p = (Node *)malloc(sizeof(Node));
    if (p == NULL) { printf("malloc failed\n"); return NULL; }
    p->data = data;
    p->next = NULL;
    return p;
}

void insert_tail(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) return;
    if (*head == NULL) { *head = new_node; return; }
    Node *p = *head;
    while (p->next != NULL) p = p->next;
    p->next = new_node;
}

void print_list(Node *head)
{
    if (head == NULL) { printf("(空链表)\n"); return; }
    for (Node *p = head; p != NULL; p = p->next)
    {
        printf("%d", p->data);
        if (p->next != NULL) printf(" -> ");
    }
    printf(" -> NULL\n");
}

void free_list(Node **head)
{
    Node *p = *head;
    while (p != NULL)
    {
        Node *next_node = p->next;
        free(p);
        p = next_node;
    }
    *head = NULL;
}

/* 建链表的小工具（测试用）：从数组建一条链表 */
static Node *build(int *arr, int n)
{
    Node *head = NULL;
    for (int i = 0; i < n; i++) insert_tail(&head, arr[i]);
    return head;
}

/* ============================================================
 *  main：测试代码（已写好，不用改）
 * ============================================================ */
int main(void)
{
    /* ---------- 测试 1：求长度 ---------- */
    printf("========== 测试 1：求长度 ==========\n");
    int a1[] = {10, 20, 30};
    Node *h1 = build(a1, 3);
    printf("链表 ");
    print_list(h1);
    printf("长度 = %d   （期望 3）\n", list_length(h1));
    printf("空链表长度 = %d   （期望 0）\n", list_length(NULL));

    Node *one = build(a1, 1);
    printf("单节点链表长度 = %d   （期望 1）\n", list_length(one));
    free_list(&one);

    /* ---------- 测试 2：按值删除 ---------- */
    printf("\n========== 测试 2：按值删除 ==========\n");

    int r1 = delete_value(&h1, 20);         /* 删中间 */
    printf("删 20 -> 返回 %d（期望 1），链表 ", r1);
    print_list(h1);                         /* 期望 10 -> 30 -> NULL */

    int r2 = delete_value(&h1, 10);         /* 删头节点 ★ 重点 */
    printf("删 10（头节点）-> 返回 %d（期望 1），链表 ", r2);
    print_list(h1);                         /* 期望 30 -> NULL */

    int r3 = delete_value(&h1, 30);         /* 删唯一的节点 */
    printf("删唯一的 30 -> 返回 %d（期望 1），链表 ", r3);
    print_list(h1);                         /* 期望 (空链表) */

    int r4 = delete_value(&h1, 99);         /* 值不存在 */
    printf("在空链表删 99 -> 返回 %d（期望 0）\n", r4);

    int a2[] = {1, 2, 3};
    Node *h2 = build(a2, 3);
    int r5 = delete_value(&h2, 99);         /* 值不存在（非空） */
    printf("在 1->2->3 删 99 -> 返回 %d（期望 0），链表 ", r5);
    print_list(h2);                         /* 期望 1 -> 2 -> 3 -> NULL */
    free_list(&h2);

    /* ---------- 测试 3：指定位置插入 ---------- */
    printf("\n========== 测试 3：指定位置插入 ==========\n");
    int a3[] = {10, 30};
    Node *h3 = build(a3, 2);                /* 10 -> 30 */

    printf("初始链表 ");
    print_list(h3);

    int i1 = insert_at(&h3, 1, 20);         /* 插到下标 1（中间） */
    printf("在下标 1 插 20 -> 返回 %d（期望 1），链表 ", i1);
    print_list(h3);                         /* 期望 10 -> 20 -> 30 -> NULL */

    int i2 = insert_at(&h3, 0, 5);          /* 插到下标 0（头部）★ */
    printf("在下标 0 插 5（头部）-> 返回 %d（期望 1），链表 ", i2);
    print_list(h3);                         /* 期望 5 -> 10 -> 20 -> 30 -> NULL */

    int i3 = insert_at(&h3, 4, 99);         /* 插到末尾（下标 == 长度） */
    printf("在下标 4 插 99（末尾）-> 返回 %d（期望 1），链表 ", i3);
    print_list(h3);                         /* 期望 5 -> 10 -> 20 -> 30 -> 99 -> NULL */

    int i4 = insert_at(&h3, 99, 77);        /* 越界 */
    printf("在下标 99 插 77（越界）-> 返回 %d（期望 0）\n", i4);

    Node *h4 = NULL;
    int i5 = insert_at(&h4, 0, 42);         /* 空链表插下标 0 */
    printf("空链表下标 0 插 42 -> 返回 %d（期望 1），链表 ", i5);
    print_list(h4);                         /* 期望 42 -> NULL */
    free_list(&h4);
    free_list(&h3);

    /* ---------- 测试 4：找中间节点 ---------- */
    printf("\n========== 测试 4：找中间节点 ==========\n");
    int a4[] = {1, 2, 3, 4, 5};
    Node *h5 = build(a4, 5);
    printf("奇数个（1->2->3->4->5）中间 = %d   （期望 3）\n", find_middle(h5)->data);

    int a5[] = {1, 2, 3, 4};
    Node *h6 = build(a5, 4);
    printf("偶数个（1->2->3->4）中间 = %d   （期望 3，或 2 取决于实现）\n", find_middle(h6)->data);

    Node *h7 = build(a4, 1);
    printf("单个节点中间 = %d   （期望 1）\n", find_middle(h7)->data);
    printf("空链表中间 = %s   （期望 NULL）\n", find_middle(NULL) == NULL ? "NULL" : "非 NULL");
    free_list(&h5); free_list(&h6); free_list(&h7);

    /* ---------- 测试 5：判断是否有环 ---------- */
    printf("\n========== 测试 5：判断是否有环 ==========\n");
    int a6[] = {1, 2, 3, 4};
    Node *h8 = build(a6, 4);
    printf("无环链表（1->2->3->4）has_cycle = %d   （期望 0）\n", has_cycle(h8));

    /* 人为造一个环：让最后一个节点指回第 2 个节点 */
    Node *tail = h8;
    while (tail->next != NULL) tail = tail->next;
    tail->next = h8->next;                  /* 4 -> 2，形成环 */
    printf("有环链表 has_cycle = %d   （期望 1）\n", has_cycle(h8));
    tail->next = NULL;                      /* 拆掉环，才能安全释放 */
    free_list(&h8);

    /* ---------- 测试 6：合并两个有序链表 ---------- */
    printf("\n========== 测试 6：合并两个有序链表 ==========\n");
    int b1[] = {1, 3, 5};
    int b2[] = {2, 4, 6};
    Node *m1 = build(b1, 3);
    Node *m2 = build(b2, 3);

    printf("合并前: ");
    print_list(m1);
    printf("        ");
    print_list(m2);

    Node *merged = merge_sorted(m1, m2);
    printf("合并后: ");
    print_list(merged);                     /* 期望 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL */

    /* 注意：合并后 m1 和 m2 的节点已被 merged 接管，只需释放 merged */
    free_list(&merged);

    /* 边界：一边为空 */
    Node *m3 = build(b1, 3);
    Node *merged2 = merge_sorted(m3, NULL);
    printf("和一个空链表合并: ");
    print_list(merged2);                    /* 期望 1 -> 3 -> 5 -> NULL */
    free_list(&merged2);

    printf("\n========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面 6 个函数由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 1. 求链表长度 ----------
 * 空链表返回 0
 */
int list_length(Node *head)
{
    Node *p=head;
    int count=0;
    while(p!=NULL)
    {
        count++;
        p=p->next;
    }
    return count;
}


/* ---------- 2. 按值删除第一个匹配的节点 ----------
 * 返回值：删掉了返回 1，没找到（或空链表）返回 0
 *
 * 必须处理三种情况：
 *   (a) 空链表            -> 返回 0
 *   (b) 要删的是【头节点】 -> *head 要指向下一个（这就是为什么参数是 Node **）
 *   (c) 要删的是中间/尾部  -> 找到前一个节点，让它跳过被删的节点
 *
 * 提示：
 *   - 删除前要 free 掉节点（否则内存泄漏）
 *   - 处理 (b) 时注意：free 之前要先保存下一个节点
 */
int delete_value(Node **head, int value)
{
    Node *cur_node=*head;
    if(cur_node==NULL)
    {
        return 0;
    }
    Node *prev_node=NULL;
    while(cur_node!=NULL&&cur_node->data!=value)
    {
        prev_node=cur_node;
        cur_node=cur_node->next;
    }
    if(cur_node==NULL)
    {
        return 0;
    }
    if(prev_node==NULL)
    {
        *head=cur_node->next;
    }
    else
    {
        prev_node->next=cur_node->next;
    }
    free(cur_node);
    return 1;
}


/* ---------- 3. 在指定下标插入 ----------
 * pos 表示"插入后新节点所在的下标"（0 开始）
 *   - pos == 0          -> 插到头部
 *   - pos == 长度       -> 插到末尾
 *   - pos > 长度 或 <0  -> 越界，返回 0（不插入）
 * 返回值：成功 1，失败 0
 *
 * 提示：
 *   - pos == 0 时要改 *head，所以要单独处理
 *   - 其他情况：先走到"下标 pos-1"的节点，然后在它后面插入
 */
int insert_at(Node **head, int pos, int value)
{
    int len=list_length(*head);
    if((pos<0||pos>len))
    {
        return 0;
    }
    if(pos==0)
    {
        Node *new_node=create_node(value);
        if (new_node == NULL) return 0;      /* 分配失败 */
        new_node->next=*head;
        *head=new_node;
        return 1;
    }
    Node *p=*head;
    for(int i=0;i<pos-1;i++)
    {
        p=p->next;
    }
    Node *new_node=create_node(value);
    if (new_node == NULL) return 0;      /* 分配失败 */
    new_node->next=p->next;
    p->next=new_node;
    return 1;
}


/* ---------- 4. 找中间节点（快慢指针）★ 面试高频 ----------
 * 要求：只遍历一次
 *   - 空链表返回 NULL
 *   - 奇数个节点：返回正中间那个（1->2->3->4->5 返回 3）
 *   - 偶数个节点：返回后中间那个（1->2->3->4 返回 3），也可以返回前一个，自己定
 *
 * 提示：用两个指针
 *   slow 每次走 1 步，fast 每次走 2 步
 *   fast 走到末尾时，slow 正好在中间
 */
Node *find_middle(Node *head)
{
    if(head==NULL)
    {
        return NULL;
    }
    Node *slow=head;
    Node *fast=head;
    while(fast!=NULL&&fast->next!=NULL)
    {
        slow=slow->next;
        fast=fast->next->next;
    }
    return slow;
}


/* ---------- 5. 判断链表是否有环（Floyd 判环）----------
 * 有环返回 1，无环返回 0
 *
 * 提示：还是快慢指针
 *   slow 走 1 步，fast 走 2 步
 *   如果链表有环，fast 一定会在环里追上 slow（两者相遇）
 *   如果无环，fast 会先到达 NULL
 *
 * 注意循环条件：fast 和 fast->next 都要判 NULL
 *   （否则 fast->next->next 会解引用空指针）
 */
int has_cycle(Node *head)
{
    Node *slow=head;
    Node *fast=head;
    while((fast!=NULL&&fast->next!=NULL))
    {
        slow=slow->next;
        fast=fast->next->next;
        if(slow==fast)
        {
            return 1;
        }
    }
    return 0;
}


/* ---------- 6. 合并两个【有序】链表，返回合并后的头 ----------
 * 要求：
 *   - 合并后仍然有序（升序）
 *   - 不能新建节点，要复用原来的节点（只改 next 指向）
 *   - 原链表 a、b 的节点被接管，调用方只需释放返回的新链表
 *
 * 提示（两种写法，任选）：
 *   写法一：用一个"尾指针"逐个接（推荐，容易想清楚）
 *   写法二：递归
 */
Node *merge_sorted(Node *a, Node *b)
{
    Node dummy;
    Node *tail=&dummy;
    while(a!=NULL&&b!=NULL)
    {
        if(a->data<=b->data)
        {
            tail->next=a;
            a=a->next;
        }
        else
        {
            tail->next=b;
            b=b->next;
        }
        tail=tail->next;
    }
    tail->next=(a!=NULL)?a:b;
    return dummy.next;
}
