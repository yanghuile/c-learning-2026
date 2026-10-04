#include <stdio.h>

/*=============练习一================*/
void counter_normal();
void counter_static();

int main()
{
    counter_normal();
    counter_normal();
    counter_normal();
    printf("\n");

    counter_static();
    counter_static();
    counter_static();

    return 0;
}

void counter_normal()
{
    int i=0;
    i++;
    printf("%d ",i);
}

void counter_static()
{
    static int j=0;
    j++;
    printf("%d ",j);
} 

/*=========练习二==========*/
// volatile int flag=0;

// void interrupt()
// {
//     flag=1;
// }

// int  main()
// {
//     printf("等待中断...\n");
//     while(flag==0)
//     {

//     }
//     printf("中断来了，退出循环！\n");
//     return 0;
// }


/*=========练习三==========*/
// int  main()
// {
//     int a=10;
//     int b=20;

//     const int *p=&a;            /*p是一个指针，指向const的int,不能改值，但能改指向*/
//     //*p=100;                   //错误！不能修改p指向的内容a
//     p=&b;                        //正确！可以修改指向

//     //int * const p=&a;          /* 指针p 本身是const,不能修改指向，但能改值 */
//     //*p=30;                     //正确
//     //p=&b;                      //错误

//     // const int * const p=&a;   /* 双重const，指针p本身和指向的内容都不能修改 */
//     //*p=30                      //错误
//     //p=&b;                      //错误

//     // int const *p=&a;          /* 与第一个等价    int const和const int一样 */
// }