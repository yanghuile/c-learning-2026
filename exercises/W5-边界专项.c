/* ============================================================
 *  W5 回炉：边界情况专项（2026-10-11 周日）
 *
 *  为什么有这个练习：
 *     你本周同一类 bug 出现了 3 次 —— 都是"某个指针还是 NULL /
 *     已经走到末尾，但代码没考虑"。
 *
 *  这个练习不是考试，是"建立条件反射"：
 *     每看到一行解引用，先问「这个指针可能是 NULL 吗？」
 *
 *  怎么做：
 *     1. 每题先【写出你的判断】（对 / 错 + 原因），不要运行
 *     2. 全部写完，运行 `b exercises/W5-边界专项.c`
 *     3. 对照输出，看有没有你看漏的
 *
 *  本文件零警告，可放心用 chk 检查。
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>     /* malloc / free */

typedef struct Node
{
    int          data;
    struct Node *next;
} Node;

/* ---------- 下面 8 个函数：每个都藏着一个问题 ----------
 * 你的任务：判断它是【正确】还是【有问题】，有问题的话指出【哪里会崩/死循环】
 * 然后在 printf 后写上你的判断
 */

/* 【1】求链表长度 */
static int f1(Node *head)
{
    int n = 0;
    while (head->next != NULL)
    {
        n++;
        head = head->next;
    }
    return n;
}

/* 【2】在链表末尾插入 */
static void f2(Node **head, int v)
{
    Node *p = *head;
    while (p->next != NULL) p = p->next;
    Node *n = (Node *)malloc(sizeof(Node));
    n->data = v;
    n->next = NULL;
    p->next = n;
}

/* 【3】删除第一个值为 v 的节点 */
static void f3(Node **head, int v)
{
    Node *cur = *head;
    Node *prev = NULL;
    while (cur != NULL && cur->data != v)
    {
        prev = cur;
        cur = cur->next;
    }
    prev->next = cur->next;      /* 跳过 cur */
    free(cur);
}

/* 【4】打印链表 */
static void f4(Node *head)
{
    Node *p = head;
    do
    {
        printf("%d ", p->data);
        p = p->next;
    } while (p != NULL);
    printf("\n");
}

/* 【5】释放整条链表 */
static void f5(Node **head)
{
    Node *p = *head;
    while (p != NULL)
    {
        free(p);
        p = p->next;
    }
    *head = NULL;
}

