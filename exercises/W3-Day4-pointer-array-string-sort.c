#include <stdio.h>
#include <string.h>

void sortStrings(char *str[],int n);

int main()
{
    // 定义指针数组，每个元素是一个指向字符串常量的指针
    char *fruits[]={"banana","apple","cherry","date","grape"};
    int len = sizeof(fruits) / sizeof(fruits[0]);

    for(int i=0;i<len;i++)
    {
        printf("%s",fruits[i]);
        if (i < len - 1) printf(", ");
    }
    printf("\n");

    sortStrings(fruits,len);

    for(int i=0;i<len;i++)
    {
        printf("%s",fruits[i]);
        if (i < len - 1) printf(", ");
    }
    printf("\n");

    return 0;
}

// 参数是指针数组（本质是指向字符串指针的指针）
void sortStrings(char *str[],int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1-i;j++)
        {
            if(strcmp(str[j],str[j+1])>0)
            {
                // 核心：交换的是“指针”，而不是字符串内容！
                char *temp=str[j];
                str[j]=str[j+1];
                str[j+1]=temp;
            }
        }
    }
}

