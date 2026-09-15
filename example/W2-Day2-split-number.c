#include <stdio.h>

int main()
{
    int x;
    scanf("%d",&x);
    int t=0;
    //先逆序打印数字
    do
    {
        int d=x%10;
        t=t*10+d;
        x/=10;
    } while (x>0);

    //正序分解数字
    x=t;
    do
    {
        int d=x%10;
        printf("%d ",d);
        x/=10;
    }while(x>0);
    
    return 0;
}