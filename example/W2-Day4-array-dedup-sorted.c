#include <stdio.h>

int main()
{
    int n;
    int arr[100];
    
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    if (n > 0) //防止输入 n=0
    {
        int j = 0; // 慢指针，指向去重后数组的最后一个元素
        
        // 单层遍历：快指针 i 从 1 开始
        for (int i = 1; i < n; i++)
        {
            if (arr[i] != arr[j]) // 发现新元素
            {
                j++;              // 慢指针后移
                arr[j] = arr[i];  // 把新元素挪到前面
            }
        }

        // 输出去重后的数组，长度为 j + 1
        for (int i = 0; i <= j; i++)
        {
            printf("%d", arr[i]);
            if (i < j) // 控制空格，最后一个元素后不加空格
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}