/* ============================================================
 *  rb.c —— 环形缓冲区模块（实现）
 *
 *  数据结构练习项目 · W6
 *
 *  ⚠️ 下面标了 TODO 的函数由你实现（8 个）。
 *     写完运行：
 *         mingw32-make check    只检查警告
 *         mingw32-make test     编译并运行测试
 *
 *  实现要点（都是你这几天学过的）：
 *    - 满的条件：count == capacity（方案 B 计数器）
 *    - 空的条件：count == 0
 *    - 下标回绕：ptr = (ptr + 1) % capacity
 *    - 批量拷贝：跨越数组末尾时分【两段】memcpy
 *      first  = capacity - write;   if (first > n) first = n;
 *      second = n - first;
 * ============================================================ */

#include "rb.h"
#include <stdio.h>
#include <string.h>

/* ============================================================
 *  一、生命周期
 * ============================================================ */

int rb_init(RingBuf *rb, uint8_t *buf, uint32_t capacity)
{
    if(rb==NULL||buf==NULL||capacity==0)
    {
        return 0;
    }
    rb->buf=buf;
    rb->capacity=capacity;
    rb->count=0;
    rb->read=0;
    rb->write=0;
    return 1;
}

void rb_clear(RingBuf *rb)
{
    if(rb==NULL)
    {
        return;
    }
    rb->count=0;
    rb->read=0;
    rb->write=0;
}

/* ============================================================
 *  二、状态查询
 * ============================================================ */

uint32_t rb_count(const RingBuf *rb)
{
    if(rb==NULL){return 0;}
    return rb->count;
}

uint32_t rb_free(const RingBuf *rb)
{
    if(rb==NULL){return 0;}
    return rb->capacity-rb->count;
}

int rb_is_empty(const RingBuf *rb)
{
    if(rb==NULL){return 0;}
    return (rb->count==0);
}

int rb_is_full(const RingBuf *rb)
{
    if(rb==NULL){return 0;}
    return (rb->count==rb->capacity);
}

/* ============================================================
 *  三、单个字节读写
 * ============================================================ */

int rb_put(RingBuf *rb, uint8_t data)
{ 
    if(rb==NULL||rb->count==rb->capacity)
    {
        return 0;
    }
    rb->buf[rb->write]=data;
    rb->write=(rb->write+1)%rb->capacity;
    rb->count++;
    return 1;
}

int rb_put_overwrite(RingBuf *rb, uint8_t data)
{
    if(rb==NULL)
    {
        return 0;
    }
    if(rb->count==rb->capacity)
    {
        rb->read=(rb->read+1)%rb->capacity;
        rb->count--;
    }
    rb->buf[rb->write]=data;
    rb->write=(rb->write+1)%rb->capacity;
    rb->count++;
    return 1;
}

int rb_get(RingBuf *rb, uint8_t *data)
{
    if(rb==NULL||data==NULL||rb->count==0)
    {
        return 0;
    }
    *data=rb->buf[rb->read];
    rb->read=(rb->read+1)%rb->capacity;
    rb->count--;
    return 1;
}

/* ============================================================
 *  四、批量读写
 * ============================================================ */

uint32_t rb_write(RingBuf *rb, const uint8_t *src, uint32_t len)
{
    if(rb==NULL||src==NULL||len==0)
    {
        return 0;
    }
    uint32_t can_write=rb->capacity-rb->count;
    uint32_t real_write=(can_write<len)?can_write:len;
    uint32_t first=rb->capacity-rb->write;
    if(first>real_write)
    {
        first=real_write;
    }
    uint32_t second=real_write-first;
    memcpy(rb->buf+rb->write,src,first);
    if(second>0)
    {
        memcpy(rb->buf,src+first,second);
    }
    rb->write=(rb->write+real_write)%rb->capacity;
    rb->count+=real_write;
    return real_write;
}

uint32_t rb_read(RingBuf *rb, uint8_t *dst, uint32_t len)
{
    /* TODO: 和 rb_write 对称
     *   ① 挡住 dst == NULL / len == 0
     *   ② n = min(count, len)                     ← 能读几个
     *   ③ first = capacity - read;  if (first > n) first = n;
     *      second = n - first;
     *      memcpy(dst, rb->buf + rb->read, first);
     *      if (second > 0) memcpy(dst + first, rb->buf, second);
     *   ④ read 回绕（推进 n 格），count -= n
     *   ⑤ 返回 n
     */
    if(rb==NULL||dst==NULL||len==0)
    {
        return 0;
    }
    uint32_t can_read=rb->count;
    uint32_t real_read=(can_read<len)?can_read:len;
    uint32_t first=rb->capacity-rb->read;
    if(first>real_read)
    {
        first=real_read;
    }
    uint32_t second=real_read-first;
    memcpy(dst,rb->buf+rb->read,first);
    if(second>0)
    {
        memcpy(dst+first,rb->buf,second);
    }
    rb->read=(rb->read+real_read)%rb->capacity;
    rb->count-=real_read;
    return real_read;
}

/* ============================================================
 *  五、调试辅助（已写好）
 * ============================================================ */

void rb_dump(const RingBuf *rb)
{
    printf("     [rb] count=%u/%u read=%u write=%u | ",
           (unsigned)rb->count, (unsigned)rb->capacity,
           (unsigned)rb->read, (unsigned)rb->write);

    if (rb->count == 0 || rb->capacity == 0)
    {
        printf("(empty)\n");
        return;
    }

    for (uint32_t i = 0; i < rb->count; i++)
    {
        printf("%02X ", rb->buf[(rb->read + i) % rb->capacity]);
    }
    printf("\n");
}
