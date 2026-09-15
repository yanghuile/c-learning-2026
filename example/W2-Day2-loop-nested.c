#include <stdio.h>

int main()
{
    //找出100以内的素数
    for(int x=2;x<100;x++)
    {
        int isPrime=1;
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
            printf("%d ",x);
        }
    }
    return 0;
}