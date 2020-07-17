#include    <gtest/gtest.h>
#include    <base_l/BL_rational.h>
#define     SUIT    rational

namespace
{
    TEST(SUIT, rational1)
    {
        static const char* fmt[] = { "%u/%u", ", %u/%u"};
        BL_2u32_t r ;
        for (uint32_t row = 0; row != 8; row++)
        {
            for (uint32_t pos = 0; pos <= row; pos++)
            {
                BL_rational_create(row, pos, r);
                printf((pos == 0) ? fmt[0] : fmt[1], r[0], r[1]);
            }
            printf("\n");
        }
    }
}