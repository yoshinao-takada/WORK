#include    "base_g/BL_3Dline.h"
#include    "base_g/BL_matexport.h"

void BL_3Dline_set_direction(pBL_3Dline_t line, const BL_3r32_t dir_unnormalized)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    fn->normalize(dir_unnormalized, line->dir, 3);
}

void BL_3Dline_normalfoot(pcBL_3Dline_t line, const BL_3r32_t p, BL_3r32_t foot)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_3r32_t p0_p; // p - line->p0
    BL_1r32_t p0_p_dir; // <p - line->p0, dir>
    BL_3r32_t p0_p_dir_dir; // <p - line->p0, dir>dir
    fn->sub(p, line->p0, p0_p, 3, 1);
    fn->mul(p0_p, line->dir, &p0_p_dir, 1, 3, 1);
    fn->muls(line->dir, 3, 1, p0_p_dir, p0_p_dir_dir);
    fn->add(line->p0, p0_p_dir_dir, foot, 3, 1);
}

void BL_3Dline_distance(pcBL_3Dline_t line, const BL_3r32_t p, BL_1r32_t* distance)
{
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    BL_3r32_t foot, foot_to_p;
    BL_1r32_t sq_distance;
    BL_3Dline_normalfoot(line, p, foot);
    fn->sub(p, foot, foot_to_p, 3, 1);
    fn->mul(foot_to_p, foot_to_p, &sq_distance, 1, 3, 1);
    *distance = sqrtf(sq_distance);
}