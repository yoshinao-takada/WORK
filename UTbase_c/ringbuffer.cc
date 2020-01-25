#include <gtest/gtest.h>
#include "base_c/BL_ringbuffer.h"
#include "base_l/BL_base.h"

#define SUIT    ringbuffer
namespace
{
    /*!
    \brief write uint8_t single byte until full, read until empty
    */
    TEST(SUIT, scenario1)
    {
        uint8_t src[32];
        for (int i = 0; i < sizeof(src); i++)
        {
            src[i] = rand();
        }

        int i = 0;
        pBL_ringbuffer_t rb = BL_ringbuffer_new(8);
        while (ESUCCESS == BL_ringbuffer_putc(rb, src + i))
        {
            i++; // if a byte was successfully written, i++.
        }
        ASSERT_EQ(7, i);

        i = 0;
        uint8_t read_char;
        while (ESUCCESS == BL_ringbuffer_getc(rb, &read_char))
        {
            ASSERT_EQ(src[i], read_char);
            i++;
        }
        ASSERT_EQ(7, i);
        free(rb);
    }

    /*!
    \brief write byte sequence longer than the buffer capacity.
    read less than all and then read all.
    */
    TEST(SUIT, scenario2)
    {
        uint8_t src[32];
        for (int i = 0; i < sizeof(src); i++)
        {
            src[i] = rand();
        }

        int i = 0;
        pBL_ringbuffer_t rb = BL_ringbuffer_new(8);
        uint16_t to_write = sizeof(src);
        BL_ringbuffer_puts(rb, src, &to_write);
        ASSERT_EQ(sizeof(src) - 7, to_write);

        uint8_t read_char;
        for (int i = 0; i < 5; i++)
        {
            ASSERT_EQ(ESUCCESS, BL_ringbuffer_getc(rb, &read_char));
            ASSERT_EQ(src[i], read_char);
        }

        uint8_t read_buffer[8];
        uint16_t gets_reuired = sizeof(read_buffer);
        BL_ringbuffer_gets(rb, read_buffer, &gets_reuired);
        ASSERT_EQ(6, gets_reuired);
        for (int i = 0; i < 2; i++)
        {
            ASSERT_EQ(src[i], read_buffer[i]);
        }
        free(rb);
    }
}
