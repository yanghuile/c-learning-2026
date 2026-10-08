/* ============================================================
 *  W5-Day4 链表反转（面试最高频）+ 面试题
 *
 *  本周重点：反转必须能手写默写。
 *
 *  【本文件用到了 list.h / list.c】（你前两天写的函数抽出来的库）
 *  编译方式（多文件）：
 *      gcc -std=c11 -Wall -Wextra -Wpedantic W5-Day4-链表反转.c list.c -o reverse
 *  或者直接用我给你的 Makefile：
 *      mingw32-make DAY4        （先按下面说明建好 Makefile）
 *
 *  ⚠️ 写每个函数前先问：
 *     空链表？只有一个节点？两个节点？反转后 head 变了吗？
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"        /* ← 这就是"复用"：链表函数不用重写 */

/* ---------- 今天要实现的函数 ---------- */
Node *reverse_iter(Node *head);          /* 迭代法反转 */
Node *reverse_rec(Node *head);           /* 递归法反转 */
Node *reverse_k(Node *head, int k);      /* 每 k 个一组反转（进阶，选做） */
int   is_palindrome(Node *head);         /* 判断回文（选做） */
Node *find_kth_from_end(Node *head, int k); /* 倒数第 k 个（面试题） */

/* ============================================================
 *  main：测试代码
 * ============================================================ */
