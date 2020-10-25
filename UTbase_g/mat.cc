#include    <gtest/gtest.h>
#include    <base_g/BL_matexport.h>
#include    <base_l/BL_futils.h>
#include    <stdlib.h>
#define     SUIT    mat
#define     TEST_MAT_ROWS   3
#define     TEST_MAT_COLS   4
namespace  
{
    static const float tolr32 = 1.0e-5f;
    static const double tolr64 = 1.0e-10;

    bool Equalr32(BL_1r32_t r0, BL_1r32_t r1, BL_1r32_t tol)
    {
        BL_1r32_t abssum = fabsf(r0 + r1);
        if (abssum < tolr32)    abssum+=tolr32;
        BL_1r32_t absdiff = fabsf(r0 - r1);
        return (absdiff/abssum) < tol;
    }

    // bool Equalr64(BL_1r64_t r0, BL_1r64_t r1, BL_1r64_t tol)
    // {
    //     BL_1r64_t abssum = fabs(r0 + r1);
    //     BL_1r64_t absdiff = fabs(r0 - r1);
    //     return (absdiff/abssum) < tol;
    // }

    bool EqualMatr32(BL_1r32_t* m0, BL_1r32_t* m1, uint32_t nr, uint32_t nc, BL_1r32_t tol)
    {
        bool are_eq = true;
        for (uint32_t i = 0; i != (nr * nc); i++)
        {
            are_eq &= Equalr32(*m0, *m1, tol);
            m0++;
            m1++;
        }
        return are_eq;
    }
    static pcBL_matfnr32RM_t matfnr32RM = BL_matfnr32RM();
    static pcBL_matfnr64RM_t matfnr64RM = BL_matfnr64RM();
    static pcBL_matfnr32CM_t matfnr32CM = BL_matfnr32CM();
    static pcBL_matfnr64CM_t matfnr64CM = BL_matfnr64CM();

