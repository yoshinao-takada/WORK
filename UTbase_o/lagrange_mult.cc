#include <gtest/gtest.h>
#include    "base_o/BL_cg.h"
#include    "base_o/BL_constraint.h"
#include    "base_o/BL_testappeqc.h"
#define SUIT    AugmentedLagrangeMuliplierMethod
/*
problem description
Objective function
*/
namespace
{
    static const BL_1r32_t tolF = 1.0e-6;
    static bool EqualF(BL_1r32_t f0, BL_1r32_t f1, BL_1r32_t tol)
    {
        BL_1r32_t sum = f0 + f1;
        if (fabsf(sum) < tol)
        {
            f0 += 1.0f;
            f1 += 1.0f;
        }
        BL_1r32_t abssum = fabsf(f0 + f1);
        BL_1r32_t absdiff = fabsf(f0 - f1);
        return absdiff / abssum < tol;
    }

    static const TEST_plane_t plane = {
        { 0.0f, 0.0f, 2.0f}, { 1.0f/sqrtf(2.0f), 0.0, 1.0f/sqrtf(2.0f)}
    };

    static const TEST_cylinder_t cylinder {
        { 0.0f, 1.0f, 2.0f}, { 1.0f/sqrtf(2.0f), 0.0, 1.0f/sqrtf(2.0f)},
        1.0f
    };


    /*!

    */
    static void get_basis(const BL_3r32_t basis0, BL_3r32_t basis1, BL_3r32_t basis2)
    {
        pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
        const BL_3r32_t vn[] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
        const BL_3r32_t basis0abs = { fabsf(basis0[0]), fabsf(basis0[1]), fabsf(basis0[2]) };
        const BL_1r32_t min_basis0abs = __min(basis0abs[0], __min(basis0abs[1], basis0abs[2]));
        uint32_t least_similar_axis = 0;
        for (uint32_t i = 0; i != 3; i++)
        {
            if (min_basis0abs == basis0abs[i])
            {
                least_similar_axis = i;
                break;
            }
        }
        BL_3r32_t basis0_x_least_similar_axis; // cross product of (basis0 x least_similar_axis)
        fnmat->crossproduct(basis0, vn[least_similar_axis], basis0_x_least_similar_axis);
        fnmat->normalize(basis0_x_least_similar_axis, basis1, 3);
        BL_3r32_t basis0_x_basis1;
        fnmat->crossproduct(basis0, basis1, basis0_x_basis1);
        fnmat->normalize(basis0_x_basis1, basis2, 3);
    }

    // basis() validation
    TEST(SUIT, validate_basis)
    {
        pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
        BL_3r32_t basis1, basis2;
        get_basis(plane.vn, basis1, basis2);
        BL_1r32_t basis0_norm, basis1_norm, basis2_norm;
        fnmat->mul(plane.vn, plane.vn, &basis0_norm, 1, 3, 1);
        fnmat->mul(basis1, basis1, &basis1_norm, 1, 3, 1);
        fnmat->mul(basis2, basis2, &basis2_norm, 1, 3, 1);
        ASSERT_TRUE(EqualF(basis0_norm, 1.0f, tolF));
        ASSERT_TRUE(EqualF(basis1_norm, 1.0f, tolF));
        ASSERT_TRUE(EqualF(basis2_norm, 1.0f, tolF));
        BL_1r32_t basis0_dot_basis1, basis1_dot_basis2, basis2_dot_basis0;
        fnmat->mul(plane.vn, basis1, &basis0_dot_basis1, 1, 3, 1);
        fnmat->mul(basis1, basis2, &basis1_dot_basis2, 1, 3, 1);
        fnmat->mul(basis2, plane.vn, &basis2_dot_basis0, 1, 3, 1);
        ASSERT_TRUE(EqualF(basis0_dot_basis1, 0.0f, tolF));
        ASSERT_TRUE(EqualF(basis1_dot_basis2, 0.0f, tolF));
        ASSERT_TRUE(EqualF(basis2_dot_basis0, 0.0f, tolF));
    }

