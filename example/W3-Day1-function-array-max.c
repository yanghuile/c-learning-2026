#include <stdio.h>

int findMax(int arr[],int len);

int main()
{
    int arr[100];
    int n;
    scanf("%d",&n);

    if (n <= 0 || n > 100) 
    {
        printf("无效的输入数量\n");
        return 0;
    }

    for(int i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    printf("最大值是:%d\n",findMax(arr,n));

    return 0;
}


int findMax(int arr[],int len)
{
    if(len<=0)return -1;

    int max=arr[0];
    for(int i=1;i<len;i++)
    {
        if(arr[i]>max)
        {
            max=arr[i];
        }
    }
    return max;
}