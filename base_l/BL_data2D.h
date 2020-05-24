#ifndef BL_DATA2D_H_
#define BL_DATA2D_H_
#include "base_l/BL_base.h"
#include "base_l/BL_types.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
    void* ptr;
    BL_2u32_t wh; // width and height of 2D array
    uint32_t cb_unit; // byte count of a unit element
} BL_data2D_t, *pBL_data2D_t;

typedef const BL_data2D_t *pcBL_data2D_t;

/*!
\brief allocate a memory block and setup ptr, wh, and cb_unit.
*/
pBL_data2D_t BL_data2D_alloc(const BL_2u32_t wh, uint32_t cb_unit);

/*!
\brief fill data array with fill_with.
\param data [in,out]
\param fill_with [in] (uint8_t*)fill_with[data->cb_unit] holds a template data element which
is copied to data->ptr[*]
*/
void BL_data2D_fill(pBL_data2D_t data, const void* fill_with);
#ifdef __cplusplus
}
#endif
#endif