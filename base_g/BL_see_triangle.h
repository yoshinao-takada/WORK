#ifndef BL_SEE_TRIANGLE_H_
#define BL_SEE_TRIANGLE_H_
#include    <base_l/BL_base.h>
#include    <base_l/BL_types.h>
#include    <base_g/BL_3Dline.h>
#include    <base_g/BL_texture.h>
#include    <base_g/BL_bilinearinterp.h>
#ifdef  __cplusplus
extern "C" {
#endif
typedef struct {
    pcBL_bilinearinterp_t interpolator;

    BL_3r32_t vert_coords[3];
    BL_2r32_t tex_coords[3];
    
} BL_see_triangle_t, *pBL_see_triangle_t;

typedef const BL_see_triangle_t *pcBL_see_triangle_t;

#ifdef  __cplusplus
}
#endif
#endif /* BL_SEE_TRIANGLE_H_ */