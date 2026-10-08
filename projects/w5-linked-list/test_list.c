/* ============================================================
 *  test_list.c —— 链表库的测试
 *
 *  运行：mingw32-make test
 *
 *  说明：这是一个"简易测试框架" —— 用一个计数器统计通过/失败，
 *        最后打印汇总。比手写 printf 对比更清楚。
 * ============================================================ */

#include <stdio.h>
#include "list.h"

/* ---------- 极简测试框架 ---------- */
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) { g_pass++; }                                 \
        else { g_fail++; printf("  [FAIL] %s  (第 %d 行)\n", msg, __LINE__); } \
    } while (0)

#define SECTION(name) printf("\n--- %s ---\n", name)

int main(void)
{
    printf("========== list.c 测试 ==========\n");

    /* ==========================================================
     * 1. 创建、插入、长度
     * ========================================================== */
    SECTION("插入与长度");
    {
        Node *h = NULL;
        CHECK(list_length(h) == 0, "空链表长度为 0");

        insert_head(&h, 20);
        insert_head(&h, 10);            /* 10 -> 20 */
        CHECK(list_length(h) == 2, "头插后长度为 2");
        CHECK(h->data == 10, "头插后第一个是 10");

        insert_tail(&h, 30);            /* 10 -> 20 -> 30 */
        insert_tail(&h, 40);            /* 10 -> 20 -> 30 -> 40 */
        CHECK(list_length(h) == 4, "尾插后长度为 4");
        CHECK(find(h, 40) != NULL, "能查到 40");
        CHECK(find(h, 40)->next == NULL, "40 是最后一个节点");

        insert_at(&h, 0, 5);            /* 5 -> 10 -> 20 -> 30 -> 40 */
        CHECK(h->data == 5, "下标 0 插入后头部是 5");
        insert_at(&h, 5, 50);           /* 尾部 */
        CHECK(list_length(h) == 6, "末尾插入后长度为 6");
        insert_at(&h, 99, 999);         /* 越界 */
        CHECK(list_length(h) == 6, "越界插入不改变长度");

        free_list(&h);
        CHECK(h == NULL, "释放后 head 为 NULL");
    }

    /* ==========================================================
     * 2. 删除（含边界）
     * ========================================================== */
    SECTION("删除");
    {
        int arr[] = {10, 20, 30, 40};
        Node *h = list_from_array(arr, 4);

        CHECK(delete_value(&h, 30) == 1, "删中间 30 成功");
        CHECK(list_length(h) == 3, "删后长度为 3");
        CHECK(find(h, 30) == NULL, "30 已被删除");

        CHECK(delete_value(&h, 10) == 1, "删头节点 10 成功");
        CHECK(h->data == 20, "新头是 20");

        CHECK(delete_value(&h, 99) == 0, "删不存在的值返回 0");
        CHECK(delete_value(&h, 20) == 1, "删 20 成功");
        CHECK(delete_value(&h, 40) == 1, "删最后一个 40 成功");
        CHECK(h == NULL, "删空后 head 为 NULL");
        CHECK(delete_value(&h, 1) == 0, "空链表删除返回 0");

        free_list(&h);
    }

    /* ==========================================================
     * 3. 快慢指针
     * ========================================================== */
    SECTION("快慢指针：中间节点");
    {
        int odd[]  = {1, 2, 3, 4, 5};
        int even[] = {1, 2, 3, 4};
        int one[]  = {7};

        Node *ho = list_from_array(odd, 5);
        Node *he = list_from_array(even, 4);
        Node *h1 = list_from_array(one, 1);

        CHECK(find_middle(ho)->data == 3, "5 个节点中间是 3");
        CHECK(find_middle(he)->data == 3, "4 个节点中间是 3（后中间）");
        CHECK(find_middle(h1)->data == 7, "单节点中间是自己");
        CHECK(find_middle(NULL) == NULL, "空链表返回 NULL");

        free_list(&ho); free_list(&he); free_list(&h1);
    }

    SECTION("快慢指针：倒数第 k 个");
    {
        int arr[] = {1, 2, 3, 4, 5};
        Node *h = list_from_array(arr, 5);

        CHECK(find_kth_from_end(h, 1)->data == 5, "倒数第 1 个是 5");
        CHECK(find_kth_from_end(h, 2)->data == 4, "倒数第 2 个是 4");
        CHECK(find_kth_from_end(h, 5)->data == 1, "倒数第 5 个是 1");
        CHECK(find_kth_from_end(h, 6) == NULL, "倒数第 6 个（超长度）返回 NULL");
        CHECK(find_kth_from_end(h, 0) == NULL, "k=0 返回 NULL");
        CHECK(find_kth_from_end(h, -1) == NULL, "k 为负数返回 NULL");
        CHECK(find_kth_from_end(NULL, 1) == NULL, "空链表返回 NULL");

        free_list(&h);
    }

    SECTION("快慢指针：判环");
    {
        int arr[] = {1, 2, 3, 4};
        Node *h = list_from_array(arr, 4);
        CHECK(has_cycle(h) == 0, "无环返回 0");
        CHECK(has_cycle(NULL) == 0, "空链表返回 0");

        /* 造环：尾巴指回第 2 个 */
        Node *tail = h;
        while (tail->next != NULL) tail = tail->next;
        tail->next = h->next;
        CHECK(has_cycle(h) == 1, "有环返回 1");

        tail->next = NULL;              /* 拆环才能安全释放 */
        free_list(&h);
    }

    /* ==========================================================
     * 4. 反转
     * ========================================================== */
    SECTION("反转");
    {
        int arr[] = {1, 2, 3, 4, 5};
        Node *h = list_from_array(arr, 5);

        h = reverse_iter(h);
        CHECK(h->data == 5, "迭代反转后头是 5");
        CHECK(list_length(h) == 5, "迭代反转后长度不变");
        CHECK(find(h, 1)->next == NULL, "1 变成尾节点");

        h = reverse_rec(h);
        CHECK(h->data == 1, "再递归反转后头变回 1");
        CHECK(find(h, 5)->next == NULL, "5 变成尾节点");

        CHECK(reverse_iter(NULL) == NULL, "空链表反转返回 NULL");

        Node *one = list_from_array(arr, 1);
        Node *r   = reverse_iter(one);
        CHECK(r->data == 1 && r->next == NULL, "单节点反转不变");
        free_list(&r);

        free_list(&h);
    }

    SECTION("每 k 个一组反转");
    {
        int arr[] = {1, 2, 3, 4, 5};
        Node *h = list_from_array(arr, 5);

        h = reverse_k(h, 2);            /* 2 -> 1 -> 4 -> 3 -> 5 */
        CHECK(h->data == 2, "k=2 后头是 2");
        CHECK(h->next->data == 1, "k=2 第二个是 1");
        CHECK(h->next->next->data == 4, "k=2 第三个是 4");
        CHECK(find(h, 5)->next == NULL, "5 仍是尾节点（不足 k 个保持原样）");
        free_list(&h);

        Node *h2 = list_from_array(arr, 5);
        h2 = reverse_k(h2, 3);          /* 3 -> 2 -> 1 -> 4 -> 5 */
        CHECK(h2->data == 3, "k=3 后头是 3");
        CHECK(h2->next->next->data == 1, "k=3 第三个是 1");
        CHECK(h2->next->next->next->data == 4, "k=3 第四个是 4（保持原样）");
        free_list(&h2);

        Node *h3 = list_from_array(arr, 5);
        h3 = reverse_k(h3, 1);
        CHECK(h3->data == 1, "k=1 不反转");
        free_list(&h3);
    }

    /* ==========================================================
     * 5. 合并与回文
     * ========================================================== */
    SECTION("合并有序链表");
    {
        int a[] = {1, 3, 5};
        int b[] = {2, 4, 6};
        Node *ha = list_from_array(a, 3);
        Node *hb = list_from_array(b, 3);

        Node *m = merge_sorted(ha, hb);
        CHECK(list_length(m) == 6, "合并后长度为 6");
        CHECK(m->data == 1, "第一个是 1");
        CHECK(find(m, 6)->next == NULL, "最后一个是 6");
        /* 验证有序 */
        int ok = 1;
        for (Node *p = m; p->next != NULL; p = p->next)
            if (p->data > p->next->data) { ok = 0; break; }
        CHECK(ok, "合并后仍然升序");
        free_list(&m);

        Node *h1 = list_from_array(a, 3);
        Node *m2 = merge_sorted(h1, NULL);
        CHECK(list_length(m2) == 3, "和空链表合并长度不变");
        free_list(&m2);

        CHECK(merge_sorted(NULL, NULL) == NULL, "两个空链表合并返回 NULL");
    }

    SECTION("回文判断");
    {
        int p1[] = {1, 2, 3, 2, 1};
        int p2[] = {1, 2, 2, 1};
        int p3[] = {1, 2, 3, 4};
        int p4[] = {1};

        Node *a = list_from_array(p1, 5);
        Node *b = list_from_array(p2, 4);
        Node *c = list_from_array(p3, 4);
        Node *d = list_from_array(p4, 1);

        CHECK(is_palindrome(a) == 1, "1-2-3-2-1 是回文");
        CHECK(list_length(a) == 5, "判断后链表长度未变（已恢复）");
        CHECK(is_palindrome(b) == 1, "1-2-2-1 是回文");
        CHECK(is_palindrome(c) == 0, "1-2-3-4 不是回文");
        CHECK(is_palindrome(d) == 1, "单节点是回文");
        CHECK(is_palindrome(NULL) == 1, "空链表是回文");

        free_list(&a); free_list(&b); free_list(&c); free_list(&d);
    }

    /* ==========================================================
     * 汇总
     * ========================================================== */
    printf("\n========== 测试汇总 ==========\n");
    printf("  通过: %d\n", g_pass);
    printf("  失败: %d\n", g_fail);
    if (g_fail == 0) printf("  全部通过！\n");
    else             printf("  有失败项，见上面 [FAIL] 行\n");

    return (g_fail == 0) ? 0 : 1;
}
