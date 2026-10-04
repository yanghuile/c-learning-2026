# 今天做什么（W4 · 2026-09-28 ~ 10-04）

> 每天 14:30 会弹窗提醒。忘了弹窗内容就看这个文件，或看 `study-daily.md`。

---

## 📁 新文件放哪（动手前先看一眼）

> 2026-10-04 重组了仓库结构。**以后新建任何文件，先照这张表判断放哪**，否则结构又会乱。

| 你要建的东西 | 放这里 | 判断标准 | 命名 |
|---|---|---|---|
| **单文件练习** | `exercises/` | 一个 `.c` 就能编译运行 | `W<周>-Day<天>-<主题>.c` |
| **周次专题练习** | `exercises/` | 一个 `.c`，但是整周的综合题 | `W<周>-<主题>.c` |
| **多文件工程** | `projects/<周次>-<名字>/` | 有 `.h`/`.c` 分离，或有 Makefile | 目录 `w<周>-<小写连字符>` |
| **笔记 / 复盘** | `notes/` | 是 `.md` 文件 | `W<周>-<主题>.md` |
| **工具脚本** | `tools/` | 不是 C 代码（`.ps1`/`.cmd`） | 小写连字符 |

### 三个反例（不要这么做）

| ❌ 错误做法 | 为什么错 | ✅ 正确做法 |
|---|---|---|
| 在根目录建 `W6-链表.c` | 根目录只放 `README.md`/`today.md` | 放 `exercises/W6-链表.c` |
| 把 `.c` 练习放 `notes/` | `notes/` 只放 `.md` | 放 `exercises/` |
| 在 `projects/` 下建单个 `.c` | `projects/` 只放"需要多文件协作"的 | 单个 `.c` 放 `exercises/` |

### 分层原则（为什么这样分）

| 目录 | 唯一原则 |
|---|---|
| `exercises/` | **单文件**能跑（学习痕迹） |
| `projects/` | **多文件**协作（作品） |
| `samples/` | **不是你写的**（参考） |
| `notes/` | **是 `.md`**（思考） |
| `tools/` | **不是 C 代码**（工具） |

**任何文件都能用这 5 条判断该放哪。**

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

**验收标准：`check-all` 显示零警告**

| 指标 | 起始 | 现在 | 目标 |
|---|---|---|---|
| 零警告文件 | 46 | **62** ✅ | 全部 |
| 警告条数 | 15 | **0** ✅ | 0 |

**警告清零情况（全部完成）：**

- [x] `exercises\W3-Day2-pointer-array-relationship.c` — 8 条 ✅ 已清零
- [x] `exercises\W4-Day1-pointer-myMemcpy.c` — 1 条（**真 bug**）✅
- [x] `projects\projects/w3-pointer-toolbox\main.c` — 2 条 ✅
- [x] `exercises\W2-Day2-reverse-number.c` — 1 条 ✅
- [x] `exercises\W3-Day3-pointer-const.c` — 1 条 ✅
- [x] `exercises\W3-Day4-pointer-strcpy.c` — 1 条 ✅
- [x] `projects\projects/w3-string-processor\main.c` — 1 条 ✅

---

## 本周三个动作

1. **清零 15 条警告**（7 个文件）—— 已完成 8 条
2. **读 `samples/multi-file-template/`**，理解多文件工程怎么组织
3. **自己写一个 Makefile**，把 `projects/w3-pointer-toolbox` 拆成多文件

---

## Day 1（周一 09-28）：先学会用工具，不动代码 ✅ 已完成

今天只做一件事：**学会用 `chk` 和 `b` 这两个命令。**

- [x] 打开终端，`cd` 到 `c-learning`
- [x] 跑 `chk exercises\W2-Day2-prime-number.c` → 显示 **clean, zero warnings**
- [x] 跑 `chk exercises\W3-Day2-pointer-array-relationship.c` → 显示警告
- [x] 跑 `b exercises\W3-Day1-function-isprime.c`，输入 `17` → 输出 `17是素数`
- [x] 跑 `b exercises\W2-Day2-reverse-number.c` → **停下不运行**，指出第 8 行有问题

