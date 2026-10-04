#include <stdio.h>

int main()
{
    int a=100;
    int *p=&a;
    int **pp=&p;

    **pp=200;

    printf("a = %d\n", a);
    printf("*p = %d\n", *p);
    printf("**pp = %d\n", **pp);

    return 0;
}

// p->a的地址      
// *p->a的值       
// pp->p的地址     
// *pp->p的值(a的地址)
//**pp->a的值