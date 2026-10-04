#include <stdio.h>

int main()
{
    int n;
    int arr[100];

    //输入数组长度
    scanf("%d",&n);

    // 输入n个整数存入数组
    for(int i=0;i<n;i++)
    {
        scanf("%d", &arr[i]);
    }

    // 逆序输出
    for(int j=n-1;j>=0;j--)
    {
        printf("%d",arr[j]);
        if(j>0)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}