#include <gtest/gtest.h>
#include "base_c/BL_crc.h"
#include "base_l/BL_base.h"

#define SUIT    crc
namespace 
{
    TEST(SUIT, crc)
    {
        uint8_t data8[] = { 0x01, 0x12, 0x14, 0xff, 0x89 };
        uint16_t data0 = 1;
        BL_crc_t crc;
        BL_crc_init(&crc, CRC_CCITT_LE, 0);
        BL_crc_puts(&crc, data8, ARRAYSIZE(data8));
        uint16_t crc_temp = crc.crc.u16[0];
        ASSERT_EQ(true, crc_temp != 0);
        BL_crc_put16(&crc, &crc_temp);
        ASSERT_EQ((uint16_t)0, crc.crc.u16[0]);
    }

    int count_bits(uint16_t i)
    {
        int icount = 0;
        for (uint16_t imask = 1; imask != 0; imask <<= 1)
        {
            icount += ((imask & i) != 0) ? 1 : 0;
        }
        return icount;
    }

    TEST(SUIT, crc_err)
    {
        uint16_t data16[] = { 0xff1f, 0x0010, 0x1234, 0xabcd };
        uint16_t err_bits[] = { 0x0010, 0, 0, 0 };

        BL_crc_t crc;
        BL_crc_init(&crc, CRC_CCITT_LE, 0);
        BL_crc_puts(&crc, (const uint8_t*)data16, sizeof(data16));
        uint16_t crc_temp = crc.crc.u16[0];
        ASSERT_NE(0, crc_temp);
        BL_crc_put16(&crc, &crc_temp);
        ASSERT_EQ((uint16_t)0, crc.crc.u16[0]);
        for (int i = 0; i < ARRAYSIZE(data16); i++)
        {
            data16[i] ^= err_bits[i];
        }
        BL_crc_init(&crc, CRC_CCITT_LE, 0);
        BL_crc_puts(&crc, (const uint8_t*)data16, sizeof(data16));
        BL_crc_put16(&crc, &crc_temp);
        printf("crc = 0x%04x\n", crc.crc.u16[0]);
        int on_bit = count_bits(crc.crc.u16[0]);
        ASSERT_GE(on_bit, 7);;        
    }
}
