#include<stdio.h>

int main()
{
	float c,f;
	printf("请输入摄氏度：");
	scanf_s("%f", &c);
	f = c * (9.0 / 5.0) + 32;
	printf("对应的华氏度为：%.2f°F\n", f);
	printf("请输入华氏度：");
	scanf_s("%f", &f);
	c = (f - 32) * (5.0 / 9.0);
	printf("对应的摄氏度为：%.2f°C", c);
	
	return 0;
}