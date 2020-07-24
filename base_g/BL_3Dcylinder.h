#ifndef BL_3DCYLINDER_H_
#define BL_3DCYLINDER_H_
#include    "base_g/BL_3Dline.h"
#ifdef  __cplusplus
extern "C" {
#endif
/*!
\brief cylinder as a mathematical 3D object for computational geometry
*/
typedef struct {
    BL_3Dline_t ax;
    BL_1r32_t r; // radius
} BL_3Dcylinder_t, *pBL_3Dcylinder_t;
typedef const BL_3Dcylinder_t *pcBL_3Dcylinder_t;

/*!
\brief get a foot of a normal line drawn from p to the cylinder axis
\param cyl [in] a cylinder
\param p [in] a point which a normal line is drawn from
\param foot [out] the foot of the normal line
*/
void BL_3Dcylinder_normalfoot_on_axis(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_3r32_t foot);

/*!
\brief get a foot of a normal line drawn from p to the cylinder surface
\param cyl [in] a cylinder
\param p [in] a point which a normal line is drawn from
\param foot [out] the foot of the normal line
*/
void BL_3Dcylinder_normalfoot_on_surface(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_3r32_t foot);

/*!
\brief get the distance between the cylinder axis and p
\param cyl [in] a cylinder
\param p [in] a point which a normal line is drawn from
\param distance [out]
*/
void BL_3Dcylinder_distance_from_axis(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_1r32_t* distance);

/*!
\brief get the signed offset from the foot on the cylinder surface to p. positive sign means outside.
\param cyl [in] a cylinder
\param p [in] a point which a normal line is drawn from
\param offset [out]
*/
void BL_3Dcylinder_offset_from_surface(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_1r32_t* offset);
#ifdef  __cplusplus
}
#endif
#endif /* BL_3DCYLINDER_H_ */
