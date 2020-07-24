#include    "base_g/BL_3Dcylinder.h"
#include    "base_g/BL_matexport.h"

void BL_3Dcylinder_normalfoot_on_axis(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_3r32_t foot)
{
    BL_3Dline_normalfoot(&cyl->ax, p, foot);
}

void BL_3Dcylinder_normalfoot_on_surface(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_3r32_t foot)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_3r32_t foot_on_axis, foot_on_axis_to_p, foot_on_axis_to_p_dir, foot_on_axis_to_surface;
    BL_3Dcylinder_normalfoot_on_axis(cyl, p, foot_on_axis);
    fn->sub(p, foot_on_axis, foot_on_axis_to_p, 3, 1);
    fn->normalize(foot_on_axis_to_p, foot_on_axis_to_p_dir, 3);
    fn->muls(foot_on_axis_to_p_dir, 3, 1, cyl->r, foot_on_axis_to_surface);
    fn->add(foot_on_axis, foot_on_axis_to_surface, foot, 3, 1);
}

void BL_3Dcylinder_distance_from_axis(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_1r32_t* distance)
{
    BL_3Dline_distance(&cyl->ax, p, distance);
}

void BL_3Dcylinder_offset_from_surface(pcBL_3Dcylinder_t cyl, const BL_3r32_t p, BL_1r32_t* offset)
{
    BL_3Dcylinder_distance_from_axis(cyl, p, offset);
    *offset -= cyl->r;
}