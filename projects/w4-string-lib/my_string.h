/* ============================================================
 *  my_string.h —— W4 练习：手写 string.h / 内存操作函数
 *
 *  头文件的三条规矩：
 *    1. 必须有 include guard（防止被重复包含）
 *    2. 只放「声明」，不放「实现」（实现放 .c 里）
 *    3. 参数该 const 的必须 const
 * ============================================================ */
#ifndef MY_STRING_H          /* 第 1 步：如果没定义过这个名字 */
#define MY_STRING_H          /* 第 2 步：定义它            */

#include <stddef.h>          /* 为了用 size_t */

/* 求字符串长度：只读，所以参数是 const char* */
size_t my_strlen(const char *s);

/* 字符串拷贝：返回 dest 首地址（和标准 strcpy 行为一致） */
char *my_strcpy(char *dest, const char *src);

/* 字符串比较：s1>s2 返回正数，相等返回 0，小于返回负数 */
int my_strcmp(const char *s1, const char *s2);

/* 字符串拼接：把 src 接到 dest 末尾 */
char *my_strcat(char *dest, const char *src);

/* 查找字符：找到返回该字符的地址，没找到返回 NULL */
char *my_strchr(const char *s, int c);

/* 内存拷贝：按字节拷贝 n 个字节。
 * 注意 src 是 const void*（只读），
 * dest 用 void* 是为了能接收任意类型的指针 —— 但【绝不能返回它作为可写指针】。
 * 返回值是 dest 首地址，类型 void*。 */
void *my_memcpy(void *dest, const void *src, size_t n);

/* 内存填充：把从 s 开始的 n 个字节都填成 c 的低 8 位。
 * c 是 int 类型（和标准 memset 一致），内部强转成 unsigned char。 */
void *my_memset(void *s, int c, size_t n);

#endif /* MY_STRING_H */
