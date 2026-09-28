#include <stdio.h>

// 函数声明
void *my_memset(void *s, int c, size_t n);

int main()
{
    // 测试一：初始化整型数组为 0
    int arr[5];
    
    // 把 arr 的前 5 个 int（20 个字节）全部填成 0
    my_memset(arr, 0, sizeof(arr)); 
    
    printf("初始化后的 arr:");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 测试二：初始化字符数组为 'A'
    char str[10];
    my_memset(str, 'A', 9); // 填 9 个 'A'
    str[9] = '\0';          // 字符串结尾必须手动加 '\0'！memset 不会帮你加
    printf("初始化后的 str:%s\n", str);

    return 0;
}

// 函数定义
void *my_memset(void *s, int c, size_t n)
{
    // 1. 备份目标首地址
    void *ret = s;

    // 2. 强制转换为 char*，按字节操作
    char *p = (char *)s;

    // 3. 循环填充 n 个字节
    while (n--)
    {
        *p++ = (char)c; // 把 c 强转为 char 后赋值给当前位置，并后移指针
    }

    // 4. 返回目标首地址
    return ret;
}


//memset只能填充0或-1，因为是按字节填充的!!!
//原本 arr[0] 的内存：  [ 00 ] [ 00 ] [ 00 ] [ 00 ]  (代表数字 0)
//memset 填成 1 之后：  [ 01 ] [ 01 ] [ 01 ] [ 01 ]  (0x01010101->16843009)
//而0的每个字节都是 0x00，-1的每个字节都是0xFF，恰好能拼出正确的整数