#include "base_c/BL_ringbuffer.h"

pBL_ringbuffer_t BL_ringbuffer_new(uint16_t cb_array_)
{
    uint32_t cb_header = sizeof(BL_ringbuffer_t);
    uint32_t cb_alloc = cb_header + cb_array_;
    pBL_ringbuffer p = (pBL_ringbuffer_t)calloc(cb_alloc, 1);
    p->cb_array = cb_array_;
    p->write_pos = p->read_pos = 0;
    return p;
}

#define BL_ringbuffer_nextpos(current_pos, cb_array_) ((current_pos + 1) % (cb_array_))

int BL_ringbuffer_putc(pBL_ringbuffer_t rb, const uint8_t* pchar)
{
    uint16_t next_write_pos = BL_ringbuffer_nextpos(rb->write_pos, rb_cb_array);
    if (next_write_pos == (rb->read_pos))
    {
        return ENOSPC;
    }
    rb->data[rb->write_pos] = *pchar;
    rb->write_pos = next_write_pos;
    return ESUCCESS;
}

void BL_ringbuffer_puts(pBL_ringbuffer_t rb, const uint8_t* pchar, uint16_t* length)
{
    while (ESUCCESS == BL_ringbuffer_putc(rb, pchar) && (*length != 0))
    {
        pchar++;
        *length--;
    }
}

void BL_ringbuffer_gets(pBL_ringbuffer_t rb, uint8_t* pchar, uint16_t* length)
{
}

int BL_ringbuffer_getc(pBL_ringbuffer_t rb, uint8_t* pchar)
{
}

#ifdef __cplusplus
}
#endif
#endif /* BL_RINGBUFFER_H_ */
