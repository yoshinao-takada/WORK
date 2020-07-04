#include    <gtest/gtest.h>
#include    "base_o/BL_linesearch.h"
#include    "base_o/BL_steepestdescent.h"
#include    "base_o/BL_cg.h"

#define SUIT    linesearch
#define TRACE1  UTDATA "Linesearch/trace1.csv"

namespace 
{
    TEST(SUIT, linesearch_xm)
    {
        BL_3r32_t  x = { 0.0f, 1.0f, 2.0f };
        BL_3r32_t f = { 1.0f, 0.0f, 0.0f };
        BL_1r32_t xm = BL_linesearch_xm(x, f);
        printf("xm = %f\n", xm);
    }

    typedef struct {
        BL_3r32_t c; // center
        BL_3r32_t span;
    } ellipsoidal_distance_t, *pellipsoidal_distance_t;
    typedef const ellipsoidal_distance_t *pcellipsoidal_distance_t;

    // target problem for minimization
    int SqEllipsoidalDistance(uint32_t vardim, const BL_1r32_t *x, const void* params, BL_1r32_t* f)
    {
        int err = ESUCCESS;
        pcellipsoidal_distance_t param_obj = (pcellipsoidal_distance_t)params;
        do {
            BL_1r32_t sqsum = 0.0f;
            for (uint32_t i = 0; i != 3; i++)
            {
                BL_1r32_t offset = x[i] - param_obj->c[i];
                offset /= 0.5f * param_obj->span[i];
                sqsum += offset * offset;
            }
            *f = sqsum;
        } while (false);
        return err;
    }

    TEST(SUIT, linesearch)
    {
        const ellipsoidal_distance_t param_obj = { { 1.1f, -2.0f, -0.5f }, { 2.0f, 1.5f, 0.7f }};
        const void* params = (const void*)&param_obj;
        const BL_3r32_t xv0 = { 0.0f, 0.0f, 0.0f };
        // const BL_3r32_t vdir = { 1.0f, 0.0f, 0.0f };
        const BL_3r32_t vdir = { 0.471f, 0.856f, -0.214f };
        BL_1r32_t hmin = 1.0e-4, hmax = 1.0f, eps = 1.0e-6;
        pBL_linesearch_t p = BL_linesearch_new(SqEllipsoidalDistance, params, 3, xv0, vdir, hmin, hmax, eps);
        int err = BL_linesearch_run(p, 100);
        BL_SAFEFREE(&p);
    }

    TEST(SUIT, steepestdescent)
    {
        const ellipsoidal_distance_t param_obj = { { 1.1f, -2.0f, -0.5f }, { 2.0f, 1.5f, 0.7f }};
        const void* params = (const void*)&param_obj;
        const BL_3r32_t xv0 = { 0.0f, 0.0f, 0.0f };
        BL_1r32_t hmin = 1.0e-4f;
        pBL_steepestdescent_t p = BL_steepestdescent_new(3, xv0, SqEllipsoidalDistance, params, hmin);
        p->trace = nullptr;
        int err = BL_steepestdescent_run(p, 100);
        ASSERT_EQ(ESUCCESS, err);
        printf("xyz final = (%f,%f,%f)\n", p->xv[0], p->xv[1], p->xv[2]);
        BL_SAFEFREE(&p);
    }

    TEST(SUIT, cg)
    {
        const ellipsoidal_distance_t param_obj = { { 1.1f, -2.0f, -0.5f }, { 2.0f, 1.5f, 0.7f }};
        const void* params = (const void*)&param_obj;
        const BL_3r32_t xv0 = { 0.0f, 0.0f, 0.0f };
        BL_1r32_t hmin = 1.0e-4f;
        pBL_cg_t p = BL_cg_new(3, hmin, xv0, SqEllipsoidalDistance, params);
        p->trace = nullptr;
        int err = BL_cg_run(p, 100);
        ASSERT_EQ(ESUCCESS, err);
        printf("xyz final = (%f,%f,%f), f final = %f\n", p->xv[0], p->xv[1], p->xv[2], p->f);
        BL_SAFEFREE(&p);
    }
}