    TEST(SUIT, unit_r32)
    {
        uint32_t    n = TEST_MAT_COLS;
        float m0[TEST_MAT_COLS * TEST_MAT_COLS]; // declare a float array for a square matrix
        const float* m0_ = matfnr32RM->unit(1.0f, m0, n);
        ASSERT_TRUE(Equalr32(1.0f, m0_[0], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, m0_[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, m0_[5], tolr32));
    }

    TEST(SUIT, add_sub_r32)
    {
        BL_2u32_t matsize = { TEST_MAT_COLS, TEST_MAT_ROWS };
        BL_1r32_t m0[TEST_MAT_ROWS * TEST_MAT_COLS] =
        {
            1.0f, 2.0f, 3.0f, 4.0f,
            1.5f, 2.5f, 2.5f, 3.5f,
            -1.0f, -2.0f, -2.5f, -3.5f
        };
        BL_1r32_t m1[TEST_MAT_ROWS * TEST_MAT_COLS] =
        {
            4.0f, 3.0f, 2.0f, 1.0f,
            3.5f, 2.5f, 2.5f, 1.5f,
            -3.5f, -2.5f, -2.0f, -1.0f
        };
        BL_1r32_t m2[TEST_MAT_ROWS * TEST_MAT_COLS];
        const BL_1r32_t* mresult = matfnr32RM->add(m0, m1, m2, matsize[0], matsize[1]);
        ASSERT_TRUE(Equalr32(5.0f, mresult[0], tolr32));
        ASSERT_TRUE(Equalr32(-4.5f, mresult[IDXRM(3,2,matsize[0])], tolr32));
        mresult = matfnr32RM->sub(m0, m1, m2, matsize[0], matsize[1]);
        ASSERT_TRUE(Equalr32(-3.0f,mresult[0], tolr32));
        ASSERT_TRUE(Equalr32(-2.5f,mresult[IDXRM(3,2,matsize[0])], tolr32));
    }

#undef  TEST_MAT_ROWS
#undef  TEST_MAT_COLS
#define M0_COLS     2
#define M0_ROWS     3
#define M1_COLS     M0_ROWS
#define M1_ROWS     M0_COLS

    TEST(SUIT,mul_r32RM)
    {
        const BL_1r32_t m0[M0_COLS * M0_ROWS] = {
            1.0f, 2.0f,
            2.0f, 3.0f,
            3.0f, 2.0f
        };
        const BL_1r32_t m1[M1_COLS * M1_ROWS] = {
            1.0f, 2.0f, 3.0f,
            3.0f, 2.0f, 1.0f
        };
        BL_1r32_t m2[M1_COLS * M0_ROWS];
        BL_1r32_t m3[M0_COLS * M1_ROWS];
        const BL_1r32_t* m2_ = matfnr32RM->mul(m0,m1,m2,M1_COLS, M0_COLS , M0_ROWS);
        ASSERT_TRUE(Equalr32(7.0f,m2_[IDXRM(0,0,M1_COLS)],tolr32));
        ASSERT_TRUE(Equalr32(6.0f,m2_[IDXRM(1,0,M1_COLS)],tolr32));
        const BL_1r32_t* m3_ = matfnr32RM->mul(m1,m0,m3,M0_COLS, M0_ROWS, M1_ROWS);
        ASSERT_TRUE(Equalr32(14.0f, m3_[IDXRM(0,0,M0_COLS)],tolr32));
        ASSERT_TRUE(Equalr32(14.0f, m3_[IDXRM(1,0,M0_COLS)],tolr32));
    }

    TEST(SUIT,mul_r32CM)
    {
        // 3 rows, 2 columns
        const BL_1r32_t m0[M0_COLS * M0_ROWS] = {
            1.0f, 2.0f, 3.0f,
            2.0f, 3.0f, 2.0f
        };
        // 2 rows, 3 columns
        const BL_1r32_t m1[M1_COLS * M1_ROWS] = {
            1.0f, 3.0f,
            2.0f, 2.0f,
            3.0f, 1.0f
        };
        BL_1r32_t m2[M1_COLS * M0_ROWS];
        BL_1r32_t m3[M0_COLS * M1_ROWS];
        const BL_1r32_t* m2_ = matfnr32CM->mul(m0,m1,m2,M1_COLS, M0_COLS , M0_ROWS);
        ASSERT_TRUE(Equalr32(7.0f,m2_[IDXCM(0,0,M1_COLS)],tolr32));
        ASSERT_TRUE(Equalr32(6.0f,m2_[IDXCM(1,0,M1_COLS)],tolr32));
        const BL_1r32_t* m3_ = matfnr32CM->mul(m1,m0,m3,M0_COLS, M0_ROWS, M1_ROWS);
        ASSERT_TRUE(Equalr32(14.0f, m3_[IDXCM(0,0,M0_COLS)],tolr32));
        ASSERT_TRUE(Equalr32(14.0f, m3_[IDXCM(1,0,M0_COLS)],tolr32));
    }

#undef  M0_COLS
#undef  M0_ROWS
#undef  M1_COLS
#undef  M1_ROWS
#define M0_COLS 4
#define M0_ROWS 4
#define M1_COLS 2
#define M1_ROWS 3
#define M2_COLS 2
#define M2_ROWS M0_ROWS

    TEST(SUIT,submat_r32RM)
    {
        BL_1r32_t m0[M0_COLS * M0_ROWS];
        BL_1r32_t m1[M1_COLS * M1_ROWS];
        BL_1r32_t m2[M2_COLS * M2_ROWS] = {
            1.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            1.0f, 0.0f
        };
        for (uint32_t i = 0; i < ARRAYSIZE(m0); i++)
        {
            m0[i] = (BL_1r32_t)i;
        }
        // test getsubmat()
        const BL_1r32_t* m1_ = matfnr32RM->getsubmat(m0, m1, M0_COLS, M0_ROWS, M1_COLS, M1_ROWS, 0,0);
        for (uint32_t irow = 0; irow < M1_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M1_COLS; icol++)
            {
                BL_1r32_t refnum = (BL_1r32_t)(icol + irow * M0_COLS + 1);
                ASSERT_TRUE(Equalr32(refnum, m1_[IDXRM(icol,irow,M1_COLS)] + 1.0f, tolr32));
            }
        }
        m1_ = matfnr32RM->getsubmat(m0, m1, M0_COLS,M0_ROWS,M1_COLS,M1_ROWS,2,1);
        for (uint32_t irow = 0; irow < M1_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M1_COLS; icol++)
            {
                BL_1r32_t refnum = (BL_1r32_t)(2 + icol + (1 + irow) * M0_COLS);
                ASSERT_TRUE(Equalr32(refnum, m1_[IDXRM(icol,irow,M1_COLS)], tolr32));
            }
        }
        // test setsubmat()
        const BL_1r32_t* m0_ = matfnr32RM->setsubmat(m0, m2, M0_COLS,M0_ROWS,M2_COLS,M2_ROWS,2,0);
        for (uint32_t irow = 0; irow < M0_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M0_COLS; icol++)
            {
                if (icol < M0_COLS/2)
                {
                    BL_1r32_t refnum = (BL_1r32_t)(icol + irow * M0_COLS + 1);
                    ASSERT_TRUE(Equalr32(refnum, m0_[IDXRM(icol,irow,M0_COLS)] + 1.0f, tolr32));
                }
                else
                {
                    uint32_t icolm2 = icol - M0_COLS/2;
                    BL_1r32_t refnum = m2[IDXRM(icolm2,irow,M2_COLS)] + 1.0f;
                    ASSERT_TRUE(Equalr32(refnum, m0_[IDXRM(icol,irow,M0_COLS)] + 1.0f, tolr32));
                }
            }
        }
    }

    TEST(SUIT,submat_r32CM)
    {
        BL_1r32_t m0[M0_COLS * M0_ROWS];
        BL_1r32_t m1[M1_COLS * M1_ROWS];
        BL_1r32_t m2[M2_COLS * M2_ROWS] = {
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 0.0f
        };
        uint32_t i = 0;
        for (uint32_t irow = 0; irow != M0_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol != M0_COLS; icol++)
            {
                m0[IDXCM(icol, irow, M0_ROWS)] = (BL_1r32_t)i++;
            }
        }
        // test getsubmat()
        const BL_1r32_t* m1_ = matfnr32CM->getsubmat(m0, m1, M0_COLS, M0_ROWS, M1_COLS, M1_ROWS, 0,0);
        for (uint32_t irow = 0; irow < M1_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M1_COLS; icol++)
            {
                BL_1r32_t refnum = (BL_1r32_t)(icol + irow * M0_COLS + 1);
                ASSERT_TRUE(Equalr32(refnum, m1_[IDXCM(icol,irow,M1_ROWS)] + 1.0f, tolr32));
            }
        }
        m1_ = matfnr32CM->getsubmat(m0, m1, M0_COLS,M0_ROWS,M1_COLS,M1_ROWS,2,1);
        for (uint32_t irow = 0; irow < M1_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M1_COLS; icol++)
            {
                BL_1r32_t refnum = (BL_1r32_t)(2 + icol + (1 + irow) * M0_COLS);
                ASSERT_TRUE(Equalr32(refnum, m1_[IDXCM(icol,irow,M1_ROWS)], tolr32));
            }
        }
        // test setsubmat()
        const BL_1r32_t* m0_ = matfnr32CM->setsubmat(m0, m2, M0_COLS,M0_ROWS,M2_COLS,M2_ROWS,2,0);
        for (uint32_t irow = 0; irow < M0_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol < M0_COLS; icol++)
            {
                if (icol < M0_COLS/2)
                {
                    BL_1r32_t refnum = (BL_1r32_t)(icol + irow * M0_COLS + 1);
                    ASSERT_TRUE(Equalr32(refnum, m0_[IDXCM(icol,irow,M0_ROWS)] + 1.0f, tolr32));
                }
                else
                {
                    uint32_t icolm2 = icol - M0_COLS/2;
                    BL_1r32_t refnum = m2[IDXCM(icolm2,irow,M2_ROWS)] + 1.0f;
                    ASSERT_TRUE(Equalr32(refnum, m0_[IDXCM(icol,irow,M0_ROWS)] + 1.0f, tolr32));
                }
            }
        }
    }

