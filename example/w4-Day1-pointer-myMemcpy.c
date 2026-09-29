#include <stdio.h>

void *myMemcpy(void *dest,const void *src,size_t n);

int main()
{
    int arr1[5]={1,2,3,4,5};
    int arr2[5]={0};

    // 拷贝 arr1 的前 3 个整数（3 * 4 = 12 个字节）到 arr2
    myMemcpy(arr2,arr1,3*sizeof(int));

    printf("拷贝后的 arr2:");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    
    return 0;
}

void *myMemcpy(void *dest,const void *src,size_t n)
{
    // 备份目标首地址，用于最终返回
    void *ret=dest;

    // 强制类型转换为 char*，以便按字节操作
    char *d=(char *)dest;
    const char *s=(const char *)src;

    // 循环拷贝 n 个字节
    while(n--)
    {
        *d++=*s++;
    }

    // 返回目标首地址
    return ret;
}