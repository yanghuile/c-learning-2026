/* ============================================================
 *  list.c —— 单链表工具库（实现）
 *
 *  这里的代码全部来自你 W5-Day2 和 W5-Day3 写的内容，
 *  只是从"每个练习文件拷一份"改成了"集中放一处"。
 *
 *  编译方式：和 main 一起编译
 *      gcc -std=c11 -Wall -Wextra -Wpedantic main.c list.c -o main
 * ============================================================ */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* ---------- 1. 创建节点 ---------- */
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

/* ---------- 2. 头插 ---------- */
void insert_head(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) return;
    new_node->next = *head;
    *head = new_node;
}

/* ---------- 3. 尾插 ---------- */
void insert_tail(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) return;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }

    Node *p = *head;
    while (p->next != NULL) p = p->next;
    p->next = new_node;
}

/* ---------- 4. 遍历打印 ---------- */
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

/* ---------- 5. 按值查找 ---------- */
Node *find(Node *head, int value)
{
    for (Node *p = head; p != NULL; p = p->next)
    {
        if (p->data == value) return p;
    }
    return NULL;
}

/* ---------- 6. 释放整条链表 ---------- */
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

/* ---------- 7. 求长度 ---------- */
int list_length(Node *head)
{
    int count = 0;
    for (Node *p = head; p != NULL; p = p->next) count++;
    return count;
}

/* ---------- 8. 按值删除 ---------- */
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

    if (cur_node == NULL) return 0;

    if (prev_node == NULL) *head = cur_node->next;
    else                   prev_node->next = cur_node->next;

    free(cur_node);
    return 1;
}

/* ---------- 9. 指定位置插入 ---------- */
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

/* ---------- 10. 找中间节点（快慢指针）---------- */
Node *find_middle(Node *head)
{
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/* ---------- 11. 判断是否有环（Floyd）---------- */
int has_cycle(Node *head)
{
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;
    }
    return 0;
}

/* ---------- 12. 合并两个有序链表 ---------- */
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

/* ---------- 13. 从数组建链表（便利函数）---------- */
Node *list_from_array(const int *arr, int n)
{
    Node *head = NULL;
    for (int i = 0; i < n; i++) insert_tail(&head, arr[i]);
    return head;
}
