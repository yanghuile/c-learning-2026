#include <stdio.h>

int main()
{
    int fact=1;
    int n;
    scanf("%d",&n);

    //阶乘计算
    for(int i=1;i<=n;i++)
    {
        fact*=i;
    }
    printf("%d!=%d",n,fact);

    return 0;
}