int main(void)
{
    /* ---------- 测试 1：迭代法反转 ---------- */
    printf("========== 测试 1：迭代法反转 ==========\n");
    int a1[] = {1, 2, 3, 4, 5};
    Node *h1 = list_from_array(a1, 5);

    printf("反转前: ");
    print_list(h1);

    h1 = reverse_iter(h1);

    printf("反转后: ");
    print_list(h1);                       /* 期望 5 -> 4 -> 3 -> 2 -> 1 -> NULL */
    free_list(&h1);

    /* 边界：空链表 */
    Node *empty = NULL;
    empty = reverse_iter(empty);
    printf("空链表反转: %s   （期望 (空链表)）\n", empty == NULL ? "(空链表)" : "错误");

    /* 边界：单个节点 */
    int a2[] = {7};
    Node *h2 = list_from_array(a2, 1);
    h2 = reverse_iter(h2);
    printf("单节点反转: ");
    print_list(h2);                       /* 期望 7 -> NULL */
    free_list(&h2);

    /* 边界：两个节点 */
    int a3[] = {1, 2};
    Node *h3 = list_from_array(a3, 2);
    h3 = reverse_iter(h3);
    printf("两节点反转: ");
    print_list(h3);                       /* 期望 2 -> 1 -> NULL */
    free_list(&h3);

    /* ---------- 测试 2：递归法反转 ---------- */
    printf("\n========== 测试 2：递归法反转 ==========\n");
    int a4[] = {1, 2, 3, 4, 5};
    Node *h4 = list_from_array(a4, 5);

    printf("反转前: ");
    print_list(h4);

    h4 = reverse_rec(h4);

    printf("反转后: ");
    print_list(h4);                       /* 期望 5 -> 4 -> 3 -> 2 -> 1 -> NULL */

    /* 反转两次应该变回原样 */
    h4 = reverse_rec(h4);
    printf("再反转一次: ");
    print_list(h4);                       /* 期望 1 -> 2 -> 3 -> 4 -> 5 -> NULL */
    free_list(&h4);

    Node *h5 = list_from_array(a2, 1);
    h5 = reverse_rec(h5);
    printf("单节点递归反转: ");
    print_list(h5);                       /* 期望 7 -> NULL */
    free_list(&h5);

    /* ---------- 测试 3：倒数第 k 个节点（面试题）---------- */
    printf("\n========== 测试 3：倒数第 k 个节点 ==========\n");
    int a6[] = {1, 2, 3, 4, 5};
    Node *h6 = list_from_array(a6, 5);

    printf("链表 ");
    print_list(h6);

    Node *k1 = find_kth_from_end(h6, 1);   /* 倒数第 1 个 = 最后一个 */
    printf("倒数第 1 个 = %d   （期望 5）\n", k1 != NULL ? k1->data : -1);

    Node *k2 = find_kth_from_end(h6, 2);
    printf("倒数第 2 个 = %d   （期望 4）\n", k2 != NULL ? k2->data : -1);

    Node *k5 = find_kth_from_end(h6, 5);   /* 倒数第 5 个 = 第一个 */
    printf("倒数第 5 个 = %d   （期望 1）\n", k5 != NULL ? k5->data : -1);

    Node *k6 = find_kth_from_end(h6, 6);   /* 超过长度 */
    printf("倒数第 6 个 = %s   （期望 NULL）\n", k6 == NULL ? "NULL" : "非 NULL");

    Node *k0 = find_kth_from_end(h6, 0);   /* 非法 k */
    printf("倒数第 0 个 = %s   （期望 NULL）\n", k0 == NULL ? "NULL" : "非 NULL");

    Node *kempty = find_kth_from_end(NULL, 1);
    printf("空链表倒数第 1 个 = %s   （期望 NULL）\n", kempty == NULL ? "NULL" : "非 NULL");
    free_list(&h6);

    /* ---------- 测试 4：判断回文（选做）---------- */
    printf("\n========== 测试 4：判断回文（选做）==========\n");
    int p1[] = {1, 2, 3, 2, 1};
    int p2[] = {1, 2, 2, 1};
    int p3[] = {1, 2, 3, 4};
    Node *pa = list_from_array(p1, 5);
    Node *pb = list_from_array(p2, 4);
    Node *pc = list_from_array(p3, 4);

    printf("1->2->3->2->1 : %d   （期望 1）\n", is_palindrome(pa));
    printf("1->2->2->1    : %d   （期望 1）\n", is_palindrome(pb));
    printf("1->2->3->4    : %d   （期望 0）\n", is_palindrome(pc));
    printf("空链表        : %d   （期望 1，空链表算回文）\n", is_palindrome(NULL));
    free_list(&pa); free_list(&pb); free_list(&pc);

    /* ---------- 测试 5：每 k 个一组反转（进阶，选做）---------- */
    printf("\n========== 测试 5：每 k 个一组反转（选做）==========\n");
    int k1a[] = {1, 2, 3, 4, 5};
    Node *ka = list_from_array(k1a, 5);
    printf("1->2->3->4->5, k=2 -> ");
    ka = reverse_k(ka, 2);
    print_list(ka);                        /* 期望 2 -> 1 -> 4 -> 3 -> 5 -> NULL */
    free_list(&ka);

    printf("\n========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 1. 迭代法反转 ★★★ 必须会 ----------
 * 输入：1 -> 2 -> 3 -> NULL
 * 输出：3 -> 2 -> 1 -> NULL
 *
 * 思路：边走边把 next 指针"掉头"
 *   需要三个指针：prev（前一个）、cur（当前）、next（下一个）
 *
 * 关键点：
 *   (a) 每一轮先保存 cur->next（否则掉头后就找不到后面了）
 *   (b) 让 cur->next 指向 prev（掉头）
 *   (c) prev 和 cur 都往前挪一格
 *   (d) 循环结束时 prev 就是【新的头】，要返回它
 *
 * 先在纸上画 3 个节点，把每一步的 prev/cur/next 画出来，再写代码。
 */
Node *reverse_iter(Node *head)
{
    Node *prev=NULL;
    Node *cur=head;
    Node *next;      //先声明，不在外面取cur->next,因为cur有可能是空

    while(cur!=NULL)
    {
        next=cur->next;
        cur->next=prev;
        prev=cur;
        cur=next;
    }
    return prev;
}


/* ---------- 2. 递归法反转 ----------
 * 思路：
 *   - 递归的"终止条件"：空链表或只有一个节点，直接返回它自己
 *   - 把 head->next 之后的整段先反转好（递归调用）
 *   - 反转后，head->next 变成了新链表的【尾节点】
 *     所以要让 head->next->next = head（把 head 接到尾巴上）
 *   - 再把 head->next 设为 NULL（head 成为新尾）
 *   - 返回递归得到的新头
 *
 * 提示：
 *   Node *new_head = reverse_rec(head->next);   // 先反转后面的
 *   head->next->next = head;                    // 把 head 接上去
 *   head->next = NULL;                          // head 变新尾
 *   return new_head;
 */
Node *reverse_rec(Node *head)
{
    if(head==NULL||head->next==NULL)
    {
        return head;
    }
    Node *new_head=reverse_rec(head->next);
    head->next->next = head;
    head->next = NULL;
    return new_head;
}


/* ---------- 3. 倒数第 k 个节点（面试题）★ ----------
 * 要求：只遍历一次（不能用"先算长度再走"的两遍法）
 *
 * 提示：快慢指针的变体
 *   - 先让 fast 从头走 k 步
 *   - 然后 slow 和 fast 一起走，直到 fast 到 NULL
 *   - 此时 slow 就在倒数第 k 个
 *
 * 边界：
 *   - 空链表、k <= 0        -> 返回 NULL
 *   - k 大于链表长度        -> 返回 NULL（fast 会在走 k 步前就变 NULL）
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
        //没走完k步就空了，说明k超过了链表长度
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


/* ---------- 4. 判断回文（选做，较难）----------
 * 1->2->3->2->1 返回 1；1->2->3->4 返回 0；空链表返回 1
 *
 * 思路（O(n) 时间，O(1) 空间）：
 *   1. 用快慢指针找到中间节点
 *   2. 把后半段反转
 *   3. 从头和从中间同时走，逐个比较
 *   4. （加分）比较完把后半段再反转回去，恢复原链表
 */
int is_palindrome(Node *head)
{
    if(head==NULL)
    {
        return 1;
    }
    Node *middle=find_middle(head);
    Node *p2=reverse_iter(middle);
    Node *p1=head;
    while(p1!=NULL&&p2!=NULL)
    {
        if(p1->data!=p2->data)
        {
            return 0;
        }
        p1=p1->next;
        p2=p2->next;
    }
    reverse_iter(p2);
    return 1;
}

/* ---------- 5. 每 k 个一组反转（选做，最难）----------
 * 1->2->3->4->5, k=2 -> 2->1->4->3->5
 * 1->2->3->4->5, k=3 -> 3->2->1->4->5   （最后不足 k 个保持原样）
 *
 * 提示：
 *   - 先把前 k 个节点反转（和 reverse_iter 类似，但只走 k 步）
 *   - 记录反转后的新头、新的尾
 *   - 递归/循环处理剩下的
 */
Node *reverse_k(Node *head, int k)
{
    if (head == NULL || k <= 1)
    {
        return head;
    }

    Node *new_head        = NULL;   
    Node *prev_group_tail = NULL;   
    Node *cur_head        = head;   

    while (cur_head != NULL)
    {
        Node *check = cur_head;
        int   enough = 1;

        for (int i = 0; i < k; i++)
        {
            if (check == NULL)
            {
                enough = 0;
                break;
            }
            check = check->next;
        }

        if (!enough)
        {
            if (prev_group_tail != NULL)
            {
                prev_group_tail->next = cur_head;
            }
            break;
        }

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

        if (new_head == NULL)
        {
            new_head = prev;
        }

        if (prev_group_tail != NULL)
        {
            prev_group_tail->next = prev;
        }

        prev_group_tail = cur_head;

        cur_head = cur;
    }

    return new_head;
}
