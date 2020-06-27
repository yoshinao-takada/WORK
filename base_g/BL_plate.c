#include    "base_g/BL_plate.h"
#include    <base_l/BL_errno.h>
#include    "base_g/BL_matexport.h"

static pcBL_matfnr32RM_t matfn = NULL;

void    BL_plate_init(
    pBL_plate_t obj,
    const BL_3r32_t rot_angle_deg,
    const BL_3r32_t translate,
    const BL_2r32_t wh)
{
    BL_1r32_t work[16*2];
    if (matfn == NULL)
    { // init function table if not initialized
        matfn = BL_matfnr32RM();
    }
    { // calculate 1st half of transform matrices
        BL_1r32_t rotx[16], roty[16];
        matfn->rotxdeg(rot_angle_deg[0], rotx);
        matfn->rotydeg(rot_angle_deg[1], roty);
        matfn->mul(roty, rotx, work, 4,4,4);
    }
    { // calculate 2nd half
        BL_1r32_t rotz[16], trans[16];
        matfn->rotzdeg(rot_angle_deg[2], rotz);
        matfn->translate(translate, trans);
        matfn->mul(trans, rotz, work+16, 4, 4, 4);
    }
    matfn->mul(work+16,work,obj->from_canonical, 4, 4, 4); // combine the 1st half and the 2nd half
    { // create the inverted matrix
        matfn->inv(obj->from_canonical, obj->to_canonical, work, 4);
    }
    obj->h_limits[0] = wh[0] * 0.5f;
    obj->h_limits[1] = -obj->h_limits[0];
    obj->v_limits[0] = wh[1] * 0.5f;
    obj->v_limits[1] = -obj->v_limits[0];
}

int     BL_plate_objectcoord(pcBL_plate_t obj, const BL_2r32_t uv, BL_3r32_t xyz)
{
    BL_4r32_t uv_ = { uv[0], uv[1], 0.0f, 1.0f };
    BL_4r32_t xyz_;
    int err = BL_plate_objectcoordH(obj, uv_, xyz_);
    BL_copy3(xyz, xyz_);
    return err;
}

int     BL_plate_objectcoordH(pcBL_plate_t obj, const BL_4r32_t uv, BL_4r32_t xyz)
{
    int err = ESUCCESS;
    do {
        if ((uv[0] < obj->h_limits[0]) || (uv[0] >= obj->h_limits[1]) ||
            (uv[1] < obj->v_limits[0]) || (uv[1] >= obj->v_limits[1]))
        {
            err = ERR_GEO_RANGE;
        }
        matfn->mul(obj->from_canonical, uv, xyz, 1, 4, 4);
    } while (0);
    return err;
}

int     BL_plate_canonicalcoord(pcBL_plate_t obj, const BL_3r32_t xyz, BL_1r32_t ztol, BL_3r32_t uv)
{
    BL_4r32_t xyz_ = { xyz[0], xyz[1], xyz[2], 1.0f };
    BL_4r32_t uv_;
    int err = BL_plate_canonicalcoordH(obj, xyz_, ztol, uv_);
    BL_copy3(uv, uv_);
    return err;
}

int     BL_plate_canonicalcoordH(pcBL_plate_t obj, const BL_4r32_t xyz, BL_1r32_t ztol, BL_4r32_t uv)
{
    int err = ESUCCESS;
    do {
        matfn->mul(obj->to_canonical, xyz, uv, 1,4, 4);
        if ((uv[0] < obj->h_limits[0]) || (uv[0] >= obj->h_limits[1]) ||
            (uv[1] < obj->v_limits[0]) || (uv[1] >= obj->v_limits[1]))
        {
            err = ERR_GEO_RANGE;
        }
        if ((uv[2] < -ztol) || (uv[2] >= ztol))
        {
            err = ERR_GEO_DEPTH_RANGE;
        }
    } while (0);
    return err;
}