#undef M0_COLS
#undef M0_ROWS
#undef M1_COLS
#undef M1_ROWS
#undef M2_COLS
#undef M2_ROWS
#define M0_COLS 3

    TEST(SUIT,inv_r32RM)
    {
        const BL_1r32_t m0[M0_COLS*M0_COLS] = {
            1.0f, 1.5f, 2.5f,
            2.5f, 1.0f, 1.5f,
            1.5f, 2.5f, 1.0f
        };
        BL_1r32_t m1[ARRAYSIZE(m0)];
        BL_1r32_t mwork[2*ARRAYSIZE(m0)];
        BL_1r32_t m0m1[ARRAYSIZE(m0)];
        const BL_1r32_t* m1_ = matfnr32RM->inv(m0, m1, mwork, M0_COLS);
        const BL_1r32_t* m0m1_ = matfnr32RM->mul(m0, m1_, m0m1, M0_COLS, M0_COLS, M0_COLS);
        for (uint32_t irow = 0; irow != M0_COLS; irow++)
        {
            for (uint32_t icol = 0; icol != M0_COLS; icol++)
            {
                BL_1r32_t refnum = (icol == irow) ? 2.0f : 1.0f;
                ASSERT_TRUE(Equalr32(refnum, m0m1_[IDXRM(icol, irow, M0_COLS)] + 1.0f, tolr32));
            }
        }
    }

    TEST(SUIT,inv_r32CM)
    {
        const BL_1r32_t m0[M0_COLS*M0_COLS] = {
            1.0f, 1.5f, 2.5f,
            2.5f, 1.0f, 1.5f,
            1.5f, 2.5f, 1.0f
        };
        BL_1r32_t m1[ARRAYSIZE(m0)];
        BL_1r32_t mwork[2*ARRAYSIZE(m0)];
        BL_1r32_t m0m1[ARRAYSIZE(m0)];
        const BL_1r32_t* m1_ = matfnr32CM->inv(m0, m1, mwork, M0_COLS);
        const BL_1r32_t* m0m1_ = matfnr32CM->mul(m0, m1_, m0m1, M0_COLS, M0_COLS, M0_COLS);
        for (uint32_t irow = 0; irow != M0_COLS; irow++)
        {
            for (uint32_t icol = 0; icol != M0_COLS; icol++)
            {
                BL_1r32_t refnum = (icol == irow) ? 2.0f : 1.0f;
                ASSERT_TRUE(Equalr32(refnum, m0m1_[IDXCM(icol, irow, M0_COLS)] + 1.0f, tolr32));
            }
        }
    }

