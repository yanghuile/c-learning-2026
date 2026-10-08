# ============================================================
#  projects/w5-linked-list/Makefile
#
#  单链表工具库 · W5
#
#  用法：
#     mingw32-make            同 test（编译并运行测试）
#     mingw32-make test       编译并运行测试
#     mingw32-make check      只检查警告，不生成可执行文件
#     mingw32-make lib        打包成静态库 liblist.a
#     mingw32-make clean      删除编译产物
#
#  在你的环境里 GNU Make 叫 mingw32-make（不是 make）
# ============================================================

CC       = gcc
CFLAGS   = -std=c11 -Wall -Wextra -Wpedantic -g
BUILD    = build
LIB_SRC  = list.c
TEST_SRC = test_list.c
HDR      = list.h

# ============================================================
.PHONY: all test check lib clean

all: test

# ---------- 编译并运行测试 ----------
test: $(BUILD)/test_list.exe
	@echo ""
	@echo "===== 运行测试 ====="
	@$(BUILD)/test_list.exe

$(BUILD)/test_list.exe: $(TEST_SRC) $(LIB_SRC) $(HDR)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_SRC) -o $@

# ---------- 只检查警告（不生成可执行文件）----------
check:
	@echo "--> 检查 list.c"
	@$(CC) $(CFLAGS) -fsyntax-only $(LIB_SRC)
	@echo "--> 检查 test_list.c"
	@$(CC) $(CFLAGS) -fsyntax-only $(TEST_SRC)
	@echo "--> 检查完成（上面没有任何 warning 就是通过）"

# ---------- 打包静态库 ----------
lib: $(BUILD)/list.o
	ar rcs $(BUILD)/liblist.a $(BUILD)/list.o
	@echo "--> 已生成 $(BUILD)/liblist.a"
	@echo "    别人用法：gcc main.c -Lbuild -llist"

$(BUILD)/list.o: $(LIB_SRC) $(HDR)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $@

# ---------- 清理 ----------
clean:
	@if exist $(BUILD) rmdir /s /q $(BUILD)
	@echo "已清理 $(BUILD)/"
