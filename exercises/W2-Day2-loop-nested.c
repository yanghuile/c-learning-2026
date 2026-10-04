#include <stdio.h>

int main()
{
    //找出前50个素数
    int cnt=0;
    int x=2;
    while(cnt<50)
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
            cnt++;
            if(cnt%10==0)
            {
                printf("\n");
            }
        }
        x++;
    }
    printf("\n");

    return 0;
}