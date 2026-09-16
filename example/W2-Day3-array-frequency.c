#include <stdio.h>

int main()
{
    int x;
    printf("请输入数字(0-9),输入 -1 结束：\n");
    scanf("%d",&x);
    int count[10]={0};
    while(x!=-1)
    {
        if(x>=0&&x<=9)
        {
            count[x]++;
        }
        scanf("%d",&x);
    }
    for(int i=0;i<10;i++)
    {
        if(count[i]>0)
        {
            printf("%d出现%d次\n",i,count[i]);
        }
    }

    return 0;
}