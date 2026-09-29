CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g
TARGET  = my_toolbox
SRCS    = my_string.c main.c
HDRS    = my_string.h

OBJS    = $(SRCS:.c=.o)

.PHONY: all run check clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "--> 链接 $@"
	@$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HDRS)
	@echo "--> 编译 $<"
	@$(CC) $(CFLAGS) -c $< -o $@

check:
	@echo "--> 检查全部源文件的警告"
	@$(CC) $(CFLAGS) -fsyntax-only $(SRCS)
	@echo "--> 检查完成（上面没有任何 warning 就是通过）"

run: $(TARGET)
	@./$(TARGET).exe

clean:
	@-del /Q $(OBJS) $(TARGET).exe 2>nul
	@-del /Q $(TARGET) 2>nul
	@echo "--> 已清理编译产物"