#ifndef BL_TYPES_H_
#define BL_TYPES_H_
#include <stdint.h>
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
typedef intptr_t    BL_1iptr_t;
typedef uintptr_t   BL_1uptr_t;

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
typedef BL_1iptr_t      BL_2iptr_t[2];
typedef BL_1uptr_t      BL_2uptr_t[2];

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
typedef BL_1iptr_t      BL_3iptr_t[3];
typedef BL_1uptr_t      BL_3uptr_t[3];

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
typedef BL_1iptr_t      BL_4iptr_t[4];
typedef BL_1uptr_t      BL_4uptr_t[4];

typedef BL_1i8_t        BL_5i8_t[5];
typedef BL_1i16_t        BL_5i16_t[5];
typedef BL_1i32_t        BL_5i32_t[5];
typedef BL_1i64_t        BL_5i64_t[5];
typedef BL_1u8_t        BL_5u8_t[5];
typedef BL_1u16_t        BL_5u16_t[5];
typedef BL_1u32_t        BL_5u32_t[5];
typedef BL_1u64_t        BL_5u64_t[5];
typedef BL_1r32_t        BL_5r32_t[5];
typedef BL_1r64_t        BL_5r64_t[5];
typedef BL_1r128_t        BL_5r128_t[5];
typedef BL_1c64_t        BL_5c64_t[5];
typedef BL_1c128_t        BL_5c128_t[5];
typedef BL_1c256_t        BL_5c256_t[5];
typedef BL_1iptr_t      BL_5iptr_t[5];
typedef BL_1uptr_t      BL_5uptr_t[5];

typedef BL_1i8_t        BL_6i8_t[6];
typedef BL_1i16_t        BL_6i16_t[6];
typedef BL_1i32_t        BL_6i32_t[6];
typedef BL_1i64_t        BL_6i64_t[6];
typedef BL_1u8_t        BL_6u8_t[6];
typedef BL_1u16_t        BL_6u16_t[6];
typedef BL_1u32_t        BL_6u32_t[6];
typedef BL_1u64_t        BL_6u64_t[6];
typedef BL_1r32_t        BL_6r32_t[6];
typedef BL_1r64_t        BL_6r64_t[6];
typedef BL_1r128_t        BL_6r128_t[6];
typedef BL_1c64_t        BL_6c64_t[6];
typedef BL_1c128_t        BL_6c128_t[6];
typedef BL_1c256_t        BL_6c256_t[6];
typedef BL_1iptr_t      BL_6iptr_t[6];
typedef BL_1uptr_t      BL_6uptr_t[6];
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
    BL_1iptr_t  *_1iptr;
    BL_1uptr_t  *_1uptr;

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
    BL_2iptr_t  *_2iptr;
    BL_2uptr_t  *_2uptr;

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
    BL_3iptr_t  *_3iptr;
    BL_3uptr_t  *_3uptr;

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
    BL_4iptr_t  *_4iptr;
    BL_4uptr_t  *_4uptr;

    BL_5i8_t    *_5i8;
    BL_5i16_t    *_5i16;
    BL_5i32_t    *_5i32;
    BL_5i64_t    *_5i64;
    BL_5u8_t    *_5u8;
    BL_5u16_t    *_5u16;
    BL_5u32_t    *_5u32;
    BL_5u64_t    *_5u64;
    BL_5r32_t    *_5r32;
    BL_5r64_t    *_5r64;
    BL_5r128_t    *_5r128;
    BL_5c64_t    *_5c64;
    BL_5c128_t    *_5c128;
    BL_5c256_t    *_5c256;
    BL_5iptr_t  *_5iptr;
    BL_5uptr_t  *_5uptr;

    BL_6i8_t    *_6i8;
    BL_6i16_t    *_6i16;
    BL_6i32_t    *_6i32;
    BL_6i64_t    *_6i64;
    BL_6u8_t    *_6u8;
    BL_6u16_t    *_6u16;
    BL_6u32_t    *_6u32;
    BL_6u64_t    *_6u64;
    BL_6r32_t    *_6r32;
    BL_6r64_t    *_6r64;
    BL_6r128_t    *_6r128;
    BL_6c64_t    *_6c64;
    BL_6c128_t    *_6c128;
    BL_6c256_t    *_6c256;
    BL_6iptr_t  *_6iptr;
    BL_6uptr_t  *_6uptr;
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
    const BL_1iptr_t  *_1iptr;
    const BL_1uptr_t  *_1uptr;

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
    const BL_2iptr_t  *_2iptr;
    const BL_2uptr_t  *_2uptr;

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
    const BL_3iptr_t  *_3iptr;
    const BL_3uptr_t  *_3uptr;

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
    const BL_4iptr_t  *_4iptr;
    const BL_4uptr_t  *_4uptr;

    const BL_5i8_t    *_5i8;
    const BL_5i16_t    *_5i16;
    const BL_5i32_t    *_5i32;
    const BL_5i64_t    *_5i64;
    const BL_5u8_t    *_5u8;
    const BL_5u16_t    *_5u16;
    const BL_5u32_t    *_5u32;
    const BL_5u64_t    *_5u64;
    const BL_5r32_t    *_5r32;
    const BL_5r64_t    *_5r64;
    const BL_5r128_t    *_5r128;
    const BL_5c64_t    *_5c64;
    const BL_5c128_t    *_5c128;
    const BL_5c256_t    *_5c256;
    const BL_5iptr_t  *_5iptr;
    const BL_5uptr_t  *_5uptr;

    const BL_6i8_t    *_6i8;
    const BL_6i16_t    *_6i16;
    const BL_6i32_t    *_6i32;
    const BL_6i64_t    *_6i64;
    const BL_6u8_t    *_6u8;
    const BL_6u16_t    *_6u16;
    const BL_6u32_t    *_6u32;
    const BL_6u64_t    *_6u64;
    const BL_6r32_t    *_6r32;
    const BL_6r64_t    *_6r64;
    const BL_6r128_t    *_6r128;
    const BL_6c64_t    *_6c64;
    const BL_6c128_t    *_6c128;
    const BL_6c256_t    *_6c256;
    const BL_6iptr_t  *_6iptr;
    const BL_6uptr_t  *_6uptr;
} BL_cptr_t, *pBL_cptr_t;