/* 【6】找倒数第 k 个 */
static Node *f6(Node *head, int k)
{
    Node *fast = head;
    for (int i = 0; i < k; i++) fast = fast->next;
    Node *slow = head;
    while (fast != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

/* 【7】合并两个有序链表 */
static Node *f7(Node *a, Node *b)
{
    Node  dummy;
    Node *tail = &dummy;
    while (a != NULL && b != NULL)
    {
        if (a->data <= b->data) { tail->next = a; a = a->next; }
        else                    { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}

/* 【8】判断回文（只比较前半和后半的值） */
static int f8(Node *head)
{
    if (head == NULL) return 1;
    Node *slow = head;
    Node *fast = head;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return (slow->data == head->data) ? 1 : 0;
}

/* ============================================================ */

int main(void)
{
    printf("========== 边界情况专项 ==========\n\n");

    printf("请对下面 8 个函数逐个判断（正确 / 有问题）。\n");
    printf("判断写在代码上方的注释里，然后运行本程序看答案分析。\n\n");

    printf("--- 你的判断（写下来再往下看）---\n");
    printf("  f1: 有问题，head可能是空，那么第一次就解引用空指针，程序会崩溃\n");
    printf("  f2: 有问题，如果是空链表，*head是NULL，p->next解引用空指针，程序崩溃");
    printf("  f3: 有问题，1.如果是空链表，循环不会进入，prev=NULL,则prev->next解引用空指针，崩溃；2.如果遍历完没有找到，此时cur==NULL，prev!=NULL，cur->next解引用空指针，崩溃\n");
    printf("  f4: 有问题，do-while循环必须执行一次，如果是空链表，p是NULL，此时p->data崩溃\n");
    printf("  f5: 有问题，如果先free(p),再p=p->next,则free 之后 p 指向的是已释放的内存。应该这样写：先保存 Node *next = p->next; 再 free(p); 再 p = next\n");
    printf("  f6: 有问题，没有判断k是否超出链表长度，如果k大于数组长度，循环没结束fast就已经是NUL，fast=fast->next程序崩溃，而且也没有判断k是否小于或等于0\n");
    printf("  f7: 正确\n");
    printf("  f8: 有问题，只比较了 slow->data 和 head->data ，没有比较完整条链表\n");
    printf("\n");

    /* ============================================================
     * 下面是【答案分析】—— 先自己判断完再看
     * ============================================================ */
    printf("========== 答案分析 ==========\n\n");

    printf("【1】求链表长度 —— 有问题\n");
    printf("     while (head->next != NULL)：如果 head 是 NULL，第一次就解引用空指针。\n");
    printf("     正确写法：while (head != NULL) { n++; head = head->next; }\n");
    printf("     或者先判断 if (head == NULL) return 0;\n");
    printf("     额外问题：这样数出来的 n 比实际少 1（没数最后一个节点）。\n\n");

    printf("【2】末尾插入 —— 有问题\n");
    printf("     Node *p = *head; while (p->next != NULL)：空链表时 *head 是 NULL，\n");
    printf("     p->next 直接解引用空指针 → 崩溃。\n");
    printf("     正确写法：先判断 if (*head == NULL) { *head = n; return; }\n");
    printf("     ★ 这正是你 W5 Day2 踩过的坑（insert_tail）\n\n");

    printf("【3】按值删除 —— 有问题（两个）\n");
    printf("     问题 a：如果链表为空（cur == NULL），prev 也是 NULL，\n");
    printf("             prev->next 解引用空指针 → 崩溃。\n");
    printf("     问题 b：如果没找到（cur == NULL 但 prev 非 NULL），\n");
    printf("             cur->next 解引用空指针 → 崩溃。\n");
    printf("     正确写法：找到之后要先 if (cur == NULL) return;\n");
    printf("             并且 if (prev == NULL) *head = cur->next; else prev->next = cur->next;\n\n");

    printf("【4】打印链表 —— 有问题\n");
    printf("     用了 do-while：先执行循环体再判断条件。\n");
    printf("     空链表时，p 是 NULL，第一轮就 printf(\"%%d\", p->data) → 崩溃。\n");
    printf("     正确写法：用 while (p != NULL) 或先判断空链表。\n");
    printf("     记住：do-while 至少执行一次，用它处理链表要格外小心。\n\n");

    printf("【5】释放整条链表 —— 有问题\n");
    printf("     free(p); p = p->next;  —— free 之后 p 指向的是已释放的内存，\n");
    printf("     再读 p->next 是【未定义行为】（可能是垃圾值 → 崩溃或死循环）。\n");
    printf("     正确写法：先保存 Node *next = p->next; 再 free(p); 再 p = next;\n");
    printf("     ★ 你 W5 写对过这个（free_list），要保持\n\n");

    printf("【6】倒数第 k 个 —— 有问题\n");
    printf("     for (int i = 0; i < k; i++) fast = fast->next;\n");
    printf("     k 大于链表长度时，fast 中途变成 NULL，再 fast->next 就崩了。\n");
    printf("     另外 k <= 0 也没处理。\n");
    printf("     正确写法：循环里判断 if (fast == NULL) return NULL;\n");
    printf("     ★ 你 W5 写对过（find_kth_from_end），而且写法更简洁\n\n");

    printf("【7】合并两个有序链表 —— 正确！\n");
    printf("     用哑节点 dummy，tail 始终有效，不会解引用空指针。\n");
    printf("     循环条件用 && 保证两个都不为空，循环外统一处理剩余部分。\n");
    printf("     这是链表题里「哑节点」的典型用法。\n\n");

    printf("【8】判断回文 —— 有问题（逻辑完全不对）\n");
    printf("     它只比较了 slow->data 和 head->data 两个值，\n");
    printf("     根本没比较整条链表，这不是回文判断。\n");
    printf("     另外 while (fast->next != NULL && ...) 在单节点时：\n");
    printf("     fast 非空，fast->next 是 NULL，第一个条件为假，循环不执行，\n");
    printf("     slow 停在 head，所以单节点恰好返回 1（歪打正着）。\n");
    printf("     正确思路：找中点 → 反转后半段 → 逐个比较 → 再反转回去。\n\n");

    printf("========== 总结 ==========\n\n");
    printf("8 个函数里有 7 个有边界 bug，全部属于同一类：\n");
    printf("  某个指针还是 NULL，或已经走到末尾，但代码直接解引用了。\n\n");
    printf("防护规则（记住这一条就够）：\n");
    printf("  每写一行 p->xxx，先在上一行问：「p 可能是 NULL 吗？」\n");
    printf("  可能 → 前面必须有判断或保证；\n");
    printf("  不可能 → 说得出为什么不可能。\n\n");
    printf("三个必查的边界：\n");
    printf("  1. 空链表（head == NULL）\n");
    printf("  2. 只有一个节点\n");
    printf("  3. 操作的是头节点 / 尾节点\n");

    return 0;
}
