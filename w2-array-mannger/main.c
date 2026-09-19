#include <stdio.h>

int main()
{
    int scores[100];
    float sum=0.0;
    int n;
    scanf("%d",&n);

    //判断边界，防止除零和越界
    if(n<1||n>100)
    {
        printf("无效的输入数量\n");
        return 0;
    }

    //数组输入并同时求和
    for(int i=0;i<n;i++)
    {
        scanf("%d",&scores[i]);
        sum+=scores[i];
    }

    //冒泡排序
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1-i;j++)
        {
            if(scores[j]>scores[j+1])
            {
                int temp=scores[j];
                scores[j]=scores[j+1];
                scores[j+1]=temp;
            }
        }
    }

    //输出最大/最小/平均
    printf("max=%d\n",scores[n-1]);
    printf("min=%d\n",scores[0]);
    printf("average=%.2f\n",sum/n);

    //排序输出
    for(int i=0;i<n;i++)
    {
        printf("%d",scores[i]);
        if(i<n-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}