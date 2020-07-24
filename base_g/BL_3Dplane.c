#include    "base_g/BL_3Dplane.h"
#include    "base_g/BL_matexport.h"

void BL_3Dplane_normalfoot(pcBL_3Dplane_t plane, const BL_3r32_t p, BL_3r32_t foot)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_3r32_t p0_to_p; // p - plane->p0
    BL_1r32_t p0_to_p_normal; // <p - plane->p0, plane->normal>
    BL_3r32_t p0_to_p_normal_normal; //<p - plane->p0, plane->normal> plane->normal
    fn->sub(p, plane->p0, p0_to_p, 3, 1);
    fn->mul(p0_to_p, plane->normal, &p0_to_p_normal, 1, 3, 1);
    fn->muls(plane->normal, 3, 1, p0_to_p_normal, p0_to_p_normal_normal);
    fn->sub(p, p0_to_p_normal_normal, foot, 3, 1);
}

void BL_3Dplane_offset(pcBL_3Dplane_t plane, const BL_3r32_t p, BL_1r32_t *offset)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_3r32_t foot, foot_to_p;
    BL_3Dplane_normalfoot(plane, p, foot);
    fn->sub(p, foot, foot_to_p, 3, 1);
    fn->mul(foot_to_p, plane->normal, offset, 1, 3, 1);
}
