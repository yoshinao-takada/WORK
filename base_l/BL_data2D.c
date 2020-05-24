#include "base_l/BL_data2D.h"
#include <malloc.h>
#include <memory.h>

pBL_data2D_t BL_data2D_alloc(const BL_2u32_t wh, uint32_t cb_unit)
{
    size_t cb_head = BL_ALIGN8(sizeof(BL_data2D_t));
    size_t cb_data = BL_ALIGN8(wh[0]*wh[1]*cb_unit);
    pBL_data2D_t p = (pBL_data2D_t)malloc(cb_head + cb_data);
    p->cb_unit = cb_unit;
    BL_copy2(p->wh, wh);
    p->ptr = (void*)((uint8_t*)p + cb_head);
    return p;
}

void BL_data2D_fill(pBL_data2D_t data, const void* fill_with)
{
    uint8_t* dst = (uint8_t*)(data->ptr);
    uint32_t linear_length = data->wh[0] * data->wh[1];
    const uint8_t* src = (uint8_t*)fill_with;
    do {
        memcpy(dst, src, data->cb_unit);
        dst += data->cb_unit;
        src += data->cb_unit;
    } while (-linear_length);
}