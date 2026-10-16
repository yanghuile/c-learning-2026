# ============================================================
#  projects/w6-ringbuf/Makefile
#
#  环形缓冲区模块 · W6
#
#  用法：
#     mingw32-make            同 test
#     mingw32-make test       编译并运行测试
#     mingw32-make check      只检查警告，不生成可执行文件
#     mingw32-make lib        打包成静态库 librb.a
#     mingw32-make clean      删除编译产物
#
#  在你的环境里 GNU Make 叫 mingw32-make（不是 make）
# ============================================================

CC       = gcc
CFLAGS   = -std=c11 -Wall -Wextra -Wpedantic -g
BUILD    = build
LIB_SRC  = rb.c
TEST_SRC = test_rb.c
HDR      = rb.h

# ============================================================
.PHONY: all test check lib clean

all: test

# ---------- 编译并运行测试 ----------
test: $(BUILD)/test_rb.exe
	@echo ""
	@echo "===== 运行测试 ====="
	@$(BUILD)/test_rb.exe

$(BUILD)/test_rb.exe: $(TEST_SRC) $(LIB_SRC) $(HDR)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_SRC) -o $@

# ---------- 只检查警告（不生成可执行文件）----------
check:
	@echo "--> 检查 rb.c"
	@$(CC) $(CFLAGS) -fsyntax-only $(LIB_SRC)
	@echo "--> 检查 test_rb.c"
	@$(CC) $(CFLAGS) -fsyntax-only $(TEST_SRC)
	@echo "--> 检查完成（上面没有任何 warning 就是通过）"

# ---------- 打包静态库 ----------
lib: $(BUILD)/rb.o
	ar rcs $(BUILD)/librb.a $(BUILD)/rb.o
	@echo "--> 已生成 $(BUILD)/librb.a"
	@echo "    别人用法：gcc main.c -Lbuild -lrb"

$(BUILD)/rb.o: $(LIB_SRC) $(HDR)
	@if not exist $(BUILD) mkdir $(BUILD)
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $@

# ---------- 清理 ----------
clean:
	@if exist $(BUILD) rmdir /s /q $(BUILD)
	@echo "已清理 $(BUILD)/"
