/* ============================================================
 *  apply_problems.c —— 链表综合应用题（W5 周六）
 *
 *  核心思想：这类题几乎都是「两个指针不同速度 / 不同起点」
 *
 *  运行：mingw32-make apply
 *
 *  本文件复用 list.h / list.c 的函数（这就是做工程的好处）
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* ============================================================
 *  今天要实现的 4 个函数
 * ============================================================ */

/* ---- 题 1：删除倒数第 N 个节点 ---- */
Node *remove_nth_from_end(Node *head, int n);

/* ---- 题 2：两条链表是否相交，相交则返回交点，否则 NULL ---- */
Node *get_intersection(Node *a, Node *b);

/* ---- 题 3：按奇偶位置拆成两条链 ---- */
void split_odd_even(Node *head, Node **odd, Node **even);

/* ---- 题 4（加分）：链表冒泡排序，只交换节点不交换值 ---- */
Node *sort_list(Node *head);

/* ============================================================
 *  main：测试
 * ============================================================ */
int main(void)
{
    printf("========== 链表综合应用题 ==========\n");

    /* ---------- 测试 1：删除倒数第 N 个 ---------- */
    printf("\n--- 题 1：删除倒数第 N 个节点 ---\n");
    {
        int arr[] = {1, 2, 3, 4, 5};

        Node *h1 = list_from_array(arr, 5);
        printf("原链表: ");
        print_list(h1);
        h1 = remove_nth_from_end(h1, 2);        /* 删 4 */
        printf("删倒数第 2 个后: ");
        print_list(h1);                         /* 期望 1 -> 2 -> 3 -> 5 -> NULL */
        free_list(&h1);

        Node *h2 = list_from_array(arr, 5);
        h2 = remove_nth_from_end(h2, 1);        /* 删最后一个 */
        printf("删倒数第 1 个后: ");
        print_list(h2);                         /* 期望 1 -> 2 -> 3 -> 4 -> NULL */
        free_list(&h2);

        Node *h3 = list_from_array(arr, 5);
        h3 = remove_nth_from_end(h3, 5);        /* 删第一个 ★ 边界 */
        printf("删倒数第 5 个（头节点）后: ");
        print_list(h3);                         /* 期望 2 -> 3 -> 4 -> 5 -> NULL */
        free_list(&h3);

        Node *h4 = list_from_array(arr, 1);
        h4 = remove_nth_from_end(h4, 1);        /* 只有一个节点，删掉 */
        printf("单节点删倒数第 1 个: ");
        print_list(h4);                         /* 期望 (空链表) */
        free_list(&h4);

        Node *h5 = list_from_array(arr, 5);
        Node *h5b = remove_nth_from_end(h5, 99); /* n 超长度，不删 */
        printf("n=99（超长度）: ");
        print_list(h5b);                        /* 期望原样 */
        free_list(&h5b);

        Node *h6 = remove_nth_from_end(NULL, 1);
        printf("空链表: %s\n", h6 == NULL ? "(空链表)" : "错误");
    }

    /* ---------- 测试 2：两条链表相交 ---------- */
    printf("\n--- 题 2：两条链表是否相交 ---\n");
    {
        /* ★ 重要：相交的两条链【共用同一段内存】，
         *   所以释放时【只能释放一次】公共部分。
         *   下面每组建一次就当场验完并释放，避免交叉引用。*/

        /* 组 1：a = 1->2->[7->8->9],  b = 3->4->5->[7->8->9] */
        {
            Node *shared = list_from_array((int[]){7, 8, 9}, 3);
            Node *a = list_from_array((int[]){1, 2}, 2);
            a->next->next = shared;
            Node *b = list_from_array((int[]){3, 4, 5}, 3);
            b->next->next->next = shared;

            printf("链表 a: ");
            print_list(a);
            printf("链表 b: ");
            print_list(b);

            Node *ip = get_intersection(a, b);
            if (ip != NULL)
                printf("交点 = %d   （期望 7）\n", ip->data);
            else
                printf("交点 = NULL   （期望 7）\n");

            /* 只释放各自的前半段，公共段单独释放一次 */
            {
                Node *pa = a;
                while (pa != NULL && pa->next != shared) pa = pa->next;
                if (pa != NULL) pa->next = NULL;
                free_list(&a);

                Node *pb = b;
                while (pb != NULL && pb->next != shared) pb = pb->next;
                if (pb != NULL) pb->next = NULL;
                free_list(&b);

                free_list(&shared);
            }
        }

        /* 组 2：两条独立的链表（不相交） */
        {
            Node *c = list_from_array((int[]){1, 2, 3}, 3);
            Node *d = list_from_array((int[]){4, 5, 6}, 3);
            printf("不相交的两条链: %s   （期望 NULL）\n",
                   get_intersection(c, d) == NULL ? "NULL（正确）" : "错误");
            free_list(&c);
            free_list(&d);
        }

        /* 组 3：一方为空 */
        {
            Node *e = list_from_array((int[]){1, 2, 3}, 3);
            printf("一方为空: %s   （期望 NULL）\n",
                   get_intersection(e, NULL) == NULL ? "NULL（正确）" : "错误");
            free_list(&e);
        }

        /* 组 4：完全相同的两条链（从头就是交点） */
        {
            Node *shared2 = list_from_array((int[]){9, 9}, 2);
            Node *f = shared2;              /* f 和 g 从头就指向同一段 */
            Node *g = shared2;
            Node *ip2 = get_intersection(f, g);
            printf("完全相同的两条链，交点值 = %s   （期望 9）\n",
                   ip2 != NULL ? "9" : "NULL（错误）");
            free_list(&shared2);
        }
    }

    /* ---------- 测试 3：按奇偶位置拆链 ---------- */
    printf("\n--- 题 3：按奇偶位置拆链 ---\n");
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7};

        Node *h = list_from_array(arr, 7);
        printf("原链表: ");
        print_list(h);

        Node *odd  = NULL;
        Node *even = NULL;
        split_odd_even(h, &odd, &even);

        printf("奇数位置（第1,3,5,7个）: ");
        print_list(odd);                        /* 期望 1 -> 3 -> 5 -> 7 -> NULL */
        printf("偶数位置（第2,4,6个）:   ");
        print_list(even);                       /* 期望 2 -> 4 -> 6 -> NULL */

        free_list(&odd);
        free_list(&even);

        /* 边界：单节点 */
        Node *one = list_from_array(arr, 1);
        Node *o1 = NULL, *e1 = NULL;
        split_odd_even(one, &o1, &e1);
        printf("单节点拆分 -> 奇: ");
        print_list(o1);                         /* 期望 1 -> NULL */
        printf("              偶: ");
        print_list(e1);                         /* 期望 (空链表) */
        free_list(&o1);
        free_list(&e1);

        /* 边界：空链表 */
        Node *o2 = NULL, *e2 = NULL;
        split_odd_even(NULL, &o2, &e2);
        printf("空链表拆分 -> 奇: %s, 偶: %s\n",
               o2 == NULL ? "NULL" : "错误", e2 == NULL ? "NULL" : "错误");
    }

    /* ---------- 测试 4：链表排序（加分）---------- */
    printf("\n--- 题 4（加分）：链表冒泡排序 ---\n");
    {
        int arr[] = {5, 2, 8, 1, 9, 3};

        Node *h = list_from_array(arr, 6);
        printf("排序前: ");
        print_list(h);

        h = sort_list(h);

        printf("排序后: ");
        print_list(h);                          /* 期望 1 -> 2 -> 3 -> 5 -> 8 -> 9 -> NULL */

        /* 验证有序 */
        int ok = 1;
        for (Node *p = h; p != NULL && p->next != NULL; p = p->next)
        {
            if (p->data > p->next->data) { ok = 0; break; }
        }
        printf("是否升序: %s\n", ok ? "是（正确）" : "否（错误）");

        /* 验证节点没被新建（地址还是原来那些） */
        printf("长度 = %d   （期望 6）\n", list_length(h));
        free_list(&h);

        /* 边界 */
        Node *empty = sort_list(NULL);
        printf("空链表排序: %s\n", empty == NULL ? "NULL（正确）" : "错误");

        Node *one = list_from_array(arr, 1);
        one = sort_list(one);
        printf("单节点排序: ");
        print_list(one);
        free_list(&one);
    }

    printf("\n========== 结束 ==========\n");
    return 0;
}

