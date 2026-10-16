/* ============================================================
 *  test_rb.c —— 环形缓冲区模块的测试
 *
 *  运行：mingw32-make test
 *
 *  用轻量测试框架（和 w5-linked-list 一样）：CHECK 宏 + 通过/失败汇总
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "rb.h"

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                                  \
    do {                                                                  \
        if (cond) { g_pass++; }                                           \
        else { g_fail++; printf("  [FAIL] %s  (第 %d 行)\n", msg, __LINE__); } \
    } while (0)

#define SECTION(name) printf("\n--- %s ---\n", name)

/* 实验用的静态缓冲区（模拟嵌入式里的做法） */
static uint8_t buf8[8];
static uint8_t buf1[1];
static uint8_t buf256[256];

int main(void)
{
    RingBuf rb;
    uint8_t out;

    printf("========== rb.c 测试 ==========\n");

    /* ==========================================================
     * 1. 初始化与参数校验
     * ========================================================== */
    SECTION("初始化与参数校验");
    {
        CHECK(rb_init(&rb, buf8, 8) == 1, "正常初始化返回 1");
        CHECK(rb_count(&rb) == 0, "初始化后 count 为 0");
        CHECK(rb_free(&rb) == 8, "初始化后 free 为 8");
        CHECK(rb_is_empty(&rb) == 1, "初始化后为空");
        CHECK(rb_is_full(&rb) == 0, "初始化后未满");

        CHECK(rb_init(&rb, NULL, 8) == 0, "buf 为 NULL → 返回 0");
        CHECK(rb_init(&rb, buf8, 0) == 0, "capacity 为 0 → 返回 0");
        CHECK(rb_init(NULL, buf8, 8) == 0, "rb 为 NULL → 返回 0");
    }

    /* ==========================================================
     * 2. 单个写入 / 读出
     * ========================================================== */
    SECTION("单个写入与读出");
    {
        rb_init(&rb, buf8, 8);

        CHECK(rb_put(&rb, 'A') == 1, "put A 成功");
        CHECK(rb_put(&rb, 'B') == 1, "put B 成功");
        CHECK(rb_count(&rb) == 2, "count 为 2");
        CHECK(rb_free(&rb) == 6, "free 为 6");

        CHECK(rb_get(&rb, &out) == 1 && out == 'A', "先读出 A（FIFO）");
        CHECK(rb_get(&rb, &out) == 1 && out == 'B', "再读出 B");
        CHECK(rb_is_empty(&rb) == 1, "读完后为空");

        out = 'Z';
        CHECK(rb_get(&rb, &out) == 0, "空读返回 0");
        CHECK(out == 'Z', "空读不修改 *data");

        CHECK(rb_get(&rb, NULL) == 0, "data 为 NULL → 返回 0（不崩）");
    }

    /* ==========================================================
     * 3. 写满 / 溢出策略
     * ========================================================== */
    SECTION("写满与两种溢出策略");
    {
        rb_init(&rb, buf8, 8);
        for (int i = 0; i < 8; i++) CHECK(rb_put(&rb, (uint8_t)('1' + i)) == 1, "写 1..8");
        CHECK(rb_is_full(&rb) == 1, "写满后 is_full 为 1");
        CHECK(rb_free(&rb) == 0, "写满后 free 为 0");

        /* 策略 1：丢弃新数据 */
        CHECK(rb_put(&rb, 'X') == 0, "满了 rb_put 返回 0（丢新）");
        CHECK(rb_count(&rb) == 8, "count 仍为 8");
        CHECK(rb_get(&rb, &out) == 1 && out == '1', "最旧的仍是 '1'（没被挤掉）");

        /* 策略 2：覆盖最旧 */
        rb_init(&rb, buf8, 8);
        for (int i = 0; i < 8; i++) rb_put_overwrite(&rb, (uint8_t)('1' + i));
        CHECK(rb_put_overwrite(&rb, 'X') == 1, "覆盖式总是返回 1");
        CHECK(rb_count(&rb) == 8, "覆盖后 count 仍是 8（不增长）");
        CHECK(rb_get(&rb, &out) == 1 && out == '2', "最旧的 '1' 被挤掉，现在最旧是 '2'");

        /* 覆盖式在未满时也要正常（不能丢数据） */
        rb_init(&rb, buf8, 8);
        rb_put_overwrite(&rb, 'a');
        rb_put_overwrite(&rb, 'b');
        CHECK(rb_count(&rb) == 2, "未满时覆盖式正常累加");
    }

    /* ==========================================================
     * 4. 批量写入（含回绕）
     * ========================================================== */
    SECTION("批量写入 rb_write");
    {
        rb_init(&rb, buf8, 8);

        CHECK(rb_write(&rb, (const uint8_t *)"ABC", 3) == 3, "空间足够写 3 个");
        CHECK(rb_count(&rb) == 3, "count 为 3");

        /* 空间不够：只剩 5 格，想写 10 个 */
        uint32_t w = rb_write(&rb, (const uint8_t *)"DEFGHIJKLM", 10);
        CHECK(w == 5, "空间不够只写 5 个");
        CHECK(rb_is_full(&rb) == 1, "写满了");
        CHECK(rb_count(&rb) == 8, "count 为 8");

        /* 边界：len=0 / src=NULL */
        rb_init(&rb, buf8, 8);
        CHECK(rb_write(&rb, (const uint8_t *)"AB", 0) == 0, "len=0 → 返回 0");
        CHECK(rb_write(&rb, NULL, 3) == 0, "src=NULL → 返回 0");
        CHECK(rb_count(&rb) == 0, "以上两次都没写入");
    }

    SECTION("批量写入 + 回绕（跨越数组末尾）");
    {
        rb_init(&rb, buf8, 8);

        /* 先把 write 推到下标 6 */
        rb_write(&rb, (const uint8_t *)"ABCDEF", 6);
        uint8_t tmp[4];
        rb_read(&rb, tmp, 4);                 /* read=4, write=6, count=2 */
        CHECK(rb_count(&rb) == 2, "读完 4 个后 count 为 2");

        /* 再写 4 个 → 下标 6,7,0,1（跨越末尾） */
        uint32_t w = rb_write(&rb, (const uint8_t *)"WXYZ", 4);
        CHECK(w == 4, "跨越末尾也能写 4 个");
        CHECK(rb_count(&rb) == 6, "count 为 6");

        uint8_t all[8];
        uint32_t r = rb_read(&rb, all, 8);
        CHECK(r == 6, "读出 6 个");
        CHECK(memcmp(all, "EFWXYZ", 6) == 0, "顺序正确：EFWXYZ（回绕没乱）");
    }

    /* ==========================================================
     * 5. 批量读出（含回绕）
     * ========================================================== */
    SECTION("批量读出 rb_read");
    {
        rb_init(&rb, buf8, 8);

        /* 数据不够 */
        rb_write(&rb, (const uint8_t *)"AB", 2);
        uint8_t dst[8];
        memset(dst, '.', sizeof(dst));
        uint32_t r = rb_read(&rb, dst, 8);
        CHECK(r == 2, "数据不够只读 2 个");
        CHECK(dst[0] == 'A' && dst[1] == 'B', "读出的内容对");
        CHECK(dst[2] == '.', "dst 剩余部分没被改写");
        CHECK(rb_is_empty(&rb) == 1, "读空后 is_empty");

        /* 空读 / 边界 */
        CHECK(rb_read(&rb, dst, 4) == 0, "空缓冲读 → 0");
        CHECK(rb_read(&rb, dst, 0) == 0, "len=0 → 0");
        CHECK(rb_read(&rb, NULL, 4) == 0, "dst=NULL → 0");
    }

    SECTION("批量读出 + 回绕");
    {
        rb_init(&rb, buf8, 8);

        /* 填满，读 5 个（让 read 到下标 5），再写 3 个（write 回绕） */
        rb_write(&rb, (const uint8_t *)"01234567", 8);
        uint8_t tmp[5];
        rb_read(&rb, tmp, 5);
        rb_write(&rb, (const uint8_t *)"abc", 3);
        /* 现在缓冲内容: 5 6 7 a b c（read=5, write=0, count=6） */
        CHECK(rb_count(&rb) == 6, "count 为 6");

        /* 一次读 6 个 → 会跨越末尾（下标 5,6,7,0,1,2） */
        uint8_t all[8];
        uint32_t r = rb_read(&rb, all, 8);
        CHECK(r == 6, "读出 6 个");
        CHECK(memcmp(all, "567abc", 6) == 0, "顺序正确：567abc（跨末尾读出没乱）");
    }

    /* ==========================================================
     * 6. rb_clear
     * ========================================================== */
    SECTION("rb_clear");
    {
        rb_init(&rb, buf8, 8);
        rb_write(&rb, (const uint8_t *)"HELLO", 5);
        rb_clear(&rb);
        CHECK(rb_count(&rb) == 0, "clear 后 count 为 0");
        CHECK(rb_is_empty(&rb) == 1, "clear 后为空");
        CHECK(rb_free(&rb) == 8, "clear 后 free 恢复为 8");

        /* 清空后必须还能正常写入 */
        CHECK(rb_write(&rb, (const uint8_t *)"NEW", 3) == 3, "clear 后能继续写");
        CHECK(rb_get(&rb, &out) == 1 && out == 'N', "clear 后读出的内容正确");
    }

    /* ==========================================================
     * 7. 容量为 1 的极端情况
     * ========================================================== */
    SECTION("容量为 1");
    {
        rb_init(&rb, buf1, 1);
        CHECK(rb_is_empty(&rb) == 1, "初始为空");
        CHECK(rb_put(&rb, 'A') == 1, "put A 成功");
        CHECK(rb_is_full(&rb) == 1, "满了");
        CHECK(rb_free(&rb) == 0, "free 为 0");
        CHECK(rb_put(&rb, 'B') == 0, "满了 put B 失败");
        CHECK(rb_get(&rb, &out) == 1 && out == 'A', "读出 A");
        CHECK(rb_is_empty(&rb) == 1, "读完后为空");
        CHECK(rb_put(&rb, 'C') == 1, "腾出空间后能再写");

        /* 覆盖式在容量 1 时也应正常 */
        rb_put_overwrite(&rb, 'D');
        CHECK(rb_get(&rb, &out) == 1 && out == 'D', "覆盖式在容量 1 时正常");
    }

    /* ==========================================================
     * 8. 大容量（256 字节，模拟串口缓冲）
     * ========================================================== */
    SECTION("容量 256（模拟串口接收缓冲）");
    {
        CHECK(rb_init(&rb, buf256, 256) == 1, "初始化 256 字节");
        CHECK(rb_free(&rb) == 256, "free 为 256");

        /* 造一段递增数据 */
        uint8_t src[256];
        for (int i = 0; i < 256; i++) src[i] = (uint8_t)i;

        CHECK(rb_write(&rb, src, 256) == 256, "一次写满 256");
        CHECK(rb_is_full(&rb) == 1, "写满");
        CHECK(rb_write(&rb, src, 1) == 0, "满了写 0 个");

        uint8_t dst[256];
        CHECK(rb_read(&rb, dst, 256) == 256, "一次读出 256");
        CHECK(memcmp(src, dst, 256) == 0, "内容完全一致");
        CHECK(rb_is_empty(&rb) == 1, "读空");
    }

    /* ==========================================================
     * 9. 压力测试：生产 = 消费，严格验证不丢数据
     * ========================================================== */
    SECTION("压力测试 9000 轮（写 3 读 3）");
    {
        rb_init(&rb, buf8, 8);

        uint32_t produced = 0, consumed = 0;
        int      order_ok = 1, first = 1;
        uint8_t  prev = 0;
        uint8_t  wbuf[3], rbuf[3];
        int      fail = -1;

        for (int round = 0; round < 3000; round++)
        {
            for (int k = 0; k < 3; k++) { wbuf[k] = (uint8_t)(produced & 0xFF); produced++; }
            if (rb_write(&rb, wbuf, 3) != 3) { fail = round; break; }
            if (rb_read(&rb, rbuf, 3) != 3)  { fail = round; break; }

            for (int k = 0; k < 3; k++)
            {
                if (!first && (uint8_t)(prev + 1) != rbuf[k]) order_ok = 0;
                prev = rbuf[k]; first = 0; consumed++;
            }
            if (!order_ok) { fail = round; break; }
        }

        if (fail >= 0) printf("  第 %d 轮出错\n", fail);
        CHECK(order_ok == 1, "9000 字节顺序严格连续");
        CHECK(consumed == 9000, "消费 9000");
        CHECK(rb_count(&rb) == 0, "生产=消费，缓冲读空");
    }

    SECTION("压力测试：生产略快于消费（正确处理部分写入）");
    {
        rb_init(&rb, buf8, 8);

        uint32_t next_val = 0, total_written = 0, consumed = 0;
        int      order_ok = 1, first = 1;
        uint8_t  prev = 0;
        int      partial = 0;

        for (int round = 0; round < 3000; round++)
        {
            uint8_t wbuf[3];
            for (int k = 0; k < 3; k++) wbuf[k] = (uint8_t)((next_val + k) & 0xFF);

            uint32_t w = rb_write(&rb, wbuf, 3);
            if (w < 3) partial++;
            next_val += w;              /* ★ 只推进实际写入数 */
            total_written += w;

            uint8_t rbuf[2];
            uint32_t g = rb_read(&rb, rbuf, 2);
            for (uint32_t k = 0; k < g; k++)
            {
                if (!first && (uint8_t)(prev + 1) != rbuf[k]) order_ok = 0;
                prev = rbuf[k]; first = 0; consumed++;
            }
        }

        CHECK(order_ok == 1, "部分写入场景下顺序仍连续");
        CHECK(total_written - consumed == rb_count(&rb), "写入-消费 == count");
        CHECK(partial > 0, "确实发生了部分写入（说明测试有效）");
        printf("  （实际写入 %u，消费 %u，部分写入轮数 %d）\n",
               (unsigned)total_written, (unsigned)consumed, partial);
    }

    /* ==========================================================
     * 10. 模拟真实场景：中断收字节 + 主循环批量处理
     * ========================================================== */
    SECTION("实战：模拟串口接收（中断写 + 主循环批量读）");
    {
        rb_init(&rb, buf256, 256);

        /* 一次"中断"收 1 个字节 */
        const char *fake_uart = "AT+TEST\r\nOK\r\n";
        uint32_t total_in = 0;
        for (const char *p = fake_uart; *p; p++)
        {
            if (rb_put(&rb, (uint8_t)*p)) total_in++;
        }
        CHECK(total_in == strlen(fake_uart), "全部字节都收进来了");
        CHECK(rb_count(&rb) == strlen(fake_uart), "count 与收到数一致");

        /* "主循环"一次批量读走 */
        uint8_t frame[64];
        uint32_t got = rb_read(&rb, frame, sizeof(frame));
        CHECK(got == strlen(fake_uart), "一次批读全部取走");
        CHECK(memcmp(frame, fake_uart, got) == 0, "内容与收到的完全一致");
        CHECK(rb_is_empty(&rb) == 1, "取走后缓冲为空");
    }

    /* ==========================================================
     * 汇总
     * ========================================================== */
    printf("\n========== 测试汇总 ==========\n");
    printf("  通过: %d\n", g_pass);
    printf("  失败: %d\n", g_fail);
    if (g_fail == 0) printf("  全部通过！\n");
    else             printf("  有失败项，见上面 [FAIL] 行\n");

    return (g_fail == 0) ? 0 : 1;
}
