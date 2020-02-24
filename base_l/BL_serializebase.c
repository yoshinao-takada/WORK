#include "base_l/BL_serializebase.h"
#include <stdio.h>
#include <stdlib.h>

#define ALLOCBUF(buf,cb_buf) \
    char* buf = (char*)malloc(cb_buf); \
    if (buf == NULL) return buf;

const char* BL_format_1i16(BL_cptr_t value)
{
    ALLOCBUF(buf, 8);
    snprintf(buf, 8, "%d", *value._1i16);
    return buf;
}

const char* BL_format_1u16(BL_cptr_t value)
{
    ALLOCBUF(buf, 8);
    snprintf(buf, 8, "%u", *value._1u16);
    return buf;
}

const char* BL_format_1i32(BL_cptr_t value)
{
    ALLOCBUF(buf, 16);
    snprintf(buf, 16, "%d", *value._1i32);
    return buf;
}

const char* BL_format_1u32(BL_cptr_t value)
{
    ALLOCBUF(buf, 16);
    snprintf(buf, 16, "%u", *value._1u32);
    return buf;
}

const char* BL_format_2i16(BL_cptr_t value)
{
    ALLOCBUF(buf, 16);
    snprintf(buf, 16, "%d,%d", (*value._2i16)[0], (*value._2i16)[1]);
    return buf;
}

const char* BL_format_2u16(BL_cptr_t value)
{
    ALLOCBUF(buf, 16);
    snprintf(buf, 16, "%u,%u", (*value._2u16)[0], (*value._2u16)[1]);
    return buf;
}

const char* BL_format_2i32(BL_cptr_t value)
{
    ALLOCBUF(buf, 32);
    snprintf(buf, 32, "%d,%d", (*value._2i32)[0], (*value._2i32)[1]);
    return buf;
}

const char* BL_format_2u32(BL_cptr_t value)
{
    ALLOCBUF(buf, 32);
    snprintf(buf, 32, "%u,%u", (*value._2u32)[0], (*value._2u32)[1]);
    return buf;
}

const char* BL_format_3i16(BL_cptr_t value)
{
    ALLOCBUF(buf, 24);
    snprintf(buf, 24, "%d,%d,%d", (*value._3i16)[0], (*value._3i16)[1], (*value._3i16)[2]);
    return buf;
}

const char* BL_format_3u16(BL_cptr_t value)
{
    ALLOCBUF(buf, 24);
    snprintf(buf, 24, "%u,%u,%u", (*value._3u16)[0], (*value._3u16)[1], (*value._3u16)[2]);
    return buf;
}

const char* BL_format_3i32(BL_cptr_t value)
{
    ALLOCBUF(buf, 48);
    snprintf(buf, 48, "%d,%d,%d", (*value._3i32)[0], (*value._3i32)[1], (*value._3i32)[2]);
    return buf;
}

const char* BL_format_3u32(BL_cptr_t value)
{
    ALLOCBUF(buf, 48);
    snprintf(buf, 48, "%u,%u,%u", (*value._3u32)[0], (*value._3u32)[1], (*value._3u32)[2]);
    return buf;
}

const char* BL_format_4i16(BL_cptr_t value)
{
    ALLOCBUF(buf, 32);
    snprintf(buf, 32, "%d,%d,%d,%d",
        (*value._4i16)[0], (*value._4i16)[1], (*value._4i16)[2], (*value._4i16)[3]);
    return buf;
}

const char* BL_format_4u16(BL_cptr_t value)
{
    ALLOCBUF(buf, 32);
    snprintf(buf, 32, "%u,%u,%u,%u",
        (*value._4u16)[0], (*value._4u16)[1], (*value._4u16)[2], (*value._4u16)[3]);
    return buf;
}

const char* BL_format_4i32(BL_cptr_t value)
{
    ALLOCBUF(buf, 64);
    snprintf(buf, 64, "%d,%d,%d,%d",
        (*value._4i32)[0], (*value._4i32)[1], (*value._4i32)[2], (*value._4i32)[3]);
    return buf;
}

const char* BL_format_4u32(BL_cptr_t value)
{
    ALLOCBUF(buf, 64);
    snprintf(buf, 64, "%u,%u,%u,%u",
        (*value._4u32)[0], (*value._4u32)[1], (*value._4u32)[2], (*value._4u32)[3]);
    return buf;
}