/* ============================================================
 *  ↓↓↓ 下面 4 个函数由你实现 ↓↓↓
 * ============================================================ */

/* ---------- 题 1：删除倒数第 N 个节点 ----------
 * 例：1->2->3->4->5, n=2  ->  1->2->3->5
 *
 * 要求：只遍历一次
 *
 * 思路：快慢指针 + 哑节点
 *   - 用哑节点（dummy）指向 head，这样"删头节点"和"删中间"逻辑就统一了
 *     （这就是你链表预览那天学的"头节点"思想！）
 *   - fast 先走 n+1 步（比 slow 多 n+1 步）
 *   - 然后 fast 和 slow 一起走，直到 fast == NULL
 *   - 此时 slow 正好停在"要删的节点的前一个"
 *   - 然后 slow->next = slow->next->next，free 掉被删的
 *
 * 边界：
 *   - head == NULL 或 n <= 0  -> 原样返回
 *   - n 大于链表长度           -> 原样返回（不删）
 *   - 返回新的头（删头节点时头会变）
 *
 * 框架：
 *   Node dummy;
 *   dummy.next = head;
 *   Node *fast = &dummy;
 *   Node *slow = &dummy;
 *   ... 走位 ...
 *   if (fast != NULL) return head;   // n 超长度
 *   Node *to_delete = slow->next;
 *   slow->next = to_delete->next;
 *   free(to_delete);
 *   return dummy.next;               // 注意返回 dummy.next 而不是 head（删头时会变）
 */
