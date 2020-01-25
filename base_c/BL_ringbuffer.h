#ifndef BL_RINGBUFFER_H_
#define BL_RINGBUFFER_H_
#include "base_l/BL_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t cb_array, cb_held; /* array size and byte count of held data */
    uint16_t write_pos, read_pos; /* write and read position */
    uint8_t data[0]; /* beginning of data */
} BL_ringbuffer_t, *pBL_ringbuffer_t;

typedef const BL_ringbuffer_t *pcBL_ringbuffer_t;

/*!
\brief create a new ring buffer
\param cb_array_ [in] byte count of data buffer
\return pointer to the new ringbuffer object
*/
pBL_ringbuffer_t BL_ringbuffer_new(uint16_t cb_array_);

/*!
\brief put a character into the ring buffer
\param rb [in,out] ring buffer
\param pchar [in] pointer to the character to put into the buffer
\return ENOBUF or ESUCCESS
*/
int BL_ringbuffer_putc(pBL_ringbuffer_t rb, const uint8_t* pchar);

/*!
\brief put characters into the ring buffer
\param rb [in,out] ring buffer
\param pchar [in] pointer to the characters to put into the buffer
\param length [in, out] in: required length, out: required length - actually put length
\return ENOBUF or ESUCCESS
*/
void BL_ringbuffer_puts(pBL_ringbuffer_t rb, const uint8_t* pchar, uint16_t* length);

/*!
\brief get characters from the ring buffer to the linear buffer
\param rb [in,out] ring buffer
\param pchar [out] pointer to the linear buffer
\param length [in,out] in: required length, out: required length - actually gotten length
*/
void BL_ringbuffer_gets(pBL_ringbuffer_t rb, uint8_t* pchar, uint16_t* length);

/*!
\brief get a character from the ring buffer to the character buffer
\param rb [in,out] ring buffer
\param pchar [out] pointer to the character buffer
\return ESUCCESS or ENODATA
*/
int BL_ringbuffer_getc(pBL_ringbuffer_t rb, uint8_t* pchar);

#ifdef __cplusplus
}
#endif
#endif /* BL_RINGBUFFER_H_ */
