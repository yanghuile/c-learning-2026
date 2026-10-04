#include <stdio.h>

int myStrlen(const char*p);

int main()
{
    char str[]="hello";

    printf("%d\n",myStrlen(str));

    return 0;
}

int myStrlen(const char*p)
{
    int len=0;
    while(*p!='\0')
    {
        len++;
        p++;
    }
    return len;
}