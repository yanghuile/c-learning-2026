#include <stdio.h>

int main()
{
    char str[1000];
    int count[26]={0};

    scanf("%s",str);

    for(int i=0;str[i]!='\0';i++)
    {
        count[str[i]-'a']++;
    }

    for(int i=0;i<26;i++)
    {
        if(count[i]>0)
        {
            printf("%c:%d ",i+'a',count[i]);
        }
    }

    return 0;
}