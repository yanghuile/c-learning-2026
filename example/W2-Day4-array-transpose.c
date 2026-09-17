#include <stdio.h>

int main()
{
    int a[10][10];  //原矩阵
    int b[10][10];  //转置后的矩阵
    int m,n;
    scanf("%d %d",&m,&n);

    //输入原矩阵
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf("%d",&a[i][j]);
        }
    }
    
    //转置
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            b[j][i]=a[i][j];
        }
    }
    printf("\n");

    //输出转置矩阵b
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            printf("%d",b[i][j]);
            if(j<m-1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}
