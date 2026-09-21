#include <stdio.h>

int main()
{
    int a = 10;
    int b = 20;

    // ================= 情况一：const int *p1 =================
    // 指向 const int 的指针。const 修饰的是 *p1，即“不能通过 p1 改值”。
    const int *p1 = &a;
    
    // 1. 尝试通过 p1 改值（取消注释会报错）
    //*p1 = 100;  // 🚨 报错：assignment of read-only location '*p1'
    
    // 2. 尝试修改 p1 的指向（合法，允许！）
    p1 = &b;       // ✅ 没问题，p1 可以指向别的变量


    // ================= 情况二：int *const p2 =================
    // 指针本身是 const。const 修饰的是 p2，即“不能改指向”。
    int *const p2 = &a;
    
    // 1. 尝试修改 p2 的指向（取消注释会报错）
    // p2 = &b;    // 🚨 报错：assignment of read-only variable 'p2'
    
    // 2. 尝试通过 p2 改值（合法，允许！）
    *p2 = 100;     // ✅ 没问题，a 的值变成了 100

    printf("a = %d\n", a);
    return 0;
}