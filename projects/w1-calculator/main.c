#include <stdio.h>

int main()
{
	double a, b;

	printf("请输入两个数(用空格分隔）：");

	/* 检查输入是否成功：scanf返回成功读取的变量个数 */
	if (scanf("%lf %lf", &a, &b) != 2)
	{
		printf("输入有误，请重新输入！\n");
		return 1;
	}
	
	printf("a+b=%.2lf\n", a + b);
	printf("a-b=%.2lf\n", a - b);
	printf("a*b=%.2lf\n", a * b);

	/* 除法特殊处理：检查除数是否为0 */
	if (b == 0)
	{
		printf("a/b=除数不能为0!\n");
	}
	else
	{
		printf("a/b=%.2lf", a / b);
	}

	return 0;
}
