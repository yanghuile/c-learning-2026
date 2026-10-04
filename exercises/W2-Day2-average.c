#include <stdio.h>

int main()
{
    int sum=0;
    int count=0;
    int number;

    printf("请输入：");

    scanf("%d",&number);
    while(number!=-1)
    {
        sum+=number;
        count++;
        scanf("%d",&number);
    }
    if(count>0)
    {
        printf("平均数为：%.2f\n",(float)sum/count);
    }
    else
    {
        printf("你还没有输入任何有效数字\n");
    }
    return 0;
}