#include "base_c/BL_crc.h"
#include <stdbool.h>

void BL_crc_init(pBL_crc_t crc, uint16_t gen, uint16_t seed)
{
    crc->gen = gen;
    crc->crc.u16[0] = seed;
}

void BL_crc_put8(pBL_crc_t crc, const uint8_t* px)
{
    // set input data byte
    crc->crc.u8[2] = *px;

    for (int i = 0; i < 8; i++)
    {
        uint16_t mask = ((crc->crc.u16[0]) ^ (crc->crc.u16[1])) & 1;

	    // calculate new CRC register value
	    // shifted_crc is copied to the bits of CRC register where generator polynomial bits == 0.
        BL_crc_shifter_t shifted = { crc->crc.u32 >> 1 };
        shifted.u16[0] &= ~CRC_INBIT_LE; // clear CRC register input bit
        if (mask)
        {
            shifted.u16[0] ^= crc->gen;
        }

        crc->crc.u32 = shifted.u32;
    }
}

void BL_crc_put16(pBL_crc_t crc, const uint16_t* px)
{
    // set input data byte
    crc->crc.u16[1] = *px;

    for (int i = 0; i < 16; i++)
    {
        uint16_t mask = ((crc->crc.u16[0]) ^ (crc->crc.u16[1])) & 1;

	    // calculate new CRC register value
	    // shifted_crc is copied to the bits of CRC register where generator polynomial bits == 0.
        BL_crc_shifter_t shifted = { crc->crc.u32 >> 1 };
        shifted.u16[0] &= ~CRC_INBIT_LE; // clear CRC register input bit
        if (mask)
        {
            shifted.u16[0] ^= crc->gen;
        }

        crc->crc.u32 = shifted.u32;
    }
}

void BL_crc_puts(pBL_crc_t crc, const uint8_t* px, uint32_t N)
{
    uint16_t* px16 = (uint16_t*)px;
    uint32_t N2 = N >> 1;
    uint32_t N_LSB = N & 1;
    for (uint32_t i = 0; i < N2; i++)
    {
        BL_crc_put16(crc, px16++);
    }
    if (N_LSB)
    {
        px = (uint8_t*)px16;
        BL_crc_put8(crc, px);
    }
}
