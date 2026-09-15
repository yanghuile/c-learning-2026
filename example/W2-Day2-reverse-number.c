#include <stdio.h>

int main()
{
    //初始化
    int x;
    int digit;
    int ret=0;

    printf("请输入一个数：");
    scanf("%d",&x);
    printf("这个数的逆序是：");

    //处理负数
    if(x<0)
    {
        printf("-");
        x=-x;
    }

    //处理0的情况
    if(x==0)
    {
        printf("0");
    }

    //逆序输出
    while(x>0)
    {
        digit=x%10;
        printf("%d",digit);   //直接取出最后一位打印
        x/=10;
    }
    printf("\n");
    
    return 0;
}