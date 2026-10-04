#include <stdio.h>

double add(int a,int b);
double sub(int a,int b);
double mul(int a,int b);
double div(int a,int b);

int main()
{
    double (*ops[4])(int, int) = {add, sub, mul, div};
    int index,a,b;


    while(1)
    {
        printf("请输入：操作符索引(0-3) 数字1 数字2(输入 -1 退出)\n");
        // 读取操作符索引
        if (scanf("%d", &index) != 1) break; // 防止输入非数字死循环
        
        // 如果输入 -1，退出循环
        if (index == -1) {
            printf("计算器已退出。\n");
            break;
        }

        // 读取两个数字
        scanf("%d %d", &a, &b);

        // 3. 边界检查
        if (index < 0 || index > 3) {
            printf("索引无效，请输入 0-3 之间的数字。\n");
            continue;
        }

        // 4. 特殊处理除法除数为0的情况
        if (index == 3 && b == 0) {
            printf("除数不能为0!\n");
            continue;
        }

        // 5. 通过函数指针数组调用函数
        // ops[index] 取出对应函数，后面加 (a, b) 执行调用
        double result = ops[index](a, b);
        
        // 根据索引输出对应的符号
        char symbol[] = {'+', '-', '*', '/'};
        printf("%d %c %d = %.2f\n", a, symbol[index], b, result);
    }

    return 0;
}

double add(int a,int b)
{
    return a+b;
}

double sub(int a,int b)
{
    return a-b;
}

double mul(int a,int b)
{
    return a*b;
}

double div(int a,int b)
{
    return (double)a/b;
}
