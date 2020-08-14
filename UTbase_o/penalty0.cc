#include    <gtest/gtest.h>
#include    <base_o/BL_penalty.h>
#include    <base_o/BL_dfo.h>
#include    <base_g/BL_3Dplane.h>
#include    <base_g/BL_3Dcylinder.h>
#include    <base_g/BL_matexport.h>
#define SUIT                    penalty0
#define CYLINDER_DIRECTION      { sqrtf(0.5f), 0.0f, sqrtf(0.5f) }
#define CYLINDER_REFPOINT       { 0.0f, 1.0f, 2.0f }
#define CYLINDER_RADUS          0.5f
#define PLANE_NORMAL            CYLINDER_DIRECTION
#define PLANE_REFPOINT          CYLINDER_REFPOINT

namespace {
    static pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    static BL_3Dplane_t plane = { PLANE_NORMAL, PLANE_REFPOINT };
    static BL_3Dcylinder_t cyl = { { CYLINDER_DIRECTION, CYLINDER_REFPOINT }, CYLINDER_RADUS };

    static int EQC_onplane(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        BL_3Dplane_offset((pcBL_3Dplane_t)params, xv, pf);
        return 0;
    }

    static int EQC_oncylinder(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        BL_3Dcylinder_offset_from_surface((pcBL_3Dcylinder_t)params, xv, pf);
        return 0;
    }

    static int OBJECTIVE_sq_zcomponent(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        static const BL_3r32_t z_normal = { 0.0f, 0.0f, 1.0f };
        fn->mul(xv, z_normal, pf, 1, 3, 1);
        *pf *= *pf;
        return 0;
    }

    TEST(SUIT, solve)
    {
        BL_1r32_t eqc_coeff_step = 8.0f;
        BL_1r32_t eps = 1.0e-4f;
        pBL_optc_t optc = BL_optc_new(2, 0);
        {
            BL_optc_element_t e = { OBJECTIVE_sq_zcomponent, nullptr, { 1.0f, 1.0f }};
            ASSERT_EQ(ESUCCESS, BL_optc_set(optc, 0, &e));
        }
        {
            BL_optc_element_t e = { EQC_onplane, (const void*)&plane, { 1.0f, eqc_coeff_step } };
            ASSERT_EQ(ESUCCESS, BL_OPTC_SET_EQC(optc, 0, &e));
        }
        {
            BL_optc_element_t e = { EQC_oncylinder, (const void*)&cyl, { 1.0f, eqc_coeff_step }};
            ASSERT_EQ(ESUCCESS, BL_OPTC_SET_EQC(optc, 1, &e));
        }
        BL_3r32_t xv0 = { 0.0f, 0.0f, 0.0f };
        BL_1r32_t dx = 1.0f;
        int err;
        for (uint32_t i = 0; i < 16; i++)
        {
            pBL_nelderMD_t dfo = BL_nelderMD_new(3, xv0, dx, eps, BL_optc_eval, (const void*)optc);
            dfo->pf = nullptr;
            err = BL_nelderMD_run(dfo, 500);
            BL_varfuncpair_t bestxf;
            BL_nelderMD_bestxf(dfo, &bestxf);
            BL_copy3(xv0, bestxf.xv);
            dx /= eqc_coeff_step;
            fprintf(stdout, "i=%u, f=%f, xv:", i, bestxf.f);
            fn->writef(xv0, 3, 1, stdout);
            BL_optc_update(optc);
            BL_SAFEFREE(&dfo);
            if (ESUCCESS == err) break;
        }
        ASSERT_EQ(ESUCCESS, err);
        BL_optc_delete(&optc);
    }
}