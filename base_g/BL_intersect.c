#include    "base_g/BL_intersect.h"
#include    "base_g/BL_matexport.h"

int BL_intersect_planeline(pcBL_intersect_planeline_t conf, BL_3r32_t p)
{
    int err = ESUCCESS;
    do {
        pcBL_matfnr32RM_t fn = BL_matfnr32RM();
        BL_3r32_t p1_p0;
        BL_1r32_t dn, p1_p0_n;
        fn->mul(conf->d_line, conf->n_plane, &dn, 1, 3, 1);
        fn->sub(conf->p1_plane, conf->p0_line, p1_p0, 3, 1);
        fn->mul(p1_p0, conf->n_plane, &p1_p0_n, 1, 3, 1);
        BL_1r32_t coeff_D = p1_p0_n / dn;
        p[0] = conf->p0_line[0] + coeff_D * conf->d_line[0];
        p[1] = conf->p0_line[1] + coeff_D * conf->d_line[1];
        p[2] = conf->p0_line[2] + coeff_D * conf->d_line[2];
    } while (0);
    return err;
}

int BL_line_plane_intersect(pcBL_line_t line, pcBL_plane_t plane, BL_3r32_t p)
{
    int err = ESUCCESS;
    do {
        pcBL_matfnr32RM_t fn = BL_matfnr32RM();
        BL_3r32_t p1_p0;
        BL_1r32_t dn, p1_p0_n;
        fn->mul(line->d, plane->n, &dn, 1, 3, 1);
        fn->sub(plane->p0, line->p0, p1_p0, 3, 1);
        fn->mul(p1_p0, plane->n, &p1_p0_n, 1, 3, 1);
        BL_1r32_t coeff_D = p1_p0_n / dn;
        p[0] = line->p0[0] + coeff_D * line->d[0];
        p[1] = line->p0[1] + coeff_D * line->d[1];
        p[2] = line->p0[2] + coeff_D * line->d[2];
    } while (0);
    return err;
}
