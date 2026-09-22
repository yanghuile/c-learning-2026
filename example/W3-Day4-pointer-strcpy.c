#include <stdio.h>

char *myStrcpy(char *dest,const char *src);

int main()
{
    char src[] = "hello";
    char dest[20]; // 必须保证 dest 有足够的空间

    char *result = myStrcpy(dest, src);

    printf("复制后的字符串: %s\n", result);
    printf("目标首地址是否正确: %s\n", dest);


    return 0;
}

// 返回 char*，接收目标地址 dest 和只读的源地址 src
char *myStrcpy(char *dest,const char *src)
{
     // 在移动前，备份 dest 的首地址
    char *start = dest; 
    
    // 核心的一行：一边复制，一边移动，直到遇到 '\0'
    while (*dest++ = *src++); 
    
    // 返回备份的首地址
    return start;
}