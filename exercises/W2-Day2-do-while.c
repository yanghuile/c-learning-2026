#include <stdio.h>

int main()
{
    //初始化
    int x;
    int n=0;
    scanf("%d",&x);
    //处理负数
    if(x<0)
    {
        x=-x;
    }
    do
    {
        n++;
        x/=10;
    } while (x>0);
    printf("这个数字是%d位数",n);

    return 0;
}