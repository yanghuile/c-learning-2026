#include <stdio.h>

int main()
{
    int arr[100];
    int diff=0;
    int n;
    scanf("%d",&n);

    //判断n是否在有效范围内
    if(n<1||n>100)
    {
        printf("输入数量超出范围\n");
        return 0;
    }

    for(int j=0;j<n;j++)
    {
        scanf("%d",&arr[j]);
    }

    //使用arr[0]初始化最大值和最小值
    int max=arr[0];
    int min=arr[0];

    //遍历数组做判断
    for(int i=0;i<n;i++)  //可以直接从 i=1 开始，因为 i=0 就是初始值
    {
        if(arr[i]>max)
        {
            max=arr[i];
        }
        if(arr[i]<min)
        {
            min=arr[i];
        }
    }
    diff=max-min;
    printf("max=%d,min=%d,diff=%d",max,min,diff);

    return 0;
}