#ifndef BL_3DPLANE_H_
#define BL_3DPLANE_H_
#include    "base_l/BL_types.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
    BL_3r32_t normal; // normalized normal vector
    BL_3r32_t p0; // a point on the plane
} BL_3Dplane_t, *pBL_3Dplane_t;
typedef const BL_3Dplane_t *pcBL_3Dplane_t;

/*!
\brief get the foot of the normal line drawn from p to plane.
\param plane [in] reference plane
\param p [in] a point which the normal line is drawn from
\param foot [out] the foot of the normal line.
*/
void BL_3Dplane_normalfoot(pcBL_3Dplane_t plane, const BL_3r32_t p, BL_3r32_t foot);

/*!
\brief get the signed offset from p to plane. positive signe means that p is in the half space of plane->normal points.
\param plane [in] reference plane
\param p [in] a point which the normal line is drawn from
\param offset [out]
*/
void BL_3Dplane_offset(pcBL_3Dplane_t plane, const BL_3r32_t p, BL_1r32_t *offset);
#ifdef __cplusplus
}
#endif
#endif /* BL_3DPLANE_H_ */