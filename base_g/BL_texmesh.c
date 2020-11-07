#include    "base_g/BL_fisheye.h"
#include    <math.h>

int BL_fisheye_sphericalview(const BL_2r32_t xy, BL_2r32_t phi_theta)
{
    static const float Qmax = M_PI * 50.0f/180.0f; // 50 = 45 + 5, 5 is margin outside 45 degrees
    static const float Rs = 1.0f/sinf(Qmax);
    static const float Rmax = 1.0f;
    int err = ESUCCESS;
    do {
        float r = sqrtf(xy[0]*xy[0] + xy[1]*xy[1]);
        if ((r < 1.0e-10f) || (r > Rmax))
        {
            err = ERANGE;
            break;
        }
        phi_theta[0] = atan2f(xy[1], xy[0]);
        phi_theta[1] = 2.0f * asinf(r / Rs);
    } while (0);
    return err;
}

int BL_fisheye_sphericalview_dirvec(const BL_2r32_t xy, BL_3r32_t vn)
{
    int err = ESUCCESS;
    do {
        BL_2r32_t phi_theta;
        if (ESUCCESS != (err = BL_fisheye_sphericalview(xy, phi_theta)))
        {
            break;
        }
        vn[0] = sinf(phi_theta[1]) * cosf(phi_theta[0]);
        vn[1] = sinf(phi_theta[1]) * sinf(phi_theta[0]);
        vn[2] = cosf(phi_theta[1]);
    } while (0);
    return err;
}