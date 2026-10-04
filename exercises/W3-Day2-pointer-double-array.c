#include <stdio.h>

void doubleArray(int *p,int n);

int main()
{
    int arr[5]={1,2,3,4,5};
    int len=5;

    //修改前
    for(int i=0;i<len;i++)
    {
        printf("%d",arr[i]);
        if(i<len-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    doubleArray(arr,len);

    //修改后
    for(int i=0;i<len;i++)
    {
        printf("%d",arr[i]);
        if(i<len-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}

void doubleArray(int*p,int n)
{
    for(int i=0;i<n;i++)
    {
        *(p+i)*=2;
    }
}
