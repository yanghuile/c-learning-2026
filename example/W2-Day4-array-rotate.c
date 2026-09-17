#include <stdio.h>

int main()
{
    int n;
    scanf("%d",&n);

    int arr[100];
    for(int i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    int k;
    scanf("%d",&k);

    k=k%n; //防止k大于n导致越界

    if(k>0)  //如果k=0，不需要做任何移动
    {
        //1.整体反转
        for(int i=0;i<n/2;i++)
        {
            int temp=arr[i];
            arr[i]=arr[n-1-i];
            arr[n-1-i]=temp;
        }

        //2.反转前n-k个元素
        for(int i=0;i<(n-k)/2;i++)
        {
            int temp=arr[i];
            arr[i]=arr[n-k-1-i];
            arr[n-k-1-i]=temp;
        }

        //3.反转后k个元素
        for(int i=0;i<k/2;i++)
        {
            int temp=arr[n-k+i];
            arr[n-k+i]=arr[n-1-i];
            arr[n-1-i]=temp;
        }
    }
    
    for(int i=0;i<n;i++)
    {
        printf("%d",arr[i]);
        if(i<n-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}