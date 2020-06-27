#include    <gtest/gtest.h>
#include    <base_g/BL_intersect.h>
#include    <base_g/BL_matexport.h>
#include    <base_g/BL_plate.h>
#define SUIT    BL_intersect

namespace
{
    static pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    static const BL_1r32_t tol = 1.0e-5f;

    TEST(SUIT, planeline)
    {
        BL_intersect_planeline_t conf =
        {
            { 0.0f, 0.0f, 0.0f }, // p0_line
            { 0.0f, 0.0f, 1.0f }, // d_line
            { 1.0f, -1.0f, 3.5f }, // p1_plane
            { 0.0f, 0.0f, 1.0f } // n_plane
        };
        BL_3r32_t p;
        BL_1r32_t tol = 1.0e-5f;
        const BL_3r32_t pref = { 0.0f, 0.0f, 3.5f };
        int err = BL_intersect_planeline(&conf, p);
        ASSERT_TRUE(fn->equalv(pref, p, 3, tol));
    }

    float rngf()
    {
        return 2.0f * ((float)rand() / (float)RAND_MAX - 0.5f);
    }

    TEST(SUIT, planeline2)
    {
        srand(1);
        BL_intersect_planeline_t conf =
        {
            { rngf(), rngf(), rngf() }, // p0_line
            { rngf(), rngf(), rngf() }, // d_line
            { rngf(), rngf(), rngf() }, // p1_plane
            { rngf(), rngf(), rngf() } // n_plane
        };
        BL_3r32_t p;
        int err = BL_intersect_planeline(&conf, p);
        // confirm p is on the plane
        // p1_p = p1 - p
        BL_3r32_t p1_p = { conf.p1_plane[0] - p[0], conf.p1_plane[1] - p[1], conf.p1_plane[2] - p[2] };
        // <p1_p, conf.n_plane> = 0
        BL_1r32_t p1_p_n;
        fn->mul(p1_p, conf.n_plane, &p1_p_n, 1, 3, 1);
        ASSERT_TRUE(fabsf(p1_p_n) < tol);

        // confirm p is on the line
        // p0_p = p0 - p
        BL_3r32_t p0_p = { conf.p0_line[0] - p[0], conf.p0_line[1] - p[1], conf.p0_line[2] - p[2] };
        BL_1r32_t p0_p_d;
        fn->mul(p0_p, conf.d_line, &p0_p_d, 1, 3, 1);
        float p0_p_length = sqrtf(p0_p[0]*p0_p[0] + p0_p[1]*p0_p[1] + p0_p[2]*p0_p[2]);
        float d_length = sqrtf(conf.d_line[0]*conf.d_line[0]+conf.d_line[1]*conf.d_line[1] + conf.d_line[2]*conf.d_line[2]);
        float abs_cos_theta = fabsf(p0_p_d)/(p0_p_length * d_length);
        ASSERT_TRUE(fabsf(abs_cos_theta - 1.0f) < tol);
    }

    TEST(SUIT, planeline3)
    {
        srand(1);
        BL_line_t line = {
            { rngf(), rngf(), rngf() }, { rngf(), rngf(), rngf() }
        };
        BL_plane_t plane = {
            { rngf(), rngf(), rngf() }, { rngf(), rngf(), rngf() }
        };
        BL_3r32_t p;
        int err = BL_line_plane_intersect(&line, &plane, p);
        // confirm p is on the plane
        // p0_p = p1 - p
        {
            BL_3r32_t p0_p = { plane.p0[0] - p[0], plane.p0[1] - p[1], plane.p0[2] - p[2]};
            // <p0_p, plane.n> = 0 (<a, b> is inner product of a and b)
            BL_1r32_t p0_p_n;
            fn->mul(p0_p, plane.n, &p0_p_n, 1, 3, 1);
            ASSERT_TRUE(fabsf(p0_p_n) < tol);
        }
        // confirm p is on the line
        // p0_p = p0 - p
        {
            BL_3r32_t p0_p = { line.p0[0] - p[0], line.p0[1] - p[1], line.p0[2] - p[2] };
            // p0_p_d = <p0_p, line.d>
            BL_1r32_t p0_p_d;
            fn->mul(p0_p, line.d, &p0_p_d, 1, 3, 1);
            float p0_p_length = sqrtf(p0_p[0]*p0_p[0] + p0_p[1]*p0_p[1] + p0_p[2]*p0_p[2]);
            float d_length = sqrtf(line.d[0]*line.d[0]+line.d[1]*line.d[1] + line.d[2]*line.d[2]);
            float abs_cos_theta = fabsf(p0_p_d)/(p0_p_length * d_length);
            ASSERT_TRUE(fabsf(abs_cos_theta - 1.0f) < tol);
        }
    }
}