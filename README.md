# c-learning

2026 年学习 C 语言的代码仓库。目标：**嵌入式软件方向就业**（大湾区），学习计划详见学习计划表。

---

## 一、目录结构

```
c-learning/
├── example/                    第 2-4 周练习（53 个 .c，按 W2-Day1 规范命名）
├── w1-Calculator/              第 1 周项目：简易计算器
├── w1-temperature-convert/     第 1 周项目：温度转换
├── w2-array-manager/           第 2 周项目：数组综合（成绩统计与排序）
├── w3-pointer-toolbox/         第 3 周项目：指针字符串工具箱
├── w3-string-processor/        第 3 周项目：单词排序与查找
├── projects/
│   └── w4-string-lib/          ★ W4 多文件工程样板（Makefile + .h + .c + 测试）
├── tools/                      编译体检脚本
│   ├── check-all.ps1           检查全部 .c 文件的警告
│   ├── check-one.ps1           检查单个文件
│   └── warnings.log            自动生成的完整诊断报告（已 gitignore）
└── notes/                      学习笔记
```

---

## 二、编译选项（本仓库统一标准）

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic
```

| 选项 | 含义 |
|---|---|
| `-std=c11` | 用 C11 标准，避免编译器默认方言 |
| `-Wall` | 打开所有常用警告（未使用变量、类型不匹配、可疑逻辑等） |
| `-Wextra` | `-Wall` 没覆盖的额外警告 |
| `-Wpedantic` | 严格按 ISO C 标准挑刺 |
| `-g` | 生成调试信息（要打断点时必须加） |
| `-fsyntax-only` | 只检查语法、不生成文件（体检专用，很快） |

**为什么必须加警告选项**：C 语言很多错误编译器默认不吭声，程序照样编过但运行出错。
例如 `char str[1000]; scanf("%s", str);` 输入 1200 字符会栈溢出崩溃，加 `-Wall` 就会提醒没有限制宽度。

---

## 三、日常操作

### 0. 每天最常用的两条命令（推荐先记这两个）

在 `c-learning` 目录下打开 PowerShell，然后：

```powershell
.\chk example\W5-bit-ops.c      # 只检查警告，不运行（改代码时反复用）
.\b   example\W5-bit-ops.c      # 编译并运行（有警告就拒绝运行）
```

⚠️ **前面的 `.\` 不能省。** PowerShell 出于安全考虑不执行当前目录下的程序，
不加就会报 `The term 'chk' is not recognized`。

这两个命令其实只是 `gcc` 的包装：

| 短命令 | 等价于 |
|---|---|
| `.\chk 文件.c` | `gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only 文件.c` |
| `.\b 文件.c` | `gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g 文件.c -o 文件.exe` 然后运行 |

（`b.cmd` / `chk.cmd` 是纯文本文件，可以直接打开看内容，也可以自己改。）

### 1. 检查所有文件有没有警告（每周验收用）

```powershell
powershell -ExecutionPolicy Bypass -File tools\check-all.ps1
```

输出摘要，并把完整诊断写进 `tools/warnings.log`。

> 注意：终端可能把 gcc 的长警告行显示不全，**要看完整的某条警告，打开 `tools/warnings.log`**，或直接手跑 gcc（见下）。

### 2. 检查单个文件

```powershell
powershell -ExecutionPolicy Bypass -File tools\check-one.ps1 example\W2-Day2-prime-number.c
```

或直接手跑（推荐养成习惯，输出最完整）：

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only example\W2-Day2-prime-number.c
```

### 3. 编译并运行单个练习

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -g example\W2-Day2-prime-number.c -o demo.exe
.\demo.exe
```

### 4. 用 gdb 调试

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -g example\W2-Day4-array-char.c -o demo.exe
gdb .\demo.exe
# 常用命令: b main(在main下断点) / run / n(单步) / p 变量(打印) / bt(调用栈) / q(退出)
```

### 5. 多文件工程（用 Makefile）

⚠️ **你的 MinGW 里 `make` 命令叫 `mingw32-make`**，功能完全一样：

```powershell
cd projects\multi-file-template
mingw32-make           # 编译
mingw32-make run       # 编译并运行
mingw32-make check     # 只查警告，不生成文件
mingw32-make clean     # 删除编译产物
```

---

## 四、本仓库的两个已知环境坑

### 坑 1：MinGW 的 printf 不支持 `%zu`

MinGW 底层用的是微软 C 运行时，`printf` 不认 C99 的 `z` 长度修饰符。所以打印 `size_t`：

```c
size_t n = my_strlen(s);
printf("%d\n", (int)n);      /* 正确（Windows/MinGW） */
printf("%zu\n", n);          /* 错误：unknown conversion type character 'z' */
```

Linux/gcc 支持 `%zu`，那段写法在 Linux 上更规范 —— 但目前以能编过为准。

### 坑 2：不要用 PowerShell 的 `2>` 收集 gcc 输出

PowerShell 的 `2>` 会把 gcc 每条诊断**截断到 76 字符**，看不到完整警告。
`tools/check-all.ps1` 内部因此改用 cmd 的 `2>>`。手工收集时也应这样：

```powershell
cmd /c "gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only file.c 2> log.txt"
```

---

## 五、当前进度

| 阶段 | 内容 | 状态 |
|---|---|---|
| W1 | 环境搭建、计算器、温度转换 | 已完成 |
| W2 | 选择 / 循环 / 数组（53 个练习） | 已完成 |
| W3 | 函数、指针、字符串、手写 strcpy | 已完成 |
| W4 | **编译警告清零 + 多文件工程 + Makefile** | 进行中 |
| W5 | 位运算、struct、volatile/static | 未开始 |
| W6–W9 | 数据结构：链表、环形缓冲区、内存池 | 未开始 |
| W10+ | STM32 外设驱动 | 未开始 |

### W4 待办（验收标准：`check-all.ps1` 显示零警告）

当前 53 个文件中有 7 个共 15 条警告：

| 文件 | 条数 | 警告类型 |
|---|---|---|
| `example/W3-Day2-pointer-array-relationship.c` | 8 | `-Wformat=` |
| `w3-pointer-toolbox/main.c` | 2 | `-Wparentheses` |
| `example/W4-Day1-pointer-myMemcpy.c` | 1 | `-Wdiscarded-qualifiers` |
| `w3-string-processor/main.c` | 1 | `-Wsign-compare` |
| `example/W2-Day2-reverse-number.c` | 1 | `-Wunused-variable` |
| `example/W3-Day3-pointer-const.c` | 1 | `-Wunused-but-set-variable` |
| `example/W3-Day4-pointer-strcpy.c` | 1 | `-Wparentheses` |

**这 7 类警告的正确写法在 `projects/multi-file-template/` 里有对照示例**（该目录零警告通过）。
