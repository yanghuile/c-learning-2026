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

    //初始化 index 为 -1，代表默认没找到
    int index=-1;

    //遍历数组
    for(int i=0;i<n;i++)
    {
        if(arr[i]==x)
        {
            index=i;
            break;
        }
    }
    //无论找没找到，最后统一输出index即可
    printf("%d\n",index);

    return 0;
}