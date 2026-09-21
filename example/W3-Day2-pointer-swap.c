#include <stdio.h>

void swap(int *pa,int *pb);

int main()
{
    int a,b;
    scanf("%d %d",&a,&b);

    swap(&a,&b);
    printf("a=%d,b=%d\n",a,b);

    return 0;
}

void swap(int *pa,int *pb)
{
    int temp=*pa;
    *pa=*pb;
    *pb=temp;
}