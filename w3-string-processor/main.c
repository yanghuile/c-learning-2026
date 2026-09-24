#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sortWords(char *word[],int n);
int findLongest(char *word[],int n);

int main()
{
    char *word[100];  // 指针数组，最多存放 100 个单词（每个元素是 char* 指针）
    char buffer[50];  // 临时缓冲区，用于接收 scanf 读入的单词
    int n=0;          // 实际录入的单词个数，同时也是下一个空位的下标

    printf("请输入单词(输入end结束):\n");

    while(1)
    {
        scanf("%s",buffer);      // 先把单词读到临时缓冲区


        if(strcmp(buffer,"end")==0)
        {
            break;
        }

        if(n>=100)
        {
            printf("已到达上限\n");
            break;
        }

        // 动态分配内存：长度为 strlen(buffer) + 1（+1 留给结尾的 '\0'）
        word[n] = (char *)malloc(strlen(buffer) + 1);
        if(word[n]==NULL)
        {
            printf("内存分配失败\n");
            return 1;
        }

        // 把缓冲区中的内容拷贝到刚分配的内存中
        strcpy(word[n],buffer);
        n++;
    }

    sortWords(word,n);

    //排序后输出
    printf("排序后:");
    for(int i=0;i<n;i++)
    {
        printf("%s",word[i]);
        if(i<n-1)
        {
            printf(" ");
        }
    }
    printf("\n");

    int idx=findLongest(word,n);
    if(idx!=-1)
    {
        printf("最长单词: %s (长度%d)\n",word[idx],(int)strlen(word[idx]));
    }

    // ========== 释放内存 ==========
    // malloc 和 free 必须成对出现，避免内存泄漏
    for(int i=0;i<n;i++)
    {
        free(word[i]);
    }

    return 0;
}

// ========== 函数定义：冒泡排序 ==========
// 参数 word[]：指针数组（本质是 char **）
// 参数 n：单词个数
// 核心思想：交换指针，而不是交换字符串内容（效率高）
void sortWords(char *word[],int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1-i;j++)
        {
            if(strcmp(word[j],word[j+1])>0)
            {
                char *temp=word[j];
                word[j]=word[j+1];
                word[j+1]=temp;
            }
        }
    }
}

// ========== 函数定义：查找最长单词 ==========
// 返回最长单词的下标；若数组为空，返回 -1
int findLongest(char *word[],int n)
{
    if (n <= 0) return -1;

    int max_len=strlen(word[0]);
    int max_idx=0;

    for(int i=1;i<n;i++)
    {
        if(strlen(word[i]) > max_len)
        {
            max_len=strlen(word[i]);
            max_idx=i;
        }
    }
    return max_idx;
}