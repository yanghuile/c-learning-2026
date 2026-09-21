#include <stdio.h>

int findMax(int *p,int n);

int main()
{
    int arr[100];
    int len;
    scanf("%d",&len);

    for(int i=0;i<len;i++)
    {
        scanf("%d",&arr[i]);
    }

    printf("max=%d",findMax(arr,len));

    return 0;
}

int findMax(int *p,int n)
{
    if(n<=0)
    {
        return -1;
    }

    int max=*p;
    for(int i=1;i<n;i++)
    {
        if(*(p+i)>*p)
        {
            max=*(p+1);
        }
    }
    return max;
}