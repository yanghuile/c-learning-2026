#include <stdio.h>

int main()
{
    int n;
    int arr[100];
    int x;

    scanf("%d",&n);

    for(int j=0;j<n;j++)
    {
        scanf("%d",&arr[j]);
    }

    //读取目标值
    scanf("%d",&x);

    //遍历数组
    int i;
    for(i=0;i<n;i++)
    {
        if(arr[i]==x)
        {
            int index=i;
            printf("%d\n",index);
            break;
        }
    }
    //循环自然结束，说明没找到
    if(i==n)
    {
        printf("-1\n");
    }

    return 0;
}