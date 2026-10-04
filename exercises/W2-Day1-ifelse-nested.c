#include <stdio.h>

int main()
{
    //初始化
    int a,b,c;
    int max=0;
    //int max=0;
    //读取数字
    printf("请输入三个数字：");
    scanf("%d %d %d",&a,&b,&c);
    //判断大小
    if(b>=a)
    {
        if(b>=c)
        {
            max=b;
        }
        else
        {
            max=c;
        }
    }
    else
    {
        if(a>=c)
        {
            max=a;
        }
        else
        {
            max=c;
        }
    }
    printf("最大数是%d\n",max);
    
    return 0; 
}