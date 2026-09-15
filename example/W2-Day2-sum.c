#include <stdio.h>

int main()
{
    double sum=0.0;
    double sign=1.0;
    int n;
    scanf("%d",&n);

    //求f(n)=1-1/1+1/2-1/3+…+1/n
    for(int i=1;i<=n;i++)
    {
        sum+=sign/i;
        sign=-sign;
    }
    printf("f(%d)=%f\n",n,sum);

    return 0;
}