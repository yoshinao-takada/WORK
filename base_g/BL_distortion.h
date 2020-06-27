#ifndef BL_DISTORTION_H_
#define BL_DISTORTION_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    BL_2r32_t src_center;
    BL_2r32_t dist_center;
    float kc, ks, k;
    BL_2u32_t wh;
} BL_dist_polymodel_t, *pBL_dist_polymodel_t;
typedef const BL_dist_polymodel_t *pcBL_dist_polymodel_t;

pBL_arrayMD_t BL_dist_polymodel_create_distmap(pcBL_dist_polymodel_t model);

pBL_arrayMD_t BL_disp_polymodel_create_undistmap(pcBL_dist_polymodel_t model);


#ifdef __cplusplus
}
#endif
#endif