#include <stdio.h>

int main()
{
    int arr[100];
    int val,n;

    //输入
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    scanf("%d",&val);

    //快慢指针遍历
    int slow=0;
    for(int fast=0;fast<n;fast++)
    {
        if(arr[fast]!=val)
        {
            arr[slow]=arr[fast];
            slow++;
        }
    }

    //输出
    printf("len=%d\n",slow);
    for(int i=0;i<slow;i++)
    {
        printf("%d",arr[i]);
        if(i<slow-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}