# 今天做什么（W4 · 2026-09-28 ~ 10-04）

> 每天 14:30 会弹窗提醒。忘了弹窗内容就看这个文件，或看 `study-daily.md`。

---

## 📌 推送规则（每天必看）

**每晚学习结束前必须 push 一次。** 这是硬性习惯，不是可选项。

| 时机 | 为什么 |
|---|---|
| **每晚学习结束时**（最重要） | 电脑坏了/丢了，这几周的成果全没。GitHub 就是你的备份 |
| **完成一个目标时** | 比如今天「警告清零」，这是一个天然的里程碑 |
| **换电脑或换环境前** | 不然代码分在两台机器上，会冲突 |
| **每次 `commit` 后立刻 `push`** | 别攒着，攒着容易忘 |

**每晚的三条命令（雷打不动）：**

```bash
git add -A
git commit -m "类型: 做了什么"
git push
```

**自检**：跑 `git status`，看到 `Your branch is up to date with 'origin/main'` 就说明推干净了。

---

## 怎么打勾

Markdown 的勾选框语法就是**空格换成字母 x**：

```markdown
- [ ] 没完成      ← 中括号里是空格
- [x] 已完成      ← 中括号里是字母 x
```

在 VS Code 里更方便：**鼠标点一下那个方框**就自动打勾了
（需要 Markdown 预览模式：按 `Ctrl+Shift+V` 打开预览）。

---

## 当前进度

**验收标准：`check-all` 显示「零警告 55」**（55 = 53 个练习 + 2 个样板文件）

| 指标 | 起始 | 现在 | 目标 |
|---|---|---|---|
| 零警告文件 | 46 | **55** ✅ | 55 |
| 警告条数 | 15 | **0** ✅ | 0 |

**警告清零情况（全部完成）：**

- [x] `example\W3-Day2-pointer-array-relationship.c` — 8 条 ✅ 已清零
- [x] `example\W4-Day1-pointer-myMemcpy.c` — 1 条（**真 bug**）✅
- [x] `w3-pointer-toolbox\main.c` — 2 条 ✅
- [x] `example\W2-Day2-reverse-number.c` — 1 条 ✅
- [x] `example\W3-Day3-pointer-const.c` — 1 条 ✅
- [x] `example\W3-Day4-pointer-strcpy.c` — 1 条 ✅
- [x] `w3-string-processor\main.c` — 1 条 ✅

---

## 本周三个动作

1. **清零 15 条警告**（7 个文件）—— 已完成 8 条
2. **读 `projects/multi-file-template/`**，理解多文件工程怎么组织
3. **自己写一个 Makefile**，把 `w3-pointer-toolbox` 拆成多文件

---

## Day 1（周一 09-28）：先学会用工具，不动代码 ✅ 已完成

今天只做一件事：**学会用 `chk` 和 `b` 这两个命令。**

- [x] 打开终端，`cd` 到 `c-learning`
- [x] 跑 `chk example\W2-Day2-prime-number.c` → 显示 **clean, zero warnings**
- [x] 跑 `chk example\W3-Day2-pointer-array-relationship.c` → 显示警告
- [x] 跑 `b example\W3-Day1-function-isprime.c`，输入 `17` → 输出 `17是素数`
- [x] 跑 `b example\W2-Day2-reverse-number.c` → **停下不运行**，指出第 8 行有问题

