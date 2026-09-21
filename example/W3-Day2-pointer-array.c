#include <stdio.h>

int main()
{
    int arr[5]={10,20,30,40,50};
    int *p=&arr[0];

    for(int i=0;i<5;i++)
    {
        printf("%d",*p++);
        if(i<4)
        {
            printf(" ");
        }
    }

    return 0;
}