#include <stdio.h>

int main()
{
    //初始化
    int x;
    int n=1;
    //读取数据
    scanf("%d",&x);
    //处理负数
    if(x<0)
    {
        x=-x;
    }
    //先整除一次10
    x/=10;
    //循环判断
    while(x>0)
    {
        n++;
        x/=10;
    }
    printf("这个数是%d位数",n);

    return 0;
} 