Node *remove_nth_from_end(Node *head, int n)
{
    if(head==NULL||n<=0)
    {
        return head;
    }
    Node dummy;
    dummy.next=head;
    Node *slow=&dummy;
    Node *fast=&dummy;
    for(int i=0;i<n+1;i++)
    {
        if(fast==NULL)
        {
            return dummy.next;
        }
        fast=fast->next;
    }
    while(fast!=NULL)
    {
        slow=slow->next;
        fast=fast->next;
    }
    Node *to_delete=slow->next;
    slow->next=slow->next->next;
    free(to_delete);
    return dummy.next;
}


/* ---------- 题 2：两条链表是否相交 ----------
 * 相交的定义：两条链表从某个节点开始【共用同一段内存】
 *             （注意：不是"值相同"，而是【节点地址相同】）
 *
 * 返回：交点节点指针；不相交返回 NULL
 *
 * 思路一（简单，O(n) 空间）：用长度差
 *   1. 分别求两条链表的长度 lenA、lenB
 *   2. 长的先走 |lenA - lenB| 步（让两条链表"对齐"）
 *   3. 然后两个指针一起走，第一次相遇的节点就是交点
 *
 * 思路二（巧妙，O(1) 空间）：双指针走两遍
 *   p1 走完 A 后接着走 B，p2 走完 B 后接着走 A
 *   两者走过的总长度相同，必然在交点相遇（或同时到 NULL）
 *
 * 任选一种实现。
 * 边界：任一条为空 -> 返回 NULL
 */
Node *get_intersection(Node *a, Node *b)
{
    if(a==NULL||b==NULL)
    {
        return NULL;
    }
    Node *p1=a;
    Node *p2=b;
    while(p1!=p2)
    {
        p1 = (p1 == NULL) ? b : p1->next;
        p2 = (p2 == NULL) ? a : p2->next;
    }
    return p1;
}


/* ---------- 题 3：按奇偶位置拆成两条链 ----------
 * 例：1->2->3->4->5->6->7
 *     奇数位置 -> 1->3->5->7
 *     偶数位置 -> 2->4->6
 *
 * 注意：
 *   - 是"按位置"拆，不是按"值的奇偶"拆
 *   - 拆完后原链表被拆解（节点被两条新链表接管），不要再 free 原 head
 *   - odd / even 都是输出参数（指针的地址），要改它们本身
 *
 * 思路：
 *   - 两个尾指针 odd_tail、even_tail 分别指向两条新链的尾部
 *   - 用一个游标 p 遍历原链表，同时用一个计数器判断当前是奇位还是偶位
 *   - 每次把节点接到对应的尾巴上，然后更新那个尾巴
 *
 * 边界：空链表 -> *odd = *even = NULL
 */
void split_odd_even(Node *head, Node **odd, Node **even)
{
    if(head==NULL)
    {
        *odd=*even=NULL;
        return;
    }
    Node *p=head;
    Node *odd_tail=NULL;
    Node *even_tail=NULL;
    int count=1;
    while(p!=NULL)
    {
        if(count%2==1)
        {
            if(*odd==NULL)
            {
                *odd=p;
                odd_tail=p;
            }
            else
            {
                odd_tail->next=p;
                odd_tail=p;
            }
        }
        else
        {
            if(*even==NULL)
            {
                *even=p;
                even_tail=p;
            }
            else
            {
                even_tail->next=p;
                even_tail=p;
            }
        }
        p=p->next;
        count++;
    }
    if(odd_tail!=NULL)
    {
        odd_tail->next=NULL;
    }
    if(even_tail!=NULL)
    {
        even_tail->next=NULL;
    }
}


/* ---------- 题 4（加分）：链表冒泡排序 ----------
 * 要求：【只交换节点，不交换 data 的值】
 *       （面试常问："如果节点数据很大，交换值代价高，怎么办？" -> 交换指针）
 *
 * 思路（交换节点的经典写法）：
 *   用哑节点 + 三个指针 prev、cur、next
 *   发现 cur->data > next->data 时，调整三个指针让 cur 和 next 互换位置：
 *
 *       prev->next = next;
 *       cur->next  = next->next;
 *       next->next = cur;
 *
 *   然后 prev 前进到 next（因为 next 现在是 cur 前面的那个了）
 *
 * 简化做法：用两重循环，外层表示"还需要几轮"，内层负责比较和交换
 *
 * 边界：空链表、单节点 -> 原样返回
 */
Node *sort_list(Node *head)
{
    if(head==NULL||head->next==NULL)
    {
        return head;
    }

    Node dummy;
    dummy.next=head;
    int swapped;
    do
    {
        swapped=0;
        Node *prev=&dummy;
        Node *cur=prev->next;
        while(cur!=NULL&&cur->next!=NULL)
        {
            Node *next=cur->next;
            if(cur->data>next->data)
            {
                prev->next = next;
                cur->next  = next->next;
                next->next = cur;
                swapped=1;
                prev=next;
            }
            else
            {
                prev=cur;
                cur=cur->next;
            }
        }
    }while(swapped==1);
    
    return dummy.next;
}
