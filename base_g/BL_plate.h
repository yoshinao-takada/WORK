#ifndef BL_PLATE_H_
#define BL_PLATE_H_
#include    <base_l/BL_types.h>
#ifdef __cplusplus
extern "C" {
#endif
/*!
\struct BL_plate_t
\brief It represents the relationship between the canonical plane (z = 0) and object plane
which is defined by a point and a normal vector. The point can be at anywhere in the world
coordinate. The normal vector can be any value.
The relationship is defined as follows
Step 1: the canonical plane is rotated around x, y, and z axes.
Step 2: the rotated plane is moved by a translate vector.
The plate exists within the range of wh;
 i.e. -wh[0]/2 <= x < wh[0]/2, -wh[1] <= y < wh[1]/2 on the canonical plane.
*/
typedef struct {
    BL_2r32_t h_limits; // horizontal range in the canonical plane
    BL_2r32_t v_limits; // vertical range in the canonical plane
    BL_1r32_t to_canonical[16], from_canonical[16];
} BL_plate_t, *pBL_plate_t;
typedef const BL_plate_t *pcBL_plate_t;

/*!
\brief Initialize obj.
\param rot_angle_deg [in] rotation angle from the canonical plane to the object plane
\param translate [in] translate vector from the canonical plane to the object plane
\param wh [in] object plante existing range
*/
void    BL_plate_init(
    pBL_plate_t obj,
    const BL_3r32_t rot_angle_deg,
    const BL_3r32_t translate,
    const BL_2r32_t wh);

/*!
\brief convert a coordinate on the canonical plane to that on object plane
\param obj [in] plate object
\param uv [in] xy coordinate on the canonical plane
\param xyz [out] 3D coordinate on the object plane
\return ERR_GEO_RANGE or ESUCCESS
*/
int     BL_plate_objectcoord(pcBL_plate_t obj, const BL_2r32_t uv, BL_3r32_t xyz);

/*!
\brief homogeneous coordinte version of BL_plate_objectcoord()
*/
int     BL_plate_objectcoordH(pcBL_plate_t obj, const BL_4r32_t uv, BL_4r32_t xyz);

/*!
\brief convert a coordinate on the object to the canonical plane
\param obj [in] plate object
\param xyz [in] 3D coordinate on the plate object
\param ztol [in] acceptable z-offset after conversion from the canonical plane
\param uv [out] 3D coordinate on the canoncal plane
\return ERR_GEO_RANGE, ERR_GEO_DEPTH_RANGE, or ESUCCESS
*/
int     BL_plate_canonicalcoord(pcBL_plate_t obj, const BL_3r32_t xyz, BL_1r32_t ztol, BL_3r32_t uv);

/*!
\brief homogeneous coordinte version of BL_plate_canonicalcoord()
*/
int     BL_plate_canonicalcoordH(pcBL_plate_t obj, const BL_4r32_t xyz, BL_1r32_t ztol, BL_4r32_t uv);

#ifdef __cplusplus
}
#endif
#endif