    // TEST_objective() validation
    TEST(SUIT, objective0)
    {
        pBL_cg_t cg = nullptr;
        ASSERT_EQ(ESUCCESS, TEST_create(&plane, &cylinder, &cg));
        const BL_3r32_t vn = { 0.0f, 0.0f, 1.0f };
        const BL_3r32_t v0 = { 1.0f, 2.0f, 3.5f }, v1 = { 2.0f, 3.0f, -4.5f };
        BL_1r32_t f;
        ASSERT_EQ(ESUCCESS, TEST_objective(3, v0, (const void*)vn, &f));
        ASSERT_TRUE(EqualF(v0[2], f, tolF));
        ASSERT_EQ(ESUCCESS, TEST_objective(3, v1, (const void*)vn, &f));
        ASSERT_TRUE(EqualF(v1[2], f, tolF));
    }

    int IsInPlane(const BL_3r32_t point, pcTEST_plane_t plane)
    {
        pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
        BL_3r32_t offset_from_plane_ref;
        fnmat->sub(point, plane->p0, offset_from_plane_ref, 3, 1);
        BL_1r32_t dot_prod, offset_norm;
        fnmat->mul(offset_from_plane_ref, plane->vn, &dot_prod, 1, 3, 1);
        fnmat->mul(offset_from_plane_ref, offset_from_plane_ref, &offset_norm, 1, 3, 1);
        BL_1r32_t relative_measure = dot_prod / sqrtf(offset_norm);
        printf("%s,dot_prod=%f, offset_norm=%f, sqrt(offset_norm)=%f\n", __FUNCTION__, dot_prod, offset_norm, sqrtf(offset_norm));
        return fabs(relative_measure) < tolF ? ESUCCESS : ERANGE;
    }

    // validate constraint function on a plane.
    // step 1: get an in-plane-point world coordinate linear sum of in-plane basis 2D coordinate
    // step 2: offset the in-plane-point in the direction of the plane normal
    // step 3: calculate constraint function value which shall be the offset distance in step 2.
    TEST(SUIT, eqc_plane)
    {
        pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
        pBL_cg_t cg = nullptr;
        ASSERT_EQ(ESUCCESS, TEST_create(&plane, &cylinder, &cg)); // in order to initialize in-module function table
        BL_2r32_t in_plane_points[] = {
            { 1.5f, 2.2f }, { 2.5f, -5.0f }, { -1.0f, 0.0f }
        };
        BL_3r32_t basis[3];
        BL_copy3(basis[0], plane.vn);
        get_basis(basis[0], basis[1], basis[2]);
        for (uint32_t i = 0; i != ARRAYSIZE(in_plane_points); i++)
        {
            BL_1r32_t in_plane_coord1 = in_plane_points[i][0];
            BL_1r32_t in_plane_coord2 = in_plane_points[i][1];
            BL_3r32_t in_plane_world_coord;
            BL_copy3(in_plane_world_coord, plane.p0);
            for (uint32_t j = 0; j != 3; j++)
            {
                in_plane_world_coord[j] += in_plane_coord1 * basis[1][j];
                in_plane_world_coord[j] += in_plane_coord2 * basis[2][j];
            }
            ASSERT_EQ(ESUCCESS, IsInPlane(in_plane_world_coord, &plane));
            BL_1r32_t eqc;
            ASSERT_EQ(ESUCCESS, TEST_eqc0(3, in_plane_world_coord, &plane, &eqc));
            printf("%s,eqc = %f\n", __FUNCTION__, eqc);
            ASSERT_TRUE(EqualF(eqc, 0.0f, tolF));
            BL_3r32_t offset_world_coord;
            fnmat->add(in_plane_world_coord, plane.vn, offset_world_coord, 3, 1);
            ASSERT_EQ(ESUCCESS, TEST_eqc0(3, offset_world_coord, &plane, &eqc));
            printf("%s,eqc(+offset) = %f\n", __FUNCTION__, eqc);
            fnmat->sub(in_plane_world_coord, plane.vn, offset_world_coord, 3, 1);
            ASSERT_EQ(ESUCCESS, TEST_eqc0(3, offset_world_coord, &plane, &eqc));
            printf("%s,eqc(-offset) = %f\n", __FUNCTION__, eqc);
        }
    }

