#ifndef BL_CRC_H_
#define BL_CRC_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// CCITT CRC16 standard polynomial
// xxx_BE : big-endian (historical original)
// xxx_LE : little-endian (modern architecture migration)
#define CRC_CCITT_BE    ((uint16_t)0b0001000000100001) /* bit0: input, bit15: output */
#define CRC_CCITT_LE    ((uint16_t)0b1000010000001000) /* bit15: input, bit0: output */
#define CRC_INBIT_LE    ((uint16_t)0b1000000000000000) /* bit15=1, bit14-0: = 0*/

    typedef union {
        uint32_t u32;
        uint16_t u16[2];
        uint8_t u8[4];
    } BL_crc_shifter_t, *pBL_crc_shifter_t;

    typedef struct {
        BL_crc_shifter_t crc;
        uint16_t gen; // generator polynomial
    } BL_crc_t, *pBL_crc_t;

    typedef const BL_crc_t *pcBL_crc_t;

    /*!
    \brief Initialize CRC instance
    \param gen [in] generator polynomial
    \param seed [in] initial value of crc calculator
    */
    void BL_crc_init(pBL_crc_t crc, uint16_t gen, uint16_t seed);

    /*!
    \brief Calculate CRC with a byte
    \param px [in] a byte data
    */
    void BL_crc_put8(pBL_crc_t crc, const uint8_t* px);

    /*!
    \brief Calculate CRC with two bytes
    \param px [in] two bytes data
    */
    void BL_crc_put16(pBL_crc_t crc, const uint16_t* px);

    /*!
    \brief Calculate CRC with N bytes
    \param px [in] N bytes data
    \param N [in] byte count of input data
    */
    void BL_crc_puts(pBL_crc_t crc, const uint8_t* px, uint32_t N);
#ifdef __cplusplus
}
#endif
#endif /* BL_CRC_H_ */
