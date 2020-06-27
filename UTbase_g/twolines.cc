#include    <gtest/gtest.h>
#include    <base_g/BL_twolines.h>
#include    <base_g/BL_matexport.h>
#define SUIT    TwoLines

namespace
{
    const BL_1r32_t tol = 1.0e-5f;
    pcBL_matfnr32RM_t fn = BL_matfnr32RM();

    bool Equal(const BL_3r32_t p0, const BL_3r32_t p1, BL_1r32_t tol)
    {
        BL_3r32_t diff, sum;
        BL_1r32_t diff_sqlen, sum_sqlen;
        fn->sub(p0, p1, diff, 1, 3);
        fn->add(p0, p1, sum, 1, 3);
        fn->mul(diff, diff, &diff_sqlen, 1, 3, 1);
        fn->mul(sum,sum, &sum_sqlen, 1, 3, 1);
        return diff_sqlen/sum_sqlen < tol*tol;
    }

    TEST(SUIT, sample0)
    {
        const BL_twolines_t conf =
        {
            { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
            { 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }
        };
        const BL_3r32_t refPTa = { 1.0f, 1.0f, 0.0f };
        const BL_3r32_t refPTb = { 1.0f, 1.0f, 1.0f };
        BL_3r32_t PTa, PTb;
        int err = BL_twolines_nearest(&conf, PTa, PTb);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_TRUE(Equal(refPTa, PTa, tol));
        ASSERT_TRUE(Equal(refPTb, PTb, tol));
    }

    TEST(SUIT, sample1)
    {
        BL_1r32_t sqrt0p5 = sqrtf(0.5f);
        const BL_twolines_t conf = 
        {
            { 1.0f, 0.1f, 0.0f }, { -sqrt0p5, 0.0f, sqrt0p5 },
            { -1.0f, -0.2f, 0.0f }, { sqrt0p5, 0.0f, sqrt0p5 }
        };
        const BL_3r32_t refPTa = { 0.0f, 0.1f, 1.0f };
        const BL_3r32_t refPTb = { 0.0f, -0.2f, 1.0f };
        BL_3r32_t PTa, PTb;
        int err = BL_twolines_nearest(&conf, PTa, PTb);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_TRUE(Equal(refPTa, PTa, tol));
        ASSERT_TRUE(Equal(refPTb, PTb, tol));
    }
}