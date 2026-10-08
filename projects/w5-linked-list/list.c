/* ============================================================
 *  list.c —— 单链表工具库（实现）
 *
 *  数据结构练习项目 · W5
 *
 *  ⚠️ 有三个函数标了 TODO —— 那是你今天要补的（你昨天写过，现在默一遍）。
 *     写完运行：
 *         mingw32-make check    只检查警告
 *         mingw32-make test     编译并运行测试
 * ============================================================ */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 *  一、生命周期
 * ============================================================ */

Node *create_node(int data)
{
    Node *p = (Node *)malloc(sizeof(Node));
    if (p == NULL)
    {
        printf("malloc failed\n");
        return NULL;
    }
    p->data = data;
    p->next = NULL;
    return p;
}

void free_list(Node **head)
{
    Node *p = *head;
    while (p != NULL)
    {
        Node *next_node = p->next;      /* 先保存，再 free */
        free(p);
        p = next_node;
    }
    *head = NULL;                       /* 防止变成野指针 */
}

/* ============================================================
 *  二、插入
 * ============================================================ */

void insert_head(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) return;

    new_node->next = *head;
    *head = new_node;
}

void insert_tail(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) return;

    if (*head == NULL)
    {
        *head = new_node;               /* ★ 空链表：新节点就是第一个 */
        return;
    }

    Node *p = *head;
    while (p->next != NULL) p = p->next;
    p->next = new_node;
}

int insert_at(Node **head, int pos, int value)
{
    int len = list_length(*head);
    if (pos < 0 || pos > len) return 0;

    if (pos == 0)
    {
        Node *new_node = create_node(value);
        if (new_node == NULL) return 0;
        new_node->next = *head;
        *head = new_node;
        return 1;
    }

    Node *p = *head;
    for (int i = 0; i < pos - 1; i++) p = p->next;

    Node *new_node = create_node(value);
    if (new_node == NULL) return 0;
    new_node->next = p->next;
    p->next = new_node;
    return 1;
}

/* ============================================================
 *  三、删除
 * ============================================================ */

int delete_value(Node **head, int value)
{
    Node *cur_node = *head;
    if (cur_node == NULL) return 0;

    Node *prev_node = NULL;
    while (cur_node != NULL && cur_node->data != value)
    {
        prev_node = cur_node;
        cur_node = cur_node->next;
    }

    if (cur_node == NULL) return 0;             /* 没找到 */

    /* prev_node == NULL 说明要删的是头节点 —— 统一处理 */
    if (prev_node == NULL) *head = cur_node->next;
    else                   prev_node->next = cur_node->next;

    free(cur_node);
    return 1;
}

/* ============================================================
 *  四、查找与统计
 * ============================================================ */

Node *find(Node *head, int value)
{
    for (Node *p = head; p != NULL; p = p->next)
    {
        if (p->data == value) return p;
    }
    return NULL;
}

int list_length(Node *head)
{
    int count = 0;
    for (Node *p = head; p != NULL; p = p->next) count++;
    return count;
}

Node *find_middle(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;                                /* 空链表时返回 NULL */
}

/* ---------- TODO 1：倒数第 k 个节点（只遍历一次）----------
 * 提示：
 *   - 先让 fast 走 k 步；走不够 k 步（遇到 NULL）说明 k 超过长度 -> 返回 NULL
 *   - 然后 slow 和 fast 一起走到 fast == NULL
 *   - 此时 slow 就是倒数第 k 个
 * 边界：head == NULL 或 k <= 0 -> 返回 NULL
 */
Node *find_kth_from_end(Node *head, int k)
{
    if(head==NULL||k<=0)
    {
        return NULL;
    }
    Node *slow=head;
    Node *fast=head;
    for(int i=0;i<k;i++)
    {
        if(fast==NULL)
        {
            return NULL;
        }
        fast=fast->next;
    }
    while(fast!=NULL)
    {
        slow=slow->next;
        fast=fast->next;
    }
    return slow;
}

int has_cycle(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL)  /* 两个都要判 */
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;              /* 相遇 = 有环 */
    }
    return 0;
}

