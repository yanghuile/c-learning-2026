#include <stdio.h>

void add(int a,int b);
void sub(int a,int b);
void mul(int a,int b);
void div(int a,int b);

int main()
{
    int a,b;
    char op;
    scanf("%d %d",&a,&b);
    scanf(" %c",&op);

    switch (op)
    {
    case '+':
        add(a,b);
        break;
    case '-':
        sub(a,b);
        break;
    case '*':
        mul(a,b);
        break;
    case '/':
        div(a,b);
        break;
    default:
        printf("无效的运算符\n");
        break;
    }

    return 0;
}

void add(int a,int b)
{
    printf("%d + %d = %d\n", a, b, a + b);
}

void sub(int a,int b)
{
    printf("%d - %d = %d\n", a, b, a - b);
}

void mul(int a,int b)
{
    printf("%d * %d = %d\n", a, b, a * b);
}

void div(int a,int b)
{
    if(b==0)
    {
        printf("除数不能为0\n");
    }
    else
    {
        printf("%d / %d = %.2f\n", a, b, (float)a / b);
    }
}