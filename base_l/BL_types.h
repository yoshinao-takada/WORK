#ifndef BL_TYPES_H_
#define BL_TYPES_H_
#include <complex.h>
#ifdef __cplusplus
extern "C" {
#endif
#pragma region define_BL_basetypes
typedef int8_t      BL_1i8_t;
typedef int16_t      BL_1i16_t;
typedef int32_t      BL_1i32_t;
typedef int64_t      BL_1i64_t;
typedef uint8_t      BL_1u8_t;
typedef uint16_t      BL_1u16_t;
typedef uint32_t      BL_1u32_t;
typedef uint64_t      BL_1u64_t;
typedef float       BL_1r32_t;
typedef double      BL_1r64_t;
typedef long double BL_1r128_t;
typedef float _Complex BL_1c64_t;
typedef double _Complex BL_1c128_t;
typedef long double _Complex BL_1c256_t;

typedef BL_1i8_t        BL_2i8_t[2];
typedef BL_1i16_t        BL_2i16_t[2];
typedef BL_1i32_t        BL_2i32_t[2];
typedef BL_1i64_t        BL_2i64_t[2];
typedef BL_1u8_t        BL_2u8_t[2];
typedef BL_1u16_t        BL_2u16_t[2];
typedef BL_1u32_t        BL_2u32_t[2];
typedef BL_1u64_t        BL_2u64_t[2];
typedef BL_1r32_t        BL_2r32_t[2];
typedef BL_1r64_t        BL_2r64_t[2];
typedef BL_1r128_t        BL_2r128_t[2];
typedef BL_1c64_t        BL_2c64_t[2];
typedef BL_1c128_t        BL_2c128_t[2];
typedef BL_1c256_t        BL_2c256_t[2];

typedef BL_1i8_t        BL_3i8_t[3];
typedef BL_1i16_t        BL_3i16_t[3];
typedef BL_1i32_t        BL_3i32_t[3];
typedef BL_1i64_t        BL_3i64_t[3];
typedef BL_1u8_t        BL_3u8_t[3];
typedef BL_1u16_t        BL_3u16_t[3];
typedef BL_1u32_t        BL_3u32_t[3];
typedef BL_1u64_t        BL_3u64_t[3];
typedef BL_1r32_t        BL_3r32_t[3];
typedef BL_1r64_t        BL_3r64_t[3];
typedef BL_1r128_t        BL_3r128_t[3];
typedef BL_1c64_t        BL_3c64_t[3];
typedef BL_1c128_t        BL_3c128_t[3];
typedef BL_1c256_t        BL_3c256_t[3];

typedef BL_1i8_t        BL_4i8_t[4];
typedef BL_1i16_t        BL_4i16_t[4];
typedef BL_1i32_t        BL_4i32_t[4];
typedef BL_1i64_t        BL_4i64_t[4];
typedef BL_1u8_t        BL_4u8_t[4];
typedef BL_1u16_t        BL_4u16_t[4];
typedef BL_1u32_t        BL_4u32_t[4];
typedef BL_1u64_t        BL_4u64_t[4];
typedef BL_1r32_t        BL_4r32_t[4];
typedef BL_1r64_t        BL_4r64_t[4];
typedef BL_1r128_t        BL_4r128_t[4];
typedef BL_1c64_t        BL_4c64_t[4];
typedef BL_1c128_t        BL_4c128_t[4];
typedef BL_1c256_t        BL_4c256_t[4];
#pragma endregion

typedef union {
    BL_1i8_t    *_1i8;
    BL_1i16_t    *_1i16;
    BL_1i32_t    *_1i32;
    BL_1i64_t    *_1i64;
    BL_1u8_t    *_1u8;
    BL_1u16_t    *_1u16;
    BL_1u32_t    *_1u32;
    BL_1u64_t    *_1u64;
    BL_1r32_t    *_1r32;
    BL_1r64_t    *_1r64;
    BL_1r128_t    *_1r128;
    BL_1c64_t    *_1c64;
    BL_1c128_t    *_1c128;
    BL_1c256_t    *_1c256;

    BL_2i8_t    *_2i8;
    BL_2i16_t    *_2i16;
    BL_2i32_t    *_2i32;
    BL_2i64_t    *_2i64;
    BL_2u8_t    *_2u8;
    BL_2u16_t    *_2u16;
    BL_2u32_t    *_2u32;
    BL_2u64_t    *_2u64;
    BL_2r32_t    *_2r32;
    BL_2r64_t    *_2r64;
    BL_2r128_t    *_2r128;
    BL_2c64_t    *_2c64;
    BL_2c128_t    *_2c128;
    BL_2c256_t    *_2c256;

    BL_3i8_t    *_3i8;
    BL_3i16_t    *_3i16;
    BL_3i32_t    *_3i32;
    BL_3i64_t    *_3i64;
    BL_3u8_t    *_3u8;
    BL_3u16_t    *_3u16;
    BL_3u32_t    *_3u32;
    BL_3u64_t    *_3u64;
    BL_3r32_t    *_3r32;
    BL_3r64_t    *_3r64;
    BL_3r128_t    *_3r128;
    BL_3c64_t    *_3c64;
    BL_3c128_t    *_3c128;
    BL_3c256_t    *_3c256;

    BL_4i8_t    *_4i8;
    BL_4i16_t    *_4i16;
    BL_4i32_t    *_4i32;
    BL_4i64_t    *_4i64;
    BL_4u8_t    *_4u8;
    BL_4u16_t    *_4u16;
    BL_4u32_t    *_4u32;
    BL_4u64_t    *_4u64;
    BL_4r32_t    *_4r32;
    BL_4r64_t    *_4r64;
    BL_4r128_t    *_4r128;
    BL_4c64_t    *_4c64;
    BL_4c128_t    *_4c128;
    BL_4c256_t    *_4c256;
} BL_ptr_t, *pBL_ptr_t;

typedef const BL_ptr_t *pcBL_ptr_t;

typedef union {
    const BL_1i8_t    *_1i8;
    const BL_1i16_t    *_1i16;
    const BL_1i32_t    *_1i32;
    const BL_1i64_t    *_1i64;
    const BL_1u8_t    *_1u8;
    const BL_1u16_t    *_1u16;
    const BL_1u32_t    *_1u32;
    const BL_1u64_t    *_1u64;
    const BL_1r32_t    *_1r32;
    const BL_1r64_t    *_1r64;
    const BL_1r128_t    *_1r128;
    const BL_1c64_t    *_1c64;
    const BL_1c128_t    *_1c128;
    const BL_1c256_t    *_1c256;

    const BL_2i8_t    *_2i8;
    const BL_2i16_t    *_2i16;
    const BL_2i32_t    *_2i32;
    const BL_2i64_t    *_2i64;
    const BL_2u8_t    *_2u8;
    const BL_2u16_t    *_2u16;
    const BL_2u32_t    *_2u32;
    const BL_2u64_t    *_2u64;
    const BL_2r32_t    *_2r32;
    const BL_2r64_t    *_2r64;
    const BL_2r128_t    *_2r128;
    const BL_2c64_t    *_2c64;
    const BL_2c128_t    *_2c128;
    const BL_2c256_t    *_2c256;

    const BL_3i8_t    *_3i8;
    const BL_3i16_t    *_3i16;
    const BL_3i32_t    *_3i32;
    const BL_3i64_t    *_3i64;
    const BL_3u8_t    *_3u8;
    const BL_3u16_t    *_3u16;
    const BL_3u32_t    *_3u32;
    const BL_3u64_t    *_3u64;
    const BL_3r32_t    *_3r32;
    const BL_3r64_t    *_3r64;
    const BL_3r128_t    *_3r128;
    const BL_3c64_t    *_3c64;
    const BL_3c128_t    *_3c128;
    const BL_3c256_t    *_3c256;

    const BL_4i8_t    *_4i8;
    const BL_4i16_t    *_4i16;
    const BL_4i32_t    *_4i32;
    const BL_4i64_t    *_4i64;
    const BL_4u8_t    *_4u8;
    const BL_4u16_t    *_4u16;
    const BL_4u32_t    *_4u32;
    const BL_4u64_t    *_4u64;
    const BL_4r32_t    *_4r32;
    const BL_4r64_t    *_4r64;
    const BL_4r128_t    *_4r128;
    const BL_4c64_t    *_4c64;
    const BL_4c128_t    *_4c128;
    const BL_4c256_t    *_4c256;
} BL_cptr_t, *pBL_cptr_t;

typedef const BL_cptr_t *pcBL_cptr_t;

#ifdef __cplusplus
}
#endif
#endif