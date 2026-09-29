#include <stdio.h>
#include "my_string.h"

int main(void)
{
    char str1[100], str2[100];
    int choice;

    while (1)
    {
        printf("\n===== 指针字符串工具箱 =====\n");
        printf("1. 测试 my_strlen\n");
        printf("2. 测试 my_strcpy\n");
        printf("3. 测试 my_strcmp\n");
        printf("4. 测试 my_strcat\n");
        printf("5. 测试 my_strchr\n");
        printf("6. 测试 my_memcpy\n");
        printf("7. 测试 my_memset\n");
        printf("0. 退出\n");
        printf("请选择: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请输入字符串: ");
            scanf("%99s", str1);
            printf("长度是: %d\n", (int)my_strlen(str1));
            break;
        case 2:
            printf("请输入源字符串: ");
            scanf("%99s", str1);
            my_strcpy(str2, str1);
            printf("拷贝结果是: %99s\n", str2);
            break;
        case 3:
            printf("请输入第一个字符串: ");
            scanf("%99s", str1);
            printf("请输入第二个字符串: ");
            scanf("%99s", str2);
            printf("比较结果: %d\n", my_strcmp(str1, str2));
            break;
        case 4:
            printf("请输入目标字符串: ");
            scanf("%99s", str1);
            printf("请输入要拼接的字符串: ");
            scanf("%99s", str2);
            // 注意：拼接前要保证 str1 有足够的空间
            my_strcat(str1, str2);
            printf("拼接结果是: %99s\n", str1);
            break;
        case 5:
            printf("请输入字符串: ");
            scanf("%99s", str1);
            printf("请输入要查找的字符: ");
            char target;
            scanf(" %c", &target); // 注意 %c 前面的空格，吃掉回车
            char *result = my_strchr(str1, target);
            if (result != NULL)
            {
                printf("找到了！位置索引是: %d\n", (int)(result - str1));
            }
            else
            {
                printf("没找到该字符。\n");
            }
            break;
        case 6:
        {
            /* 演示 my_memcpy 按【字节】拷贝：
             * 这里故意只拷前 3 个 int，看第 4、5 个有没有被改动 */
            int src[5] = {1, 2, 3, 4, 5};
            int dst[5] = {0};

            my_memcpy(dst, src, 3 * sizeof(int));   /* 3 个 int = 12 字节 */

            printf("源数组 (src): ");
            for (int i = 0; i < 5; i++)
            {
                printf("%d ", src[i]);
            }
            printf("\n");

            printf("拷贝后 (dst): ");
            for (int i = 0; i < 5; i++)
            {
                printf("%d ", dst[i]);
            }
            printf("\n说明: 只拷贝了前 3 个 int，后 2 个保持初始值 0\n");
            break;
        }
        case 7:
        {
            /* 演示 my_memset 按【字节】填充，以及它的经典陷阱 */
            int arr[2] = {0};

            printf("\n--- 用 0 填充（正确用法）---\n");
            my_memset(arr, 0, sizeof(arr));
            printf("arr[0]=%d arr[1]=%d\n", arr[0], arr[1]);

            printf("\n--- 用 1 填充（经典陷阱！）---\n");
            my_memset(arr, 1, sizeof(arr));
            printf("arr[0]=%d arr[1]=%d\n", arr[0], arr[1]);
            printf("说明: 每个字节被填成 0x01，一个 int 有 4 个字节\n");
            printf("      所以得到 0x01010101 = 16843009，而不是 1\n");
            break;
        }
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
