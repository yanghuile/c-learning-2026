/* ============================================================
 *  W5-Day2 链表基础操作
 *
 *  今天要实现 6 个函数（见下方标注 "TODO" 的地方）。
 *
 *  约定：单链表，【不带头节点】（第一个节点就是数据节点）
 *        —— 这是最常见的写法，也是面试默认的写法
 *
 *  参考：昨天你画的图（exercises/W5-Day1-链表预览.md）
 *
 *  写完后运行：
 *      b exercises/W5-Day2-链表基础.c
 *  看输出是否和预期一致。
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>     /* malloc / free */

/* ============================================================
 *  节点定义
 * ============================================================ */
typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

/* ============================================================
 *  函数声明
 * ============================================================ */
Node *create_node(int data);
void  insert_head(Node **head, int data);
void  insert_tail(Node **head, int data);
void  print_list(Node *head);
Node *find(Node *head, int value);
void  free_list(Node **head);

/* ============================================================
 *  main：测试代码（已写好，不用改）
 * ============================================================ */
int main(void)
{
    Node *head = NULL;          /* 空链表：head 为 NULL */

    /* ---------- 测试 1：头插 ---------- */
    printf("========== 测试 1：头插 ==========\n");
    printf("向空链表头插 10：\n");
    insert_head(&head, 10);
    print_list(head);           /* 期望: 10 -> NULL */

    printf("再头插 20、30（注意顺序是倒的）：\n");
    insert_head(&head, 20);
    insert_head(&head, 30);
    print_list(head);           /* 期望: 30 -> 20 -> 10 -> NULL */

    /* ---------- 测试 2：尾插 ---------- */
    printf("\n========== 测试 2：尾插 ==========\n");
    printf("在 30->20->10 后面尾插 40、50：\n");
    insert_tail(&head, 40);
    insert_tail(&head, 50);
    print_list(head);           /* 期望: 30 -> 20 -> 10 -> 40 -> 50 -> NULL */

    /* ---------- 测试 3：查找 ---------- */
    printf("\n========== 测试 3：查找 ==========\n");
    Node *found = find(head, 10);
    if (found != NULL)
        printf("找到 10，它的地址 = %p，data = %d\n", (void *)found, found->data);
    else
        printf("没找到 10（错误！）\n");

    found = find(head, 99);
    printf("查找 99：%s\n", found == NULL ? "没找到（正确）" : "居然找到了（错误！）");

    /* ---------- 测试 4：空链表边界 ---------- */
    printf("\n========== 测试 4：空链表 ==========\n");
    Node *empty = NULL;
    printf("打印空链表：");
    print_list(empty);          /* 期望: 不崩溃，输出 (空链表) */
    printf("在空链表里查找：");
    found = find(empty, 1);
    printf("%s\n", found == NULL ? "返回 NULL（正确）" : "错误");
    printf("释放空链表：");
    free_list(&empty);          /* 期望: 不崩溃 */
    printf("OK\n");

    /* ---------- 测试 5：释放整条链表 ---------- */
    printf("\n========== 测试 5：释放链表 ==========\n");
    printf("释放前链表长度（自己数）：");
    print_list(head);
    free_list(&head);
    printf("释放后 head = %s（应为 NULL）\n", head == NULL ? "NULL（正确）" : "非 NULL（错误！）");
    printf("释放后打印：");
    print_list(head);


    printf("\n===== 测试 6：空链表尾插（边界）=====\n");
    Node *empty2 = NULL;
    insert_tail(&empty2, 77);
    printf("空链表尾插 77 后：");
    print_list(empty2);        /* 期望: 77 -> NULL */
    free_list(&empty2);

    printf("\n========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面 6 个函数由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 1. 创建一个节点 ----------
 * 要求：
 *   - 用 malloc 分配一个 Node
 *   - 把 data 填进去，next 设为 NULL（新节点后面没有东西）
 *   - 返回节点指针
 *   - malloc 可能失败返回 NULL，要处理（打印错误并返回 NULL）
 */
Node *create_node(int data)
{
    Node *p=(Node *)malloc(sizeof(Node));
    if(p==NULL)
    {
        printf("malloc failed for create_node\n");
        return NULL;
    }
    p->data=data;
    p->next=NULL;
    return p;
}


/* ---------- 2. 头插 ----------
 * 要求：
 *   - 新建一个节点（调用 create_node）
 *   - 让它成为第一个节点：新节点的 next 指向原来的 head
 *   - 然后让 head 指向新节点
 *
 * 注意参数是 Node **head：
 *   - *head 才是真正的头指针
 *   - 因为空链表时 head 会从 NULL 变成指向新节点，必须改 head 本身
 */
void insert_head(Node **head, int data)
{
    Node *new_node=create_node(data);
    if(new_node==NULL)
    {
        return;
    }
    new_node->next=*head;
    *head=new_node;
}


/* ---------- 3. 尾插 ----------
 * 要求：
 *   - 新建节点
 *   - 如果链表为空（*head == NULL）：直接让 *head 指向新节点
 *   - 否则：从 *head 开始走到最后一个节点（next == NULL 的那个），
 *           把它的 next 指向新节点
 *
 * 提示：需要两个指针 —— 一个记录当前节点，一个从头开始走
 *       注意不要用 *head 本身去遍历，否则头指针就丢了
 */
void insert_tail(Node **head, int data)
{
    Node *new_node=create_node(data);
    if(new_node==NULL)
    {
        return;
    }
    if(*head==NULL)
    {
        *head = new_node;
        return;
    }
    Node *p=*head;
    while(p->next!=NULL)
    {
        p=p->next;
    }
    p->next=new_node;
}


/* ---------- 4. 遍历打印 ----------
 * 要求：
 *   - 从头走到尾，依次打印 data
 *   - 格式：30 -> 20 -> 10 -> NULL
 *   - 空链表打印：(空链表)
 */
void print_list(Node *head)
{
    if(head==NULL)
    {
        printf("（空链表）\n");
        return;
    }
    for(Node *p=head;p!=NULL;p=p->next)
    {
        printf("%d",p->data);
        if(p->next!=NULL)
        {
            printf(" -> ");
        }
    }
    printf(" -> NULL\n");
}


/* ---------- 5. 按值查找 ----------
 * 要求：
 *   - 从头开始找，找到返回该节点指针
 *   - 找不到返回 NULL
 *   - 空链表直接返回 NULL
 */
Node *find(Node *head, int value)
{
    Node *p=head;
    if(p==NULL)
    {
        return NULL;
    }
    while(p!=NULL)
    {
        if(p->data==value)
        {
            return p;
        }
        p=p->next;
    }
    return NULL;
}


/* ---------- 6. 释放整条链表 ----------
 * 要求：
 *   - 逐个 free 每个节点
 *   - 【关键】free 当前节点前，必须先保存 next 指针，
 *             否则 free 之后就找不到下一个节点了
 *   - 最后把 *head 设为 NULL（避免变成野指针）
 */
void free_list(Node **head)
{
    Node *p=*head;
    while(p!=NULL)
    {
        Node *next_node=p->next;
        free(p);
        p=next_node;
    }
    *head=NULL;
}
