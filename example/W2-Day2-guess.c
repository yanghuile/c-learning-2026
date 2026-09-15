#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    //生成一个100以内的随机数
    srand(time(0));
    int number=rand()%100+1;

    int guess=0;
    int count=0;

    printf("我已经想好了一个100以内的数\n");
    printf("来猜猜这个数：");

    do
    {
        scanf("%d",&guess);
        count++;
        if(guess>number)
        {
            printf("猜大了\n");
        }
        else if(guess<number)
        {
            printf("猜小了\n");
        }
    } while (guess!=number);
    printf("你猜对了！用了%d次\n",count);
    
    return 0;
}