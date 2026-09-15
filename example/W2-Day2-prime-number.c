#include <stdio.h>

int main()
{
    int x;
    scanf("%d",&x);

    int isPrime=1;    //设标志位

    for(int i=2;i<x;i++)
    {
        if(x%i==0)
        {
            isPrime=0;
            break;
        }
    }
    if(isPrime==1)
    {
        printf("这个数是素数");
    }
    else
    {
        printf("这个数不是素数");
    }

    return 0;
}