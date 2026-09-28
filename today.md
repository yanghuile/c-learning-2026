# 今天做什么（W4 · 2026-09-28 ~ 10-04）

> 每天 14:30 会弹窗提醒。忘了弹窗内容就看这个文件，或看 `study-daily.md`。

## 本周目标（一句话）

让 `check-all` 显示 **53 个文件、零警告 53**。

## 本周三个动作

1. **清零 15 条警告**（7 个文件）
2. **读 `projects/w4-string-lib/`**，理解多文件工程怎么组织
3. **自己写一个 Makefile**，把 `w3-pointer-toolbox` 拆成多文件

---

## Day 1（周一 09-28）：先学会用工具，不动代码

今天只做一件事：**学会用 `chk` 和 `b` 这两个命令。**

- [ ] 打开终端，`cd` 到 `c-learning`
- [ ] 跑 `.\chk example\W2-Day2-prime-number.c` → 应该显示 **clean, zero warnings**
- [ ] 跑 `.\chk example\W3-Day2-pointer-array-relationship.c` → 应该显示 **8 warnings, need to fix**
- [ ] 跑 `.\b example\W3-Day1-function-isprime.c`，输入 `17` → 应输出 `17是素数`
- [ ] 跑 `.\b example\W2-Day2-reverse-number.c` → 应**停下不运行**，并指出第 8 行有问题

**今天的验收**：你能不看本文，自己说出 `chk` 和 `b` 的区别。

---

## Day 2（周二 09-29）：修 3 个文件（10 条警告）

用 `chk <文件>` 逐条看，改一条重跑一次。

- [ ] `example\W3-Day2-pointer-array-relationship.c` — **8 条** `-Wformat=`
  - 提示：`%p` 要求参数是 `void *`，所以打印指针要写 `(void*)arr`
- [ ] `example\W4-Day1-pointer-myMemcpy.c` — **1 条** `-Wdiscarded-qualifiers`
  - 提示：`char *s = (const char*)src;` 把 `const` 丢了。
  - 正确写法：`const char *s = (const char *)src;`
  - **这是真 bug**：在单片机上，字符串常量放在只读区，往那里写会直接崩溃（HardFault）
- [ ] `example\W3-Day4-pointer-strcpy.c` — **1 条** `-Wparentheses`
  - 提示：`while (*dest++ = *src++);` 外面再加一层括号：`while ((*dest++ = *src++))`

**正确的写法对照在 `projects\w4-string-lib\my_string.c`**，可以打开对着看。

**今天的验收**：这 3 个文件跑 `chk` 都是 clean。

---

## Day 3（周三 09-30）：修剩下 4 个文件 + 修 2 个真 bug

- [ ] `example\W2-Day2-reverse-number.c` — 1 条 `-Wunused-variable`
  - 第 8 行 `int ret=0;` 声明了从没用过 → 删掉
- [ ] `example\W3-Day3-pointer-const.c` — 1 条 `-Wunused-but-set-variable`
  - `p1` 只赋值没使用 → 在 printf 里用一下，或改成 `(void)p1;`
- [ ] `w3-pointer-toolbox\main.c` — 2 条 `-Wparentheses`（第 99、127 行）
  - 和 Day2 的 strcpy 同一个问题
- [ ] `w3-string-processor\main.c` — 1 条 `-Wsign-compare`（第 106 行）
  - 有符号数和无符号数比较 → 两边统一成 `int`，或给 `strlen` 的结果加 `(int)`
- [ ] **真 bug ①**：`example\W2-Day4-array-char.c` 第 8 行
  - `scanf("%s", str)` 没有限制宽度，`str` 只有 1000 字节
  - 改成 `scanf("%999s", str)`（留 1 个字节给结尾的 `\0`）
  - 验证：输入 1200 个字符，改之前会崩溃，改之后不会
- [ ] **真 bug ②**：同一个文件第 12 行
  - `count[str[i]-'a']++`，输入大写字母 `ABC` 时下标变成负数 → 越界写
  - 改法：先判断是不是小写字母，或把字母统一转成小写再算

**今天的验收**：跑 `check-all` → **零警告 53**。

---

## Day 4（周四 10-01）：面试题 + 把警告讲明白

- [ ] 牛客 C 面试题 3 道（主题：指针与数组、const 语义）
- [ ] 打开 `tools\warnings.log`，把这 15 条警告**每一条用自己的话解释**：
  - 它为什么是警告？不改会出什么问题？
  - 把答案写进 `notes\W4-警告笔记.md`

**今天的验收**：15 条警告，每条你都能说出「不改会怎样」。

---

## Day 5（周五 10-02）：读懂 Makefile

- [ ] 打开 `projects\w4-string-lib\Makefile`，逐行看懂
- [ ] 在 `projects\w4-string-lib` 下依次跑：
  - `mingw32-make check`（只查警告）
  - `mingw32-make`（编译）
  - `mingw32-make run`（运行）
  - `mingw32-make clean`（清理）
- [ ] 打开 `my_string.h`，搞懂 include guard（`#ifndef` / `#define` / `#endif`）在防什么

**今天的验收**：能解释「为什么头文件要有 include guard」和「为什么实现要放 .c 而不是 .h」。

---

## Day 6（周六 10-03）：自己拆一个多文件工程

- [ ] 把 `w3-pointer-toolbox\main.c` 拆成三个文件：
  - `my_string.h`（声明）
  - `my_string.c`（实现）
  - `main.c`（测试代码）
- [ ] 照 `projects\w4-string-lib\Makefile` 写一个自己的 Makefile
- [ ] `mingw32-make check` 必须零警告

**今天的验收**：`mingw32-make` 一条命令能编译出可执行文件。

---

## Day 7（周日 10-04）：复盘 + 提交

- [ ] 跑 `check-all`，确认零警告 53
- [ ] 写 `notes\W4-复盘.md`（3-5 条：哪条警告最难懂？为什么？）
- [ ] Git 提交：
  ```
  git add -A
  git commit -m "W4: 编译警告清零 + 多文件工程与 Makefile"
  git push
  ```
- [ ] 确认三件套已下单（逻辑分析仪、CAN 模块 ×2、第二块 STM32F103）

**本周结束的标志**：`check-all` 显示「53 个文件、零警告 53」。

---

## 常用命令速查

```powershell
.\chk example\W2-Day2-prime-number.c      # 只查警告
.\b   example\W2-Day2-prime-number.c      # 编译并运行
powershell -ExecutionPolicy Bypass -File tools\check-all.ps1   # 查全部 53 个文件
```
