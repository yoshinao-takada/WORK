#ifndef BL_INTERSECT_H_
#define BL_INTERSECT_H_
#include    <base_l/BL_types.h>
#ifdef __cplusplus
extern "C" {
#endif
/*!
\struct BL_intersect_planeline_t
\brief configuration of plane-line intersection problem
*/
typedef struct {
    BL_3r32_t p0_line; // reference point on a line
    BL_3r32_t d_line; // unit direction vector of a line
    BL_3r32_t p1_plane; // reference point on a plane
    BL_3r32_t n_plane; // unit normal vector of a plane
} BL_intersect_planeline_t, *pBL_intersect_planeline_t;
typedef const BL_intersect_planeline_t *pcBL_intersect_planeline_t;

/*!
\brief calculate an intersection of a plane and a line
\param conf [in] problem configuration
\param p [out] intersection point
\return always ESUCCESS
*/
int BL_intersect_planeline(pcBL_intersect_planeline_t conf, BL_3r32_t p);

typedef struct {
    BL_3r32_t p0; // reference point
    BL_3r32_t d; // direction vector
} BL_line_t, *pBL_line_t;
typedef const BL_line_t *pcBL_line_t;

typedef struct {
    BL_3r32_t p0; // reference point
    BL_3r32_t n; // normal vector
} BL_plane_t, *pBL_plane_t;
typedef const BL_plane_t *pcBL_plane_t;

int BL_line_plane_intersect(pcBL_line_t line, pcBL_plane_t plane, BL_3r32_t p);

typedef struct {
    BL_2r32_t phi, gamma, delta;
} BL_linearintrinsic_t, *pBLBL_linearintrinsic_t;
typedef const BL_linearintrinsic_t *pcBL_linearintrinsic_t;

#ifdef __cplusplus
}
#endif
#endif
