/* ============================================================
 *  my_string.c —— 各函数的实现
 *
 *  本周要修的 15 条警告里，有 4 类在这里演示了正确写法：
 *    -Wdiscarded-qualifiers : const 不能丢（见 my_memcpy / my_memset）
 *    -Wparentheses          : while((*d++ = *s++)) 要加一层括号
 *    -Wsign-compare         : 有符号/无符号比较要统一类型
 *    -Wformat=              : %p 打印指针要先转成 (void*)
 * ============================================================ */

#include "my_string.h"

/* ---------- 求长度 ---------- */
size_t my_strlen(const char *s)
{
    const char *p = s;      /* 备份首地址，用 p 去走，s 保持不动 */
    while (*p != '\0')
    {
        p++;
    }
    return (size_t)(p - s); /* 指针相减 = 元素个数 */
}

/* ---------- 拷贝 ---------- */
char *my_strcpy(char *dest, const char *src)
{
    char *start = dest;     /* 先备份，因为 dest 等下会被移动 */

    /* 这里演示 -Wparentheses 的正确写法：
     * 外面再加一层括号，明确告诉编译器"我就是要用赋值结果做条件" */
    while ((*dest++ = *src++))
    {
        /* 空循环体：复制到 '\0' 为止 */
    }
    return start;
}

/* ---------- 比较 ---------- */
int my_strcmp(const char *s1, const char *s2)
{
    /* unsigned char 转换：保证比较的是 0~255 的字节值，
     * 避免 char 是 signed 时中文/高位字符比较出错
     * （这是标准库的实现方式，面试会问） */
    while (*s1 != '\0' && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (int)(unsigned char)*s1 - (int)(unsigned char)*s2;
}

/* ---------- 拼接 ---------- */
char *my_strcat(char *dest, const char *src)
{
    char *start = dest;

    while (*dest != '\0')   /* 第一步：走到 dest 末尾 */
    {
        dest++;
    }
    while ((*dest++ = *src++))  /* 第二步：把 src 接上去 */
    {
    }
    return start;
}

/* ---------- 查找字符 ---------- */
char *my_strchr(const char *s, int c)
{
    char target = (char)c;  /* 明确转成 char 再比较 */

    /* 注意：标准 strchr 还要能查到 '\0' 本身，所以循环条件写在内部 */
    while (*s != '\0')
    {
        if (*s == target)
        {
            return (char *)s;   /* 去掉 const：因为标准库签名就是返回 char* */
        }
        s++;
    }
    if (target == '\0')         /* 查的就是结尾符，返回末尾地址 */
    {
        return (char *)s;
    }
    return NULL;
}

/* ---------- 内存拷贝（注意 const） ---------- */
void *my_memcpy(void *dest, const void *src, size_t n)
{
    void *ret = dest;           /* 备份首地址用于返回 */

    char *d = (char *)dest;     /* 目标可按字节写，所以不强加 const */
    const char *s = (const char *)src;   /* ← 关键：源是只读，必须声明成 const char*。
                                          *   写成 char *s = (const char*)src 会触发
                                          *   -Wdiscarded-qualifiers，而且在 MCU 上
                                          *   写入字面量所在的只读区会直接 HardFault */

    while (n > 0)               /* 用 n > 0 而不是 n--，
                                 * 因为 n 是 size_t（无符号），
                                 * 比较时不会出现有符号/无符号混用 */
    {
        *d++ = *s++;
        n--;
    }
    return ret;
}

/* ---------- 内存填充 ---------- */
void *my_memset(void *s, int c, size_t n)
{
    void *ret = s;
    unsigned char *p = (unsigned char *)s;   /* 用 unsigned char 保证按字节处理 */
    unsigned char value = (unsigned char)c;  /* 只取低 8 位 */

    while (n > 0)
    {
        *p++ = value;
        n--;
    }
    return ret;
}