**额外完成**：把仓库目录加进 PATH（以后 `chk` 不用加 `.\`）；统一换行符策略；两次 git 提交并推送成功。

**验收**：能说出 `chk`（只检查）和 `b`（检查通过才编译运行）的区别。

---

## Day 2（周二 09-29）：修 3 个文件（10 条警告）

用 `chk <文件>` 逐条看，改一条重跑一次。

- [x] `example\W3-Day2-pointer-array-relationship.c` — **8 条** `-Wformat=` ✅
  - `(void *)(arr + 1)` — 注意括号要包住整个算式，因为 `(void *)` 优先级高于 `+`
- [x] `example\W4-Day1-pointer-myMemcpy.c` — **1 条** `-Wdiscarded-qualifiers` ✅
  - 提示：`char *s = (const char*)src;` 把 `const` 丢了。
  - 正确写法：`const char *s = (const char *)src;`
  - **这是真 bug**：在单片机上，字符串常量放在只读区，往那里写会直接崩溃（HardFault）
- [x] `example\W3-Day4-pointer-strcpy.c` — **1 条** `-Wparentheses` ✅
  - 提示：`while (*dest++ = *src++);` 外面再加一层括号：`while ((*dest++ = *src++))`

**正确的写法对照在 `projects\multi-file-template\my_string.c`**，可以打开对着看。

**今天的验收**：这 3 个文件跑 `chk` 都是 clean。

---

## Day 3（周三 09-30）：修 2 个真 bug ✅ 已完成（警告也已全部清零）

- [x] `example\W2-Day2-reverse-number.c` — 1 条 ✅ `-Wunused-variable`
  - 第 8 行 `int ret=0;` 声明了从没用过 → 删掉
- [x] `example\W3-Day3-pointer-const.c` — 1 条 ✅ `-Wunused-but-set-variable`
  - `p1` 只赋值没使用 → 在 printf 里用一下，或改成 `(void)p1;`
- [x] `w3-pointer-toolbox\main.c` — 2 条 ✅ `-Wparentheses`（第 99、127 行）
  - 和 Day2 的 strcpy 同一个问题
- [x] `w3-string-processor\main.c` — 1 条 ✅ `-Wsign-compare`（第 106 行）
  - 有符号数和无符号数比较 → 两边统一成 `int`，或给 `strlen` 的结果加 `(int)`
- [x] **真 bug ①**：`example\W2-Day4-array-char.c` 第 8 行 ✅
  - `scanf("%s", str)` 没有限制宽度，`str` 只有 1000 字节
  - 改成 `scanf("%999s", str)`（留 1 个字节给结尾的 `\0`）
  - 验证：输入 1200 个字符，改之前会崩溃，改之后不会
- [x] **真 bug ②**：同一个文件第 12 行 ✅
  - `count[str[i]-'a']++`，输入大写字母 `ABC` 时下标变成负数 → 越界写
  - 改法：先判断是不是小写字母，或把字母统一转成小写再算

**今天的验收**：跑 `check-all` → **零警告 53**。

---

## Day 4（周四 10-01）：面试题 + 把警告讲明白

- [ ] 牛客 C 面试题 3 道（主题：指针与数组、const 语义）
- [x] 把 15 条警告每一条用自己的话解释 ✅（见 `notes\W4-警告笔记.md`，319 行）
  - 它为什么是警告？不改会出什么问题？
  - 把答案写进 `notes\W4-警告笔记.md`

**今天的验收**：15 条警告，每条你都能说出「不改会怎样」。

---

## Day 5（周五 10-02）：读懂 Makefile

- [x] 打开 `projects\multi-file-template\Makefile`，逐行看懂 ✅
- [x] 在 `projects\multi-file-template` 下依次跑四个命令 ✅
  - `mingw32-make check`（只查警告）
  - `mingw32-make`（编译）
  - `mingw32-make run`（运行）
  - `mingw32-make clean`（清理）
- [x] 打开 `my_string.h`，搞懂 include guard 在防什么 ✅

**今天的验收**：能解释「为什么头文件要有 include guard」和「为什么实现要放 .c 而不是 .h」。

---

## Day 6（周六 10-03）：自己拆一个多文件工程

- [x] 把 `w3-pointer-toolbox\main.c` 拆成三个文件 ✅（`w4-my-toolbox\`）
  - `my_string.h`（声明）
  - `my_string.c`（实现）
  - `main.c`（测试代码）
- [x] 照样板写一个自己的 Makefile ✅（修了 8 处错误后跑通）
- [x] `mingw32-make check` 零警告 ✅

**今天的验收**：`mingw32-make` 一条命令能编译出可执行文件。

---

## Day 7（周日 10-04）：复盘 + 提交

- [x] 跑 `check-all`，确认零警告 55 ✅ 已达成
- [ ] 写 `notes\W4-复盘.md`（3-5 条：哪条警告最难懂？为什么？）
- [ ] Git 提交：
  ```
  git add -A
  git commit -m "W4: 编译警告清零 + 多文件工程与 Makefile"
  git push
  ```
- [ ] 确认三件套已下单（逻辑分析仪、CAN 模块 ×2、第二块 STM32F103）

**本周结束的标志**：`check-all` 显示「零警告 55」✅ 已达成 + 两个真 bug 已修 + 多文件工程已拆好。

---

## 常用命令速查

```powershell
.\chk example\W2-Day2-prime-number.c      # 只查警告
.\b   example\W2-Day2-prime-number.c      # 编译并运行
powershell -ExecutionPolicy Bypass -File tools\check-all.ps1   # 查全部 53 个文件
```
