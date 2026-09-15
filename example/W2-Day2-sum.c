#include <stdio.h>

int main()
{
    double sum=0.0;
    int n;
    scanf("%d",&n);

    //求f(n)=1+1/1+1/2+1/3+…1/n
    for(int i=1;i<=n;i++)
    {
        sum+=1.0/i;
    }
    printf("f(%d)=%f\n",n,sum);

    return 0;
}