    // validate constraint function on a cylinder surface
    // step 1: get the 2nd and 3rd basis refering the cylinder axis as the 1st one.
    // step 2: get a point on the cylinder surface
    // step 3: calc constraint function value
    // step 4: offset the subject point outside a little
    // step 5: calc constraint function value
    // step 6: offset the subject point inside a little
    // step 7: calc constraint function value
    TEST(SUIT, eqc_cyl)
    {
        pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
        pBL_cg_t cg = nullptr;
        ASSERT_EQ(ESUCCESS, TEST_create(&plane, &cylinder, &cg)); // in order to initialize in-module function table

        BL_3r32_t basis[3];
        BL_copy3(basis[0], cylinder.axis_dir);
        get_basis(basis[0], basis[1], basis[2]);
        const BL_1r32_t a = 1.5f; // axis direction offset from the axis reference point
        BL_3r32_t point_on_axis = { 
            cylinder.axis0[0] + cylinder.axis_dir[0] * a, 
            cylinder.axis0[1] + cylinder.axis_dir[1] * a, 
            cylinder.axis0[2] + cylinder.axis_dir[2] * a
        };
        const BL_1r32_t a0 = 0.5f, a1 = 0.8f;
        BL_3r32_t normal_on_axis = {
            a0 * basis[1][0] + a1 * basis[2][0],
            a0 * basis[1][1] + a1 * basis[2][1],
            a0 * basis[1][2] + a1 * basis[2][2]
        };
        BL_3r32_t unit_normal_on_axis;
        fnmat->normalize(normal_on_axis, unit_normal_on_axis, 3);
        BL_1r32_t orth_dot;
        fnmat->mul(unit_normal_on_axis, cylinder.axis_dir, &orth_dot, 1, 3, 1);
        ASSERT_TRUE(EqualF(0.0f, orth_dot, tolF));
        BL_3r32_t radius_normal_on_axis;
        fnmat->muls(unit_normal_on_axis, 3, 1, cylinder.R, radius_normal_on_axis);
        BL_3r32_t point_on_surface ;
        fnmat->add(point_on_axis, radius_normal_on_axis, point_on_surface, 3, 1);
        BL_1r32_t eqc;
        ASSERT_EQ(ESUCCESS, TEST_eqc1(3, point_on_surface, (const void*)&cylinder, &eqc));
        printf("%s,eqc=%f\n", __FUNCTION__, eqc);
        ASSERT_TRUE(EqualF(0.0f, eqc, tolF));
        BL_3r32_t point_outside, point_inside;
        fnmat->add(point_on_surface, unit_normal_on_axis, point_outside, 3, 1);
        fnmat->sub(point_on_surface, unit_normal_on_axis, point_inside, 3, 1);
        ASSERT_EQ(ESUCCESS, TEST_eqc1(3, point_outside, (const void*)&cylinder, &eqc));
        printf("%s,eqc=%f\n", __FUNCTION__, eqc);
        ASSERT_EQ(ESUCCESS, TEST_eqc1(3, point_inside, (const void*)&cylinder, &eqc));
        printf("%s,eqc=%f\n", __FUNCTION__, eqc);
    }

    // Solve constraint optmization problem using CG solver
    TEST(SUIT, optcg0)
    {
        pBL_cg_t cg = nullptr;
        ASSERT_EQ(ESUCCESS, TEST_create_penalty(&plane, &cylinder, &cg));
        pcBL_constraint_t constraints = (pcBL_constraint_t)cg->params;
        ASSERT_EQ(3, constraints->vardim_objective);
        ASSERT_EQ(5, constraints->vardim);
        ASSERT_EQ(2, constraints->eqc_dim);
        ASSERT_EQ(0, constraints->neqc_dim);
        cg->trace = nullptr;
        ASSERT_EQ(ESUCCESS, BL_cg_run(cg, 100));
        printf("xyz = %f,%f,%f, f = %f\n", cg->xv[0], cg->xv[1], cg->xv[2], cg->f);
        TEST_destroy(&cg);
    }
}