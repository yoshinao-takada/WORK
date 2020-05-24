#ifndef BL_DISTORTION_H_
#define BL_DISTORTION_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
    typedef BL_arrayMD_t    BL_map2D_t,*pBL_map2D_t;
    typedef const BL_map2D_t    *pcBL_map2D_t;
    pBL_map2D_t alloc_map2D(const BL_2u32_t wh);

    typedef struct {
        BL_2u32_t wh; // width, height of mapping table, which is equal to that of a destination image.
        BL_2r32_t rot_center; // 1st stage transform parameter, rotaton center
        float rot_angle_deg; // 1st stage transform parameter, rotation angle
        BL_2r32_t shift; // 2nd stage transform parameter
        BL_2r32_t c_to, c_from; // center of the source and the destination images
        BL_2r32_t k; // extra extension factor in x and y direction
    } BL_distconf_t, *pBL_distconf_t;
    typedef const BL_distconf_t *pcBL_distconf_t;

    /*!
    \brief remap an image in non-linear manner
    remap step 1: rotation
    remap step 2: shift
    remap step 3: non-linear mapping
    \param conf [in] transform configuration
    \return mapping table for 2D textures
    */
    pBL_map2D_t BL_distortion_createmap(pcBL_distconf_t conf);

    /*!
    \brief configuration struct for creating remap table of Step1) rotating, Step2) shifting
    */
    typedef struct {
        BL_2u32_t wh_dst; // width and height of mapping table and destination image.
        BL_2u32_t wh_src; // width and height of source image
        float rot_angle_deg; // rotation angle in degrees.
        BL_2r32_t shift; // shift vector
    } BL_rotshift_t, *pBL_rotshift_t;
    typedef const BL_rotshift_t *pcBL_rotshift_t;

    pBL_map2D_t BL_rotshift_createmap(pcBL_rotshift_t conf);

    typedef struct {
        BL_2u32_t wh_dst; // width and height of mapping table and destination image.
        BL_2u32_t wh_src; // width and height of source image
        BL_2r32_t c_dist; // distortion center (a point where distortion vanishes)
        BL_2r32_t k; // extra extension factor in x and y direction. negative values means extra shrinking.
    } BL_rotshiftdist_t, *pBL_rotshiftdist_t;
    typedef const BL_rotshiftdist_t *pcBL_rotshiftdist_t;

    pBL_map2D_t BL_rotshiftdist_createmap(pcBL_rotshiftdist_t conf);
#ifdef __cplusplus
}
#endif
#endif