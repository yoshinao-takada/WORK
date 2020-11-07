#ifndef BL_FISHEYE_H_
#define BL_FISHEYE_H_
#include    <base_l/BL_base.h>
#include    <base_l/BL_types.h>
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief spherical mirror fisheye camera
    ^
    |   right area
    |
    \<-------------- 90 deg right side view
    |<==============> front view
    /<-------------- 90 deg left side view
    |
    |   left area
    v
    spherical mirror area
    spherical radius Rs: 1.0 = Rs * sin(Qmax), Qmax = PI * 45 / 180
    normal angle at reflected view radius r: Rs * sin(Q) = r, Q = asin(r/Rs)
    viewline direction vector at reflected view radius r: Q = 2*asin(r/Rs)
    camera origin : spherical mirror center
    reflected view direction : Z-
*/
/*!
\brief mapping function from canonical square plane to far spherical view
\param xy [in] canonical square plane coordinate
\param phi_theta [out] far spherical view coordinate angular parts
\return ESUCCESS: in valid range, ERANGE : out of range
*/
int BL_fisheye_sphericalview(const BL_2r32_t xy, BL_2r32_t phi_theta);

/*!
\brief mapping function from canonical square plane to far spherical view normalized direction vector
\param xy [in] canonical square plane coordinate
\param vn [out] normalized direction vector
\return ESUCCESS: in valid range, ERANGE : out of range
*/
int BL_fisheye_sphericalview_dirvec(const BL_2r32_t xy, BL_3r32_t vn);

#ifdef __cplusplus
}
#endif
#endif /* BL_FISHEYE_H_ */