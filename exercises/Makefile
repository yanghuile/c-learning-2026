# ============================================================
#  exercises/Makefile —— 链表练习的编译规则
#
#  用法：
#     mingw32-make day4      编译并运行 Day4（链表反转）
#     mingw32-make day3      编译并运行 Day3（链表进阶）
#     mingw32-make day2      编译并运行 Day2（链表基础）
#     mingw32-make check     只检查 list.c 的警告，不生成（周目标）
#     mingw32-make clean     删除编译产物
#
#  注意：mingw32-make 不是 make（你的环境里叫这个名字）
# ============================================================

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
BUILD   = build

# 源文件
LIB     = list.c
DAY2    = W5-Day2-链表基础.c
DAY3    = W5-Day3-链表进阶.c
DAY4    = W5-Day4-链表反转.c

# ============================================================
.PHONY: all day2 day3 day4 check clean

# 默认目标
all: day4

# ---------- Day 4：链表反转 ----------
day4: $(BUILD)/day4.exe
	@echo ""
	@echo "===== 运行 Day4 ====="
	@echo ""
	@$(BUILD)/day4.exe

$(BUILD)/day4.exe: $(DAY4) $(LIB) list.h
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) $(DAY4) $(LIB) -o $@

# ---------- Day 3：链表进阶 ----------
day3: $(BUILD)/day3.exe
	@echo ""
	@echo "===== 运行 Day3 ====="
	@echo ""
	@$(BUILD)/day3.exe

$(BUILD)/day3.exe: $(DAY3)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) $(DAY3) -o $@

# ---------- Day 2：链表基础 ----------
day2: $(BUILD)/day2.exe
	@echo ""
	@echo "===== 运行 Day2 ====="
	@echo ""
	@$(BUILD)/day2.exe

$(BUILD)/day2.exe: $(DAY2)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) $(DAY2) -o $@

# ---------- 只检查 list.c（不生成可执行文件）----------
check:
	@echo "--> 检查 list.c 的警告"
	@$(CC) $(CFLAGS) -fsyntax-only $(LIB)
	@echo "--> 检查完成（上面没有任何 warning 就是通过）"

# ---------- 清理 ----------
clean:
	@if exist $(BUILD) rmdir /s /q $(BUILD)
	@echo "已清理 $(BUILD)/"