typedef const BL_cptr_t *pcBL_cptr_t;

#define BL_copy2(dst,src) { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; }
#define BL_copy3(dst,src) { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; }
#define BL_copy4(dst,src) { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; (dst)[3] = (src)[3]; }
#define BL_copy5(dst,src) { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; (dst)[3] = (src)[3]; \
    (dst)[4] = (src)[4]; }
#define BL_copy6(dst,src) { (dst)[0] = (src)[0]; (dst)[1] = (src)[1]; (dst)[2] = (src)[2]; (dst)[3] = (src)[3]; \
    (dst)[4] = (src)[4]; (dst)[5] = (src)[5]; }
#define BL_copy_array(dst,src,count)    {for (uint32_t i___ = 0; i___ != count; i___++) { (dst)[i___] = (src)[i___]; }}
#define BL_copy9(dst,src)   BL_copy_array(dst,src,9)
#define BL_copy16(dst,src)  BL_copy_array(dst,src,16)

#define BL_accumulate_array(dst,src,count)   {for (uint32_t i___ = 0; i___ != count; i___++) { (dst)[i___] += (src)[i___]; }}
#define BL_accumulate2(dst,src) { (dst)[0]+=(src)[0]; (dst)[1]+=(src)[1]; }
#define BL_accumulate3(dst,src) { (dst)[0]+=(src)[0]; (dst)[1]+=(src)[1]; (dst)[2]+=(src)[2]; }
#define BL_accumulate4(dst,src) { (dst)[0]+=(src)[0]; (dst)[1]+=(src)[1]; (dst)[2]+=(src)[2]; (dst)[3]+=(src)[3]; }

#define BL_is_in_rect(index2D,rect) ( \
    ( \
        (index2D[0] < rect[0]) || (index2D[1] < rect[1]) || \
        (index2D[0] >= (rect[0] + rect[2])) || (index2D[1] >= (rect[1] + rect[3])) \
    ) ? ERANGE : ESUCCESS \
)
#ifdef __cplusplus
}
#endif
#endif