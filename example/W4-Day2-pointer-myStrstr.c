#include <stdio.h>

char *myStrstr(const char *haystack, const char *needle);

int main()
{
    const char *text = "hello world, welcome to C";
    const char *pattern = "world";

    char *result = myStrstr(text, pattern);

    if (result != NULL)
    {
        // 用返回的指针减去首地址，得到子串的起始下标
        printf("找到了！下标是: %d\n", (int)(result - text));
        printf("从该位置开始的字符串: %s\n", result);
    }
    else
    {
        printf("没找到子串。\n");
    }

    return 0;
}

// 函数定义
char *myStrstr(const char *haystack, const char *needle)
{
    // 防御：如果 needle 是空字符串，按标准应返回 haystack
    if (*needle == '\0')
    {
        return (char *)haystack;
    }

    // 外层循环：用 haystack 遍历主串的每一个起始位置
    while (*haystack != '\0')
    {
        // 备份当前匹配位置，用临时指针 p1 和 p2 去比较
        const char *p1 = haystack;
        const char *p2 = needle;

        // 内层循环：当 p1 和 p2 都没到结尾，且字符相等时，继续往后比
        while (*p1 && *p2 && (*p1 == *p2))
        {
            p1++;
            p2++;
        }

        // 如果 p2 走到了 '\0'，说明 needle 全部匹配成功
        if (*p2 == '\0')
        {
            return (char *)haystack; // 返回本次匹配的起始地址
        }

        // 匹配失败，haystack 向后移动一位，重新开始
        haystack++;
    }

    return NULL; // 遍历完 haystack 都没找到
}



//"hello" 中找 "ll"
//外层起点	  p1 和 p2 的比较过程	 内层退出时 p2 的位置	         结果
//  'h'	        'h' vs 'l' ❌	       指向 'l'	            不匹配，haystack++
//  'e'	        'e' vs 'l' ❌	       指向 'l'	            不匹配，haystack++
//  'l'	     'l'='l'→'l'='l'→'\0'	   指向 '\0' ✅	         匹配成功，返回