# ============================================================
#  W4 练习：把 w3-pointer-toolbox 拆成多文件工程
#  用法（注意：你的 MinGW 里 make 叫 mingw32-make）:
#     mingw32-make          编译
#     mingw32-make run      编译并运行
#     mingw32-make check    只做警告检查（不生成可执行文件）
#     mingw32-make clean     删除编译产物
# ============================================================

CC      = gcc
# -Wall -Wextra -Wpedantic = 最严格警告；本周目标就是让这里一条警告都不出
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
TARGET  = string_toolbox

# 所有源文件（新增 .c 就加在这里）
SRCS    = my_string.c main.c
# 头文件（改了头文件也要重新编译）
HDRS    = my_string.h

# 由 SRCS 推导出目标文件名
OBJS    = $(SRCS:.c=.o)

.PHONY: all run check clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "--> 链接 $@"
	@$(CC) $(CFLAGS) -o $@ $(OBJS)

# .c -> .o 的通用规则；$< 是源文件，$@ 是目标文件
%.o: %.c $(HDRS)
	@echo "--> 编译 $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# 只做语法与警告检查，不产生任何文件（也便于快速迭代）
check:
	@echo "--> 检查全部源文件的警告"
	@$(CC) $(CFLAGS) -fsyntax-only $(SRCS)
	@echo "--> 检查完成（上面没有任何 warning 就是通过）"

run: $(TARGET)
	@./$(TARGET)

clean:
	@-del /Q $(OBJS) $(TARGET).exe 2>nul
	@-del /Q $(TARGET) 2>nul
	@echo "--> 已清理编译产物"
