#include <stdio.h>

int main()
{
    //初始化
    int a,b,c;
    //int max=0;
    //读取数字
    printf("请输入三个数字：");
    scanf("%d %d %d",&a,&b,&c);
    //判断大小
    if(b>=a)
    {
        if(b>=c)
        {
            printf("最大数是%d\n",b);
        }
        else
        {
            printf("最大数是%d\n",c);
        }
    }
    else
    {
        if(a>=c)
        {
            printf("最大数是%d",a);
        }
        else
        {
            printf("最大数是%d",c);
        }
    }
    
    return 0; 
}