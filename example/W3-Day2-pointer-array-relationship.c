#include <stdio.h>

int main()
{
    int arr[5] = {10, 20, 30, 40, 50};

    // 打印地址必须用 %p 格式化符
    printf("arr          = %p\n", arr);         // 数组名
    printf("&arr[0]      = %p\n", &arr[0]);     // 首元素取地址
    printf("arr + 1      = %p\n", arr + 1);     // 数组名 + 1
    printf("&arr[0] + 1  = %p\n", &arr[0] + 1); // 首元素取地址 + 1

    return 0;
}