/* ============================================================
 *  五、反转
 * ============================================================ */

/* ---------- TODO 2：迭代法反转 ----------
 * 你昨天闭卷默写过了，现在再写一遍（不看聊天记录）
 *   三指针：prev / cur / next
 *   四步：存 next -> 掉头 -> 前移 prev -> 前移 cur
 *   返回 prev
 */
Node *reverse_iter(Node *head)
{
    Node *prev=NULL;
    Node *cur=head;
    Node *next;

    while(cur!=NULL)
    {
        next=cur->next;
        cur->next=prev;
        prev=cur;
        cur=next;
    }
    return prev;
}

Node *reverse_rec(Node *head)
{
    /* 终止条件：空链表或只有一个节点 */
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    Node *new_head = reverse_rec(head->next);   /* 先反转后面的 */
    head->next->next = head;                    /* 把 head 接到新尾后面 */
    head->next = NULL;                          /* head 成为新尾 */
    return new_head;
}

Node *reverse_k(Node *head, int k)
{
    if (head == NULL || k <= 1) return head;

    Node *new_head        = NULL;   /* 整个链表的新头（只设一次） */
    Node *prev_group_tail = NULL;   /* 上一组反转后的尾节点 */
    Node *cur_head        = head;   /* 当前这一组的第一个节点 */

    while (cur_head != NULL)
    {
        /* 第 1 步：检查剩下够不够 k 个 */
        Node *check  = cur_head;
        int   enough = 1;
        for (int i = 0; i < k; i++)
        {
            if (check == NULL) { enough = 0; break; }
            check = check->next;
        }

        if (!enough)
        {
            /* 不够 k 个：剩下保持原样，接到上一组尾巴后面 */
            if (prev_group_tail != NULL) prev_group_tail->next = cur_head;
            break;
        }

        /* 第 2 步：反转这 k 个节点 */
        Node *prev = NULL;
        Node *cur  = cur_head;
        Node *next = NULL;
        for (int i = 0; i < k; i++)
        {
            next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }

        /* 反转后：prev=这组新头，cur_head=这组新尾，cur=下一组第一个 */

        if (new_head == NULL) new_head = prev;              /* 只设一次 */
        if (prev_group_tail != NULL) prev_group_tail->next = prev;

        prev_group_tail = cur_head;                         /* ★ 是新尾，不是 prev */
        cur_head = cur;
    }

    return new_head;
}

/* ============================================================
 *  六、合并与判断
 * ============================================================ */

Node *merge_sorted(Node *a, Node *b)
{
    Node  dummy;
    Node *tail = &dummy;
    dummy.next = NULL;

    while (a != NULL && b != NULL)
    {
        if (a->data <= b->data) { tail->next = a; a = a->next; }
        else                    { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}

int is_palindrome(Node *head)
{
    if (head == NULL || head->next == NULL) return 1;

    /* 第 1 步：找中间节点 */
    Node *mid = find_middle(head);

    /* 第 2 步：反转后半段 */
    Node *second_half = reverse_iter(mid);

    /* 第 3 步：从两头比较 */
    Node *p1 = head;
    Node *p2 = second_half;
    int   result = 1;

    while (p2 != NULL)
    {
        if (p1->data != p2->data) { result = 0; break; }
        p1 = p1->next;
        p2 = p2->next;
    }

    /* 第 4 步：把后半段反转回去，恢复原链表 */
    reverse_iter(second_half);

    return result;
}

/* ============================================================
 *  七、输出与便利函数
 * ============================================================ */

void print_list(Node *head)
{
    if (head == NULL)
    {
        printf("(空链表)\n");
        return;
    }
    for (Node *p = head; p != NULL; p = p->next)
    {
        printf("%d", p->data);
        if (p->next != NULL) printf(" -> ");
    }
    printf(" -> NULL\n");
}

Node *list_from_array(const int *arr, int n)
{
    Node *head = NULL;
    for (int i = 0; i < n; i++) insert_tail(&head, arr[i]);
    return head;
}
