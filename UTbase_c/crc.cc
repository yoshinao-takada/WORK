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
}
