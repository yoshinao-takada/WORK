#ifndef BL_TEXTURE_H_
#define BL_TEXTURE_H_
#include    <base_l/BL_containers.h>
#include    "base_g/BL_distortion.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef BL_arrayMD_t BL_texture_t,*pBL_texture_t;
typedef const BL_texture_t *pcBL_texture_t;

/*!
\brief create a texture object. the texture is an ordinary pixel map.
Its top-left corner is pixel(0,0). bottom-right corner is pixel(wh[0]-1,wh[1]-1)
*/
pBL_texture_t BL_texture_alloc(const BL_2u32_t wh, bool with_alpha);

typedef struct {
    bool with_alpha; // turn on/off alpha channel, false: 3CH, true: 4CH
    BL_2u32_t wh, block_size; // wh: width and height, block_size: checker pattern block size
    BL_4u8_t colors[2]; // two colors of the checker pattern
} BL_checkerconf_t, *pBL_checkerconf_t;
typedef const BL_checkerconf_t *pcBL_checkerconf_t;

typedef struct {
    BL_checkerconf_t base; // upper-left part of border is defined by base
    BL_2u32_t border; // border pixel coordinate of upper-left part.
    BL_4u8_t colors[2]; // right, or lower part of border is defined by color2 and color3.
} BL_checkerconf2_t, *pBL_checkerconf2_t;
typedef const BL_checkerconf2_t *pcBL_checkerconf2_t;

/*!
\brief create a checker pattern texture
\param conf [in] checker pattern configuration info.
\return a pointer to a newly created texture
*/
pBL_texture_t BL_texture_checker(pcBL_checkerconf_t conf);

/*!
\brief create a checker pattern texture
\param conf [in] checker pattern configuration info.
\return a pointer to a newly created texture
*/
pBL_texture_t BL_texture_checker2(pcBL_checkerconf2_t conf);

/*!
\brief remap an image to another image
\param in [in] source image
\param out [out] remapped image
\param map [in] mapping table
\return unix errno compatible number
*/
int BL_texture_remap(pBL_texture_t in, pBL_texture_t out, pBL_arrayMD_t map);

/*!
\brief create a texture image of uniformly aligned circular dots
\param conf [in] configuration parameters
\return created image
*/
pBL_texture_t BL_texture_circulardots(pcBL_checkerconf_t conf);

pBL_texture_t BL_texture_circulardots2(pcBL_checkerconf2_t conf);
#ifdef __cplusplus
}
#endif
#endif
