#include <stdio.h>

void reverseArray(int *p,int n);

int main()
{
    int arr[]={1,2,3,4,5};
    int len=sizeof(arr)/sizeof(arr[0]);

    //反转前
    for(int i=0;i<len;i++)
    {
        printf("%d",arr[i]);
        if(i<len-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    reverseArray(arr,len);

    //反转后
    for(int i=0;i<len;i++)
    {
        printf("%d",arr[i]);
        if(i<len-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}

void reverseArray(int *p,int n)
{
    if(n<=1)
    {
        return;
    }

    // 定义头尾指针
    int *left = p;             // 头指针，指向第一个元素
    int *right = p + n - 1;    // 尾指针，指向最后一个元素

    // 当头指针在尾指针左侧时，循环继续
    while (left < right)
    {
        // 交换 *left 和 *right 指向的值
        int temp = *left;
        *left = *right;
        *right = temp;

        // 指针向中间靠拢
        left++;
        right--;
    }
}