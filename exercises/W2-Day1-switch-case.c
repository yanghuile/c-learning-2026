#include <stdio.h>

int main()
{
    //初始化
    int score;
    //获取成绩
    printf("请输入你的成绩：");
    scanf("%d",&score);
    score/=10;
    //确定等级
    switch(score)
    {
        case 10:
        case 9:
            printf("你的成绩等级为A\n");
            break;
        case 8:
            printf("你的成绩等级为B\n");
            break;
        case 7:
            printf("你的成绩等级为C\n");
            break;
        case 6:
            printf("你的成绩等级为D\n");
            break;
        default:
            printf("你的成绩等级为F\n");
            break;
    }

    return 0;
}