**额外完成**：把仓库目录加进 PATH（以后 `chk` 不用加 `.\`）；统一换行符策略；两次 git 提交并推送成功。

**验收**：能说出 `chk`（只检查）和 `b`（检查通过才编译运行）的区别。

---

## Day 2（周二 09-29）：修 3 个文件（10 条警告）

用 `chk <文件>` 逐条看，改一条重跑一次。

- [x] `exercises\W3-Day2-pointer-array-relationship.c` — **8 条** `-Wformat=` ✅
  - `(void *)(arr + 1)` — 注意括号要包住整个算式，因为 `(void *)` 优先级高于 `+`
- [x] `exercises\W4-Day1-pointer-myMemcpy.c` — **1 条** `-Wdiscarded-qualifiers` ✅
  - 提示：`char *s = (const char*)src;` 把 `const` 丢了。
  - 正确写法：`const char *s = (const char *)src;`
  - **这是真 bug**：在单片机上，字符串常量放在只读区，往那里写会直接崩溃（HardFault）
- [x] `exercises\W3-Day4-pointer-strcpy.c` — **1 条** `-Wparentheses` ✅
  - 提示：`while (*dest++ = *src++);` 外面再加一层括号：`while ((*dest++ = *src++))`

**正确的写法对照在 `samples\multi-file-template\my_string.c`**，可以打开对着看。

**今天的验收**：这 3 个文件跑 `chk` 都是 clean。

---

## Day 3（周三 09-30）：修 2 个真 bug ✅ 已完成（警告也已全部清零）

- [x] `exercises\W2-Day2-reverse-number.c` — 1 条 ✅ `-Wunused-variable`
  - 第 8 行 `int ret=0;` 声明了从没用过 → 删掉
- [x] `exercises\W3-Day3-pointer-const.c` — 1 条 ✅ `-Wunused-but-set-variable`
  - `p1` 只赋值没使用 → 在 printf 里用一下，或改成 `(void)p1;`
- [x] `projects\projects/w3-pointer-toolbox\main.c` — 2 条 ✅ `-Wparentheses`（第 99、127 行）
  - 和 Day2 的 strcpy 同一个问题
- [x] `projects\projects/w3-string-processor\main.c` — 1 条 ✅ `-Wsign-compare`（第 106 行）
  - 有符号数和无符号数比较 → 两边统一成 `int`，或给 `strlen` 的结果加 `(int)`
- [x] **真 bug ①**：`exercises\W2-Day4-array-char.c` 第 8 行 ✅
  - `scanf("%s", str)` 没有限制宽度，`str` 只有 1000 字节
  - 改成 `scanf("%999s", str)`（留 1 个字节给结尾的 `\0`）
  - 验证：输入 1200 个字符，改之前会崩溃，改之后不会
- [x] **真 bug ②**：同一个文件第 12 行 ✅
  - `count[str[i]-'a']++`，输入大写字母 `ABC` 时下标变成负数 → 越界写
  - 改法：先判断是不是小写字母，或把字母统一转成小写再算

**今天的验收**：跑 `check-all` → **零警告**。

---

## Day 4（周四 10-01）：面试题 + 把警告讲明白

- [x] C 面试题：指针与数组、const 语义 ✅（exercises/W4-面试题.c，5 题）
- [x] 把 15 条警告每一条用自己的话解释 ✅（见 `notes\W4-警告笔记.md`，319 行）
  - 它为什么是警告？不改会出什么问题？
  - 把答案写进 `notes\W4-警告笔记.md`

**今天的验收**：15 条警告，每条你都能说出「不改会怎样」。

---

## Day 5（周五 10-02）：读懂 Makefile

- [x] 打开 `samples\multi-file-template\Makefile`，逐行看懂 ✅
- [x] 在 `samples\multi-file-template` 下依次跑四个命令 ✅
  - `mingw32-make check`（只查警告）
  - `mingw32-make`（编译）
  - `mingw32-make run`（运行）
  - `mingw32-make clean`（清理）
- [x] 打开 `my_string.h`，搞懂 include guard 在防什么 ✅

**今天的验收**：能解释「为什么头文件要有 include guard」和「为什么实现要放 .c 而不是 .h」。

---

## Day 6（周六 10-03）：自己拆一个多文件工程

- [x] 把 `projects\projects/w3-pointer-toolbox\main.c` 拆成三个文件 ✅（`projects\projects/w4-my-toolbox\`）
  - `my_string.h`（声明）
  - `my_string.c`（实现）
  - `main.c`（测试代码）
- [x] 照样板写一个自己的 Makefile ✅（修了 8 处错误后跑通）
- [x] `mingw32-make check` 零警告 ✅

**今天的验收**：`mingw32-make` 一条命令能编译出可执行文件。

---

## Day 7（周日 10-04）：复盘 + 提交

- [x] 跑 `check-all`，确认全部零警告（62 个文件）✅
- [x] 写 `notes\W4-复盘.md` ✅
- [x] Git 提交并推送（W4 共 15 个提交）✅
  ```
  git add -A
  git commit -m "W4: 编译警告清零 + 多文件工程与 Makefile"
  git push
  ```
- [x] 三件套已下单 ✅

**本周结束的标志**：`check-all` 显示全部零警告（62 个文件）✅ + 两个真 bug 已修 ✅ + 多文件工程已拆好 ✅

---

## 常用命令速查

```powershell
.\chk exercises\W2-Day2-prime-number.c      # 只查警告
.\b   exercises\W2-Day2-prime-number.c      # 编译并运行
powershell -ExecutionPolicy Bypass -File tools\check-all.ps1   # 查全部 53 个文件
```

---

# W4 完成情况（2026-09-28 ~ 10-04）✅ 全部完成

| 任务 | 状态 |
|---|---|
| 62 个文件编译警告清零（原 15 条） | ✅ |
| 修 2 个真 bug（栈溢出、负数下标越界） | ✅ |
| 建立 chk / b / check-all 工具链 | ✅ |
| 统一换行符策略（.gitattributes） | ✅ |
| 拆多文件工程 + 手写 Makefile | ✅ |
| 警告笔记（319 行） | ✅ |
| W4 复盘 | ✅ |
| 面试题 5 道 | ✅ |
| 三件套下单 | ✅ |

# W5 完成情况（原定 10-05 起，实际提前完成）✅ 全部完成

| 内容 | 验收结果 |
|---|---|
| 位运算 | ✅ 验收 16/17 |
| struct / typedef / enum | ✅ 概念 11/13，sizeof / offsetof / 帧打包全部通过 |
| volatile / static / const / extern | ✅ 概念 4.5/5 |

**待补（已记录，用到时再补）**：
- [ ] **指针回炉验收**：函数指针 + 多级指针 + 指针运算（学生选 B：用到时再补）

# 复盘清单

见 notes/W4-W5-复盘清单.md
