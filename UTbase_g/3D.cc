#include    <gtest/gtest.h>
#include    <base_g/BL_3Dline.h>
#include    <base_g/BL_3Dplane.h>
#include    <base_g/BL_3Dcylinder.h>
#include    <base_g/BL_matexport.h>
#define SUIT    Object3D

namespace
{
    static const pcBL_matfnr32RM_t fn = BL_matfnr32RM();
    static const BL_1r32_t tol = 1.0e-5f;

    bool Equal(float f0, float f1, float tol)
    {
        float abssum = std::abs<float>(f0 + f1);
        if (abssum < tol)
        {
            f0 += tol;
            f1 += tol;
        }
        abssum = std::abs<float>(f0 + f1);
        float absdif = std::abs<float>(f0 - f1);
        return (absdif/abssum) < tol;
    }

    bool Equal(const BL_3r32_t v0, const BL_3r32_t v1, float tol)
    {
        bool b = true;
        for (uint32_t i = 0; i != 3; i++)
        {
            b &= Equal(v0[i], v1[i], tol);
            if (!b) break;
        }
        return b;
    }

    TEST(SUIT, line_normalfoot)
    {
        const BL_3r32_t foot_ref = { 0.5f, 0.5f, 0.0f };
        const float distance_ref = sqrtf(0.5f);
        BL_3Dline_t line = {
            { -1.0f, -1.0f, 0.0f }, // direction
            { 1.0f, 1.0f, 0.0f } // reference point
        };
        BL_3Dline_set_direction(&line, line.dir);
        BL_3r32_t foot, p = { 1.0f, 0.0f, 0.0f };
        BL_3Dline_normalfoot(&line, p, foot);
        BL_1r32_t distance;
        BL_3Dline_distance(&line, p, &distance);
        ASSERT_TRUE(Equal(foot_ref, foot, tol));
        ASSERT_TRUE(Equal(distance_ref, distance, tol));
    }

    TEST(SUIT, plane_normalfoot)
    {
        const BL_3r32_t foot_ref = { 0.5f, 0.5f, 0.0f };
        const float distance_ref = sqrtf(0.5f);
        BL_3Dplane_t plane = {
            { distance_ref, distance_ref, 0.0f }, // normal
            { 1.0f, 0.0f, 0.0f } // ref point
        };
        BL_3r32_t foot, p = { 0.0f, 0.0f, 0.0f };
        BL_3Dplane_normalfoot(&plane, p, foot);
        BL_1r32_t offset;
        BL_3Dplane_offset(&plane, p, &offset);
        ASSERT_TRUE(Equal(foot_ref, foot, tol));
        ASSERT_TRUE(Equal(-distance_ref, offset, tol));
        p[0] = p[1] = 1.0f;
        BL_3Dplane_offset(&plane, p, &offset);
        ASSERT_TRUE(Equal(distance_ref, offset, tol));
    }

    #define CYL_DIR_UNNORMALIZED    { 1.0f, 1.0f, 0.0f }
    #define ORIGIN3D                { 0.0f, 0.0f, 0.0f }
    TEST(SUIT, cylinder_normalfoot)
    {
        BL_3Dcylinder_t cyl = {
            { CYL_DIR_UNNORMALIZED, ORIGIN3D }, 1.0f
        };
        fn->normalize(cyl.ax.dir, cyl.ax.dir, 3);
        BL_3r32_t foot, p = { 1.0f, 0.0f, 0.0f };
        BL_1r32_t offset;
        BL_3Dcylinder_normalfoot_on_axis(&cyl, p, foot);
        const BL_3r32_t foot_on_axis_ref = { 0.5f, 0.5f, 0.0f };
        ASSERT_TRUE(Equal(foot_on_axis_ref, foot, tol));
        BL_3Dcylinder_offset_from_surface(&cyl, p, &offset);
        ASSERT_TRUE(Equal(-(1.0f - sqrtf(0.5f)), offset, tol));
    }
}