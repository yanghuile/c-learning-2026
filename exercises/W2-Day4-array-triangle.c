#include <stdio.h>

int main()
{
    int arr[20][20]={0};
    int n;
    scanf("%d",&n);

    //构建杨辉三角
    for(int i=0;i<n;i++)
    {
        arr[i][0]=arr[i][i]=1;   //第一列和对角线全为1
        //从第3行（i=2）开始，计算中间的数
        for(int j=1;j<i;j++)    
        {
            arr[i][j]=arr[i-1][j-1]+arr[i-1][j];
        }
    }

    //打印直角三角形
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<=i;j++)
        {
            printf("%d",arr[i][j]);
            if(j<i)
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}