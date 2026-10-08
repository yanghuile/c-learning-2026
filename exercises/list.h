/* ============================================================
 *  list.h —— 单链表工具库（头文件）
 *
 *  这个文件只放【声明】：告诉别人"有哪些函数可以用"。
 *  实现放在 list.c 里。
 *
 *  include guard 防止被重复包含（你 W4 学过的）
 * ============================================================ */
#ifndef LIST_H
#define LIST_H

/* ---------- 节点类型 ---------- */
typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

/* ---------- 基础操作（你 W5-Day2 写的）---------- */
Node *create_node(int data);
void  insert_head(Node **head, int data);
void  insert_tail(Node **head, int data);
void  print_list(Node *head);
Node *find(Node *head, int value);
void  free_list(Node **head);

/* ---------- 进阶操作（你 W5-Day3 写的）---------- */
int   list_length(Node *head);
int   delete_value(Node **head, int value);
int   insert_at(Node **head, int pos, int value);
Node *find_middle(Node *head);
int   has_cycle(Node *head);
Node *merge_sorted(Node *a, Node *b);

/* ---------- 建链表的便利函数（测试用）----------
 * 从数组建一条链表，返回头指针
 * 例：int a[] = {1,2,3};  Node *h = list_from_array(a, 3);
 */
Node *list_from_array(const int *arr, int n);

#endif /* LIST_H */
