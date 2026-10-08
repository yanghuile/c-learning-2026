# 单链表工具库（W5）

用 C 实现的单链表数据结构库。W5 的学习产出，也是第一个**可复用的多文件工程**。

---

## 一、快速开始

```bash
cd projects/w5-linked-list

mingw32-make          # 编译并运行测试
mingw32-make check    # 只检查警告（不生成可执行文件）
mingw32-make lib      # 打包成静态库 liblist.a
mingw32-make clean    # 清理 build/
```

> 在你的环境里 GNU Make 叫 `mingw32-make`，不是 `make`。

### 在自己的代码里用它

```c
#include "list.h"

int main(void)
{
    Node *head = NULL;

    insert_tail(&head, 10);
    insert_tail(&head, 20);
    insert_head(&head, 5);          /* 5 -> 10 -> 20 */

    print_list(head);

    head = reverse_iter(head);      /* 20 -> 10 -> 5 */
    print_list(head);

    free_list(&head);               /* 必须释放，否则内存泄漏 */
    return 0;
}
```

编译（把 `list.c` 一起编）：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c list.c -o main
```

或用静态库：

```bash
mingw32-make lib
gcc -std=c11 main.c -Lbuild -llist -o main
```

---

## 二、设计约定

| 项 | 约定 |
|---|---|
| 链表类型 | **单链表** |
| 头节点 | **不带头节点**（第一个节点就是数据节点） |
| 空链表表示 | `head == NULL` |
| 下标 | 从 **0** 开始 |
| 失败返回值 | 返回 `int` 的函数：成功 `1` / 失败 `0`；返回指针的函数：失败 `NULL` |
| 内存管理 | 用 `malloc`/`free`，**用完后必须调用 `free_list`** |

### 为什么插入类函数要用 `Node **head`

因为**空链表上插入**时，`head` 本身要从 `NULL` 变成指向新节点 —— 而 C 是值传递，只有传 `head` 的**地址**（二级指针）才能改掉调用方的 `head`。

```c
insert_head(&head, 5);      /* 传地址 */
```

---

## 三、接口一览（共 16 个）

### 生命周期

| 函数 | 说明 |
|---|---|
| `Node *create_node(int data)` | 创建节点，失败返回 `NULL` |
| `void free_list(Node **head)` | 释放整条链表，并把 `*head` 置 `NULL` |

### 插入

| 函数 | 说明 | 复杂度 |
|---|---|---|
| `void insert_head(Node **head, int data)` | 头插 | O(1) |
| `void insert_tail(Node **head, int data)` | 尾插 | O(n) |
| `int insert_at(Node **head, int pos, int value)` | 在下标 `pos` 插入 | O(n) |

### 删除

| 函数 | 说明 | 复杂度 |
|---|---|---|
| `int delete_value(Node **head, int value)` | 删除第一个值为 `value` 的节点 | O(n) |

### 查找与统计

| 函数 | 说明 | 复杂度 |
|---|---|---|
| `Node *find(Node *head, int value)` | 按值查找 | O(n) |
| `int list_length(Node *head)` | 求长度 | O(n) |
| `Node *find_middle(Node *head)` | 中间节点（快慢指针） | O(n) |
| `Node *find_kth_from_end(Node *head, int k)` | 倒数第 k 个（快慢指针，**只遍历一次**） | O(n) |
| `int has_cycle(Node *head)` | 判断有环（Floyd 判环） | O(n) |

### 反转

| 函数 | 说明 | 空间 |
|---|---|---|
| `Node *reverse_iter(Node *head)` | 迭代法反转 | O(1) |
| `Node *reverse_rec(Node *head)` | 递归法反转 | O(n) 递归栈 |
| `Node *reverse_k(Node *head, int k)` | 每 k 个一组反转，不足 k 个保持原样 | O(1) |

### 合并与判断

| 函数 | 说明 | 空间 |
|---|---|---|
| `Node *merge_sorted(Node *a, Node *b)` | 合并两个有序链表（复用原节点） | O(1) |
| `int is_palindrome(Node *head)` | 判断回文（快慢指针 + 反转后半段） | O(1) |

### 输出与便利函数

| 函数 | 说明 |
|---|---|
| `void print_list(Node *head)` | 打印，格式 `1 -> 2 -> 3 -> NULL` |
| `Node *list_from_array(const int *arr, int n)` | 从数组建链表（测试用） |

---

## 四、两个关键算法

### 快慢指针

```
slow 每次走 1 步，fast 每次走 2 步

用途一：找中间节点
    1 -> 2 -> 3 -> 4 -> 5 -> NULL
              slow         fast(到 NULL)
    fast 走完全程时，slow 正好在一半位置

用途二：判断有环
    有环时 fast 会在环里追上 slow（因为每轮快 1 步）
    ⚠️ 循环条件必须同时判 fast 和 fast->next，否则解引用空指针
```

### 迭代法反转（面试必考）

```
prev=NULL, cur=head
while (cur != NULL) {
    next = cur->next;      // ① 先保存下一个
    cur->next = prev;      // ② 掉头
    prev = cur;            // ③ 前移
    cur = next;
}
return prev;               // 返回新头（不是 cur，因为 cur 已经是 NULL）
```

**四个易错点**：① 保存 `next` 必须在掉头之前；② 返回 `prev`；③ 两个前移的顺序；④ 空链表也要能处理。

---

## 五、已知边界（测试已覆盖）

| 场景 | 预期行为 |
|---|---|
| 空链表 `head == NULL` | 长度 0、遍历不崩溃、反转返回 `NULL`、回文返回 1 |
| 单节点链表 | 中间是自己、反转后不变 |
| 两个节点 | 反转后交换 |
| 删头节点 | `*head` 正确后移 |
| 删唯一节点 | 删完 `head == NULL` |
| 插到下标 0 | 改 `*head` |
| 插到下标 == 长度 | 等价于尾插 |
| 下标越界 | 返回 0，链表不变 |
| `k <= 0` 或 `k > 长度` | `find_kth_from_end` 返回 `NULL` |
| 不足 k 个节点 | `reverse_k` 保持原样 |

---

## 六、目录结构

```
w5-linked-list/
├── list.h          接口声明（别人只需要看这个）
├── list.c          实现
├── test_list.c     测试（含边界情况）
├── Makefile        build 规则
├── README.md       本文件
└── build/          编译产物（已 gitignore）
```

---

## 七、演进记录

| 时间 | 变化 |
|---|---|
| W5 周二 | 在 `exercises/W5-Day2-链表基础.c` 里写基础 6 个函数 |
| W5 周三 | 在 `exercises/W5-Day3-链表进阶.c` 里写进阶 6 个函数 |
| W5 周四 | 在 `exercises/W5-Day4-链表反转.c` 里写反转与面试题 |
| W5 周五 | **抽成库**：`.h`/`.c` 分离 + Makefile + 测试 → 本工程 |

**教训记录**：`insert_tail` 最初漏了"空链表"的情况（新节点被丢弃 + 内存泄漏）。
**这就是本工程把大量边界情况写进测试的原因。**
