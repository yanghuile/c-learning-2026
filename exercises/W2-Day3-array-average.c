#include <stdio.h>

int main()
{
    int x;
    int cnt=0;
    double sum=0;
    int number[100];
    scanf("%d",&x);

    while(x!=-1)
    {
        //防止数组越界
        if(cnt<100)
        {
            number[cnt]=x;
        }
        sum+=x;
        cnt++;
        scanf("%d",&x);
    }

    if(cnt==0)
    {
        printf("你未输入任何有效数字\n");
    }
    else
    {
        printf("平均数是:%.2f\n",sum/cnt);
        for(int i=0;i<cnt;i++)
        {
            if(number[i]>sum/cnt)
            {
                printf("%d\n",number[i]);
            }
        }
    }

    return 0;
}