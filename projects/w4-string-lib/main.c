/* ============================================================
 *  main.c —— 测试 my_string 库
 *
 *  学到的做法：每个函数都要测「正常情况 + 边界情况」。
 *  边界情况才是面试官会追问的地方。
 * ============================================================ */

#include <stdio.h>
#include "my_string.h"

int main(void)
{
    int failed = 0;     /* 用变量统计失败数，而不是靠肉眼看输出 */

    /* ---------- 测 my_strlen ---------- */
    /* 注意：MinGW 的 printf 不支持 %zu（它用微软的 C 运行时），
     * 所以 size_t 用 %d 打印时必须先强转成 int。
     * 这个坑在 Windows 上写 C 很常见，Linux/gcc 才支持 %zu。 */
    if (my_strlen("hello") != 5) { printf("[FAIL] strlen(hello) != 5\n"); failed++; }
    if (my_strlen("") != 0)      { printf("[FAIL] strlen(空串) != 0\n"); failed++; }
    printf("my_strlen   : hello=%d, 空串=%d\n", (int)my_strlen("hello"), (int)my_strlen(""));

    /* ---------- 测 my_strcpy ---------- */
    char buf[32];
    char *ret = my_strcpy(buf, "embedded");
    /* 标准 strcpy 返回 dest 首地址，这里顺便验证返回值 */
    if (ret != buf)          { printf("[FAIL] strcpy 返回值不是 dest 首地址\n"); failed++; }
    if (my_strlen(buf) != 8) { printf("[FAIL] strcpy 结果长度错\n"); failed++; }
    printf("my_strcpy   : %s\n", buf);

    /* ---------- 测 my_strcmp ---------- */
    if (my_strcmp("abc", "abc") != 0) { printf("[FAIL] strcmp 相等应返回 0\n"); failed++; }
    if (my_strcmp("abc", "abd") >= 0) { printf("[FAIL] abc 应小于 abd\n"); failed++; }
    if (my_strcmp("abd", "abc") <= 0) { printf("[FAIL] abd 应大于 abc\n"); failed++; }
    printf("my_strcmp   : abc/abc=%d, abc/abd=%d\n", my_strcmp("abc","abc"), my_strcmp("abc","abd"));

    /* ---------- 测 my_strcat ---------- */
    char cat[32];
    my_strcpy(cat, "hello ");
    my_strcat(cat, "world");
    if (my_strlen(cat) != 11) { printf("[FAIL] strcat 结果长度错\n"); failed++; }
    printf("my_strcat   : %s\n", cat);

    /* ---------- 测 my_strchr ---------- */
    const char *text = "hello world";
    char *found = my_strchr(text, 'w');
    if (found == NULL)                 { printf("[FAIL] 应该能找到 w\n"); failed++; }
    else if (found - text != 6)        { printf("[FAIL] w 的下标应是 6\n"); failed++; }
    if (my_strchr(text, 'z') != NULL)  { printf("[FAIL] z 不应该被找到\n"); failed++; }
    printf("my_strchr   : 找到 w 于下标 %d，查 z 返回 %s\n",
           (int)(found - text), my_strchr(text, 'z') == NULL ? "NULL" : "非NULL");

    /* ---------- 测 my_memcpy（重点：验证按字节拷贝） ---------- */
    int src[5] = { 1, 2, 3, 4, 5 };
    int dst[5] = { 0 };
    my_memcpy(dst, src, 3 * sizeof(int));   /* 只拷前 3 个 */
    if (dst[0] != 1 || dst[1] != 2 || dst[2] != 3) { printf("[FAIL] memcpy 前 3 个不对\n"); failed++; }
    if (dst[3] != 0)                                { printf("[FAIL] memcpy 不应改动第 4 个\n"); failed++; }
    printf("my_memcpy   : ");
    for (int i = 0; i < 5; i++) { printf("%d ", dst[i]); }
    printf("\n");

    /* ---------- 测 my_memset（验证按字节填充的陷阱） ---------- */
    int zero[3];
    my_memset(zero, 0, sizeof(zero));       /* 填 0：每个字节 0x00 → 整数 0，正确 */
    if (zero[0] != 0 || zero[1] != 0 || zero[2] != 0) { printf("[FAIL] memset 填 0 失败\n"); failed++; }
    printf("my_memset 0 : %d %d %d\n", zero[0], zero[1], zero[2]);

    /* 这里演示「memset 填 1 不会得到整数 1」这个经典陷阱 */
    int one[1];
    my_memset(one, 1, sizeof(one));         /* 每个字节 0x01 → 0x01010101 = 16843009 */
    printf("my_memset 1 : %d  (按字节填 1，不是整数 1！)\n", one[0]);

    /* ---------- 汇总 ---------- */
    printf("\n");
    if (failed == 0) { printf("全部测试通过\n"); return 0; }
    printf("有 %d 项测试失败\n", failed);
    return 1;
}