#undef  M0_COLS
#define M0_COLS 3
#define M0_ROWS 2
    TEST(SUIT, transpose_r32RM)
    {
        const BL_1r32_t m0[M0_COLS * M0_ROWS] = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f
        };
        BL_1r32_t m1[ARRAYSIZE(m0)];
        const BL_1r32_t* m1_ = matfnr32RM->transpose(m0, m1, M0_ROWS, M0_COLS);
        for (uint32_t irow = 0; irow != M0_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol != M0_COLS; icol++)
            {
                ASSERT_TRUE(Equalr32(m0[IDXRM(icol, irow, M0_COLS)], m1_[IDXRM(irow, icol, M0_ROWS)], tolr32));
            }
        }
    }
    TEST(SUIT, transpose_r32CM)
    {
        const BL_1r32_t m0[M0_COLS * M0_ROWS] = {
            1.0f, 4.0f,
            2.0f, 5.0f,
            3.0f, 6.0f
        };
        BL_1r32_t m1[ARRAYSIZE(m0)];
        const BL_1r32_t* m1_ = matfnr32CM->transpose(m0, m1, M0_ROWS, M0_COLS);
        for (uint32_t irow = 0; irow != M0_ROWS; irow++)
        {
            for (uint32_t icol = 0; icol != M0_COLS; icol++)
            {
                ASSERT_TRUE(Equalr32(m0[IDXCM(icol, irow, M0_ROWS)], m1_[IDXCM(irow, icol, M0_COLS)], tolr32));
            }
        }
    }

    TEST(SUIT, rotmat_r32RM)
    {
        const BL_4r32_t v0x = { 1.0f, 0.0f, 0.0f, 1.0f };
        const BL_4r32_t v0y = { 0.0f, 1.0f, 0.0f, 1.0f };
        const BL_4r32_t v0z = { 0.0f, 0.0f, 1.0f, 1.0f };

        BL_1r32_t m[16];
        const BL_1r32_t* matrotz = matfnr32RM->rotzdeg(90.0f, m);
        BL_4r32_t v;
        const BL_1r32_t* v1 = matfnr32RM->mul(matrotz, v0x, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32RM->mul(matrotz, v0y, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(-1.0f, v1[0], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        const BL_1r32_t* matrotx = matfnr32RM->rotxdeg(90.0f, m);
        v1 = matfnr32RM->mul(matrotx, v0y, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32RM->mul(matrotx, v0z, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(-1.0f, v1[1], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        const BL_1r32_t* matroty = matfnr32RM->rotydeg(90.0f, m);
        v1 = matfnr32RM->mul(matroty, v0z, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32RM->mul(matroty, v0x, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(-1.0f, v1[2], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
    }

    TEST(SUIT, rotmat_r32CM)
    {
        const BL_4r32_t v0x = { 1.0f, 0.0f, 0.0f, 1.0f };
        const BL_4r32_t v0y = { 0.0f, 1.0f, 0.0f, 1.0f };
        const BL_4r32_t v0z = { 0.0f, 0.0f, 1.0f, 1.0f };
        BL_1r32_t m[16];
        const BL_1r32_t* matrotz = matfnr32CM->rotzdeg(90.0f, m);
        BL_4r32_t v;
        const BL_1r32_t* v1 = matfnr32CM->mul(matrotz, v0x, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32CM->mul(matrotz, v0y, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(-1.0f, v1[0], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        const BL_1r32_t* matrotx = matfnr32CM->rotxdeg(90.0f, m);
        v1 = matfnr32CM->mul(matrotx, v0y, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32CM->mul(matrotx, v0z, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(-1.0f, v1[1], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        const BL_1r32_t* matroty = matfnr32CM->rotydeg(90.0f, m);
        v1 = matfnr32CM->mul(matroty, v0z, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[2] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
        v1 = matfnr32CM->mul(matroty, v0x, v, 1, 4, 4);
        ASSERT_TRUE(Equalr32(1.0f, v1[0] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[1] + 1.0f, tolr32));
        ASSERT_TRUE(Equalr32(-1.0f, v1[2], tolr32));
        ASSERT_TRUE(Equalr32(1.0f, v1[3], tolr32));
    }

    void setLHSRM(size_t N, BL_1r32_t* LHS)
    {
        for (size_t irow = 0; irow != N; irow++)
        {
            for (size_t icol=0; icol != N; icol++)
            {
                LHS[icol + irow*N] = (float)rand()/(float)RAND_MAX;
            }
        }
    }

    void setRHSRM(size_t N, size_t NBC, BL_1r32_t* RHS)
    {
        for (size_t i = 0; i != (N*NBC); i++)
        {
            *RHS++ = (float)rand() / (float)RAND_MAX;
        }
    }

    TEST(SUIT, soldense_r32RM)
    {
        const size_t N = 5; // number of linear equations
        const size_t NBC = 3; // number of boundary conditions
        BL_1r32_t LHS[N*N];
        BL_1r32_t RHS[N*NBC];
        BL_1r32_t x[N*NBC];
        BL_1r32_t m[ARRAYSIZE(LHS)+ARRAYSIZE(RHS)];
        setLHSRM(N, LHS);
        setRHSRM(N, NBC, RHS);
        matfnr32RM->setsubmat(m, LHS, N+NBC, N, N, N, 0, 0);
        matfnr32RM->setsubmat(m, RHS, N+NBC, N, NBC, N, N, 0);
        Tenum err = matfnr32RM->soldense(m, N+NBC, N, x);
        ASSERT_EQ(ESUCCESS, (int)err);
        BL_1r32_t RHS_restored[ARRAYSIZE(RHS)];
        matfnr32RM->mul(LHS, x, RHS_restored, NBC, N, N);
        ASSERT_TRUE(EqualMatr32(RHS, RHS_restored, N, NBC, tolr32));
    }


    void setLHSCM(size_t N, BL_1r32_t* LHS)
    {
        for (size_t irow = 0; irow != N; irow++)
        {
            for (size_t icol=0; icol != N; icol++)
            {
                LHS[irow + icol*N] = (float)rand()/(float)RAND_MAX;
            }
        }
    }

    void setRHSCM(size_t N, size_t NBC, BL_1r32_t* RHS)
    {
        for (size_t i = 0; i != (N*NBC); i++)
        {
            *RHS++ = (float)rand() / (float)RAND_MAX;
        }
    }
    TEST(SUIT, soldense_r32CM)
    {
        const size_t N = 5; // number of linear equations
        const size_t NBC = 3; // number of boundary conditions
        BL_1r32_t LHS[N*N];
        BL_1r32_t RHS[N*NBC];
        BL_1r32_t x[N*NBC];
        BL_1r32_t m[ARRAYSIZE(LHS)+ARRAYSIZE(RHS)];
        setLHSCM(N, LHS);
        setRHSCM(N, NBC, RHS);
        matfnr32CM->setsubmat(m, LHS, N+NBC, N, N, N, 0, 0);
        matfnr32CM->setsubmat(m, RHS, N+NBC, N, NBC, N, N, 0);
        Tenum err = matfnr32CM->soldense(m, N+NBC, N, x);
        ASSERT_EQ(ESUCCESS, (int)err);
        BL_1r32_t RHS_restored[ARRAYSIZE(RHS)];
        matfnr32CM->mul(LHS, x, RHS_restored, NBC, N, N);
        ASSERT_TRUE(EqualMatr32(RHS, RHS_restored, N, NBC, tolr32));
    }
}