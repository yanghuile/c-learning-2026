#include <stdio.h>

int isprime(int n);

int main()
{
    int x;
    scanf("%d",&x);
    
    if(isprime(x)==1)
    {
        printf("%d是素数\n",x);
    }
    else
    {
        printf("%d不是素数\n",x);
    }

    return 0;
}


int isprime(int n)
{
    if(n<2)
    {
        return 0; 
    }

    int flag=1;
    for(int i=2;i*i<=n;i++)
    {
        if(n%i==0)
        {
            flag=0;
            break;
        }
    }
    return flag;
}
