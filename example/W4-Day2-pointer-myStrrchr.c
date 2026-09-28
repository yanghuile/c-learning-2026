#include <stdio.h>

// 函数声明
char *myStrrchr(const char *str, int c);

int main()
{
    const char *text = "hello world, welcome to C";
    char target = 'w';

    char *result = myStrrchr(text, target);

    if (result != NULL)
    {
        // 用返回的指针减去首地址，得到最后一次出现的下标
        printf("找到了！最后一次出现的下标是: %d\n", (int)(result - text));
        printf("从该位置开始的字符串: %s\n", result);
    }
    else
    {
        printf("没找到该字符。\n");
    }

    return 0;
}

// 函数定义（逆序遍历版）
char *myStrrchr(const char *str, int c)
{
    const char *p = str;

    // 1. 先走到字符串的末尾 '\0' 处
    while (*p != '\0')
    {
        p++;
    }

    // 2. 特殊情况：如果查找的就是 '\0'，直接返回末尾地址
    if ((char)c == '\0')
    {
        return (char *)p;
    }

    // 3. 从末尾往前退一步，开始逆向查找
    p--; 
    
    // 4. 逆序遍历，直到退到 str 前面为止
    while (p >= str) 
    {
        if (*p == (char)c)
        {
            return (char *)p; // 找到第一个就返回
        }
        p--; // 指针向前退一格
    }

    // 5. 退到头都没找到
    return NULL;
}