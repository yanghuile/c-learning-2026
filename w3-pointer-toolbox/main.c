#include <stdio.h>

int myStrlen(const char *s);
char *myStrcpy(char *dest,const char *src);
int myStrcmp(const char *s1,const char *s2);
char *myStrcat(char *dest,const char *src);
char *myStrchr(const char *s,int c);

int main()
{
    char str1[100], str2[100];
    int choice;

    while (1)
    {
        printf("\n===== 指针字符串工具箱 =====\n");
        printf("1. 测试 myStrlen\n");
        printf("2. 测试 myStrcpy\n");
        printf("3. 测试 myStrcmp\n");
        printf("4. 测试 myStrcat\n");
        printf("5. 测试 myStrchr\n");
        printf("0. 退出\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请输入字符串: ");
            scanf("%s", str1);
            printf("长度是: %d\n", myStrlen(str1));
            break;
        case 2:
            printf("请输入源字符串: ");
            scanf("%s", str1);
            myStrcpy(str2, str1);
            printf("拷贝结果是: %s\n", str2);
            break;
        case 3:
            printf("请输入第一个字符串: ");
            scanf("%s", str1);
            printf("请输入第二个字符串: ");
            scanf("%s", str2);
            printf("比较结果: %d\n", myStrcmp(str1, str2));
            break;
        case 4:
            printf("请输入目标字符串: ");
            scanf("%s", str1);
            printf("请输入要拼接的字符串: ");
            scanf("%s", str2);
            // 注意：拼接前要保证 str1 有足够的空间
            myStrcat(str1, str2);
            printf("拼接结果是: %s\n", str1);
            break;
        case 5:
            printf("请输入字符串: ");
            scanf("%s", str1);
            printf("请输入要查找的字符: ");
            char target;
            scanf(" %c", &target); // 注意 %c 前面的空格，吃掉回车
            char *result = myStrchr(str1, target);
            if (result != NULL)
            {
                printf("找到了！位置索引是: %d\n", (int)(result - str1));
            }
            else
            {
                printf("没找到该字符。\n");
            }
            break;
        case 0:
            printf("再见！\n");
            return 0;
        default:
            printf("无效输入，请重新选择！\n");
            break;
        }
    }

    return 0;
}

// ================= 函数实现 =================
// 1. 计算字符串长度
int myStrlen(const char *s)
{
    const char *p=s;     // 备份首地址，用 p 去遍历，保留 s 不动
    while(*p!='\0')
    {
        p++;
    }
    return p-s;          // 两个指针相减，得到元素个数
}

// 2. 字符串拷贝
char *myStrcpy(char *dest,const char *src)
{
    char *start=dest;          //备份 dest 的首地址
    while(*dest++=*src++);
    return start;
}

// 3. 字符串比较
int myStrcmp(const char *s1,const char *s2)
{
    while(*s1&&(*s1==*s2))       // 只要 s1 没到末尾，且 s1 和 s2 当前字符相等，就继续往后走
    {
        s1++;
        s2++;
    }
    // 循环结束时，要么 s1 到了末尾，要么遇到了不相等的字符
    // 直接返回两者的差值（ASCII 码相减），>0 表示 s1 大，<0 表示 s1 小，=0 表示相等
    return *s1-*s2;
}

// 4. 字符串拼接
char *myStrcat(char *dest,const char *src)
{
    // 第一步：让 dest 指针走到目标字符串的末尾（遇到 '\0'）
    while(*dest!='\0')
    {
        dest++;
    }
    // 第二步：把 src 拷贝到 dest 的末尾
    char *start=dest;
    while(*dest++=*src++);
    return start;
}

// 5. 查找字符
char *myStrchr(const char *s,int c)
{
    while(*s!='\0')        // 把 int 类型的 c 强转成 char 再比较
    {
        if(*s==(char)c)
        {
            // 找到了！返回当前字符的地址
            // (char*)s 强制转换是为了去除 const 属性，符合标准库 char* 的返回类型
            return (char*)s;
        }
        s++;
    }
    return NULL;           // 遍历完整个字符串都没找到，返回空指针
}