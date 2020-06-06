#ifndef BL_DIST_H_
#define BL_DIST_H_
#include    "base_l/BL_containers.h"
#ifdef  __cplusplus
extern "C" {
#endif
/*
Purpose: defining 2D-2D distortion properties
Type of distortion
(1) rotation: defined by center coordinate and rotation angle
(2) shift: defined by a vector
(3) circularily symmetrical nonlinearity: defined by center coordinates and coefficients of a polynomial of radius
*/

typedef enum {
    BL_disttype_rotation,
    BL_disttype_shift,
    BL_disttype_csnl, // circularily symmetrical nonlinearlity
} BL_disttype_t, *pBL_disttype_t;
typedef const BL_disttype_t *pcBL_disttype_t;

typedef int (*BL_DIST_OPERATION)(pcBL_disttype_t dist, pcBL_arrayMD_t in, pBL_arrayMD_t out);

typedef struct {
    BL_disttype_t base;
    BL_2r32_t normalized_center; // (Xc/width, Yc/height), top-left origin
    BL_1r32_t rotangle_degs; // rotation angle represented in degrees
} BL_dist_rotation_t, *pBL_dist_rotation_t;
typedef const BL_dist_rotation_t *pcBL_dist_rotation_t;

typedef struct {
    BL_disttype_t base;
    BL_2r32_t normalized_shift; // (Vx/width, Vy/height) (V: shift vector, (width, height): destination iamge size)
} BL_dist_shift_t, *pBL_dist_shift_t;
typedef const BL_dist_shift_t *pcBL_dist_shift_t;

typedef struct {
    BL_disttype_t base;
    BL_1r32_t k; // Rsrc = Rdst * (1 + (k/width^2)*Rdst^2): width is of desitination image size
} BL_dist_csnl_t, *pBL_dist_csnl_t;
typedef const BL_dist_csnl_t *pcBL_dist_csnl_t;

#ifdef  __cplusplus
}
#endif
#endif
