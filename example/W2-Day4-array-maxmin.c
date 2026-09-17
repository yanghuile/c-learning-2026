#include <stdio.h>

int main()
{
    int arr[100];
    int diff=0;
    int max,min;
    int n;
    scanf("%d",&n);

    for(int j=0;j<n;j++)
    {
        scanf("%d",&arr[j]);
    }
    max=arr[0];
    min=arr[0];
    for(int i=0;i<n;i++)
    {
        if(arr[i]>max)
        {
            max=arr[i];
        }
        if(arr[i]<min)
        {
            min=arr[i];
        }
    }
    diff=max-min;
    printf("max=%d,min=%d,diff=%d",max,min,diff);

    return 0;
}