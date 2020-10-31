#include    <gtest/gtest.h>
#include    <base_z/BL_styz.h>
#include    <base_z/BL_2port.h>
#include    <ccomplex>
#include    <base_g/BL_matexport.h>
#define SUIT    styz

pcBL_matfnc128RM_t matfnc = BL_matfnc128RM();

namespace
{
    #define TOL     1.0e-10

    bool EqualDouble(const double& d0, const double& d1)
    {
        double d0_ = d0;
        double d1_ = d1;
        if (fabs(d0_ + d1_) < TOL)
        {
            d0_ += TOL;
            d1_ += TOL;
        }
        double absdiff = fabs(d0_ - d1_);
        double abssum = fabs(d0_ + d1_);
        return absdiff / abssum < TOL;
    }

    bool EqualComplexDouble(const std::complex<double>& c0, const std::complex<double>& c1)
    {
        return EqualDouble(c0.real(), c1.real()) && EqualDouble(c0.imag(), c1.imag());
    }

    bool EqualMat2x2(const BL_1c128_t* mat0, const BL_1c128_t* mat1)
    {
        const std::complex<double>* mat0_ = (const std::complex<double>*)mat0;
        const std::complex<double>* mat1_ = (const std::complex<double>*)mat1;
        bool bEqual = true;
        for (uint32_t i = 0; i != 3; i++)
        {
            bEqual = bEqual && EqualComplexDouble(mat0_[i], mat1_[i]);
        }
        return bEqual;
    }

    TEST(SUIT, yssy)
    {
        double w = 2.0 * M_PI * 1e9;
        BL_1c128_t y[4];
        const BL_1c128_t *s, *y2;
        pBL_styz_t calc = BL_styz_new1(2, 50.0);
        ASSERT_NE(nullptr, calc);
        BL_2port_LC_section_t section = {
            BL_linear_element_lossy_LC_parallel,
            BL_2port_topology_series_2port,
            1e-9, 1e-12, 50.0,
            1.0e-6, 1.0e-6
        };
        ASSERT_EQ(ESUCCESS, BL_2port_LC_section_y(&section, w, y));
        fprintf(stdout, "y\n");
        matfnc->writef(y, 2, 2, stdout);
        s = BL_styz_ys(calc, y, 0);
        ASSERT_NE(nullptr, s);
        fprintf(stdout, "s\n");
        matfnc->writef(s, 2, 2, stdout);
        y2 = BL_styz_sy(calc, s, 1);
        ASSERT_NE(nullptr, y2);
        fprintf(stdout, "y2\n");
        matfnc->writef(y2, 2, 2, stdout);
        ASSERT_TRUE(EqualMat2x2(y, y2));
    }

    TEST(SUIT, zssz)
    {
        double w = 2.0 * M_PI * 1e9;
        BL_1c128_t z[4];
        const BL_1c128_t *s, *z2;
        pBL_styz_t calc = BL_styz_new1(2, 50.0);
        BL_2port_LC_section_t section = {
            BL_linear_element_lossy_LC_series,
            BL_2port_topology_shunt_2port,
            1e-9, 1e-12, 50.0,
            1.0e-6, 1.0e-6
        };
        ASSERT_EQ(ESUCCESS, BL_2port_LC_section_z(&section, w, z));
        fprintf(stdout, "z\n");
        matfnc->writef(z, 2, 2, stdout);
        s = BL_styz_zs(calc, z, 0);
        ASSERT_NE(nullptr, s);
        fprintf(stdout, "s\n");
        matfnc->writef(s, 2, 2, stdout);
        z2 = BL_styz_sz(calc, s, 1);
        ASSERT_NE(nullptr, z2);
        fprintf(stdout, "z2\n");
        matfnc->writef(z2, 2, 2, stdout);
        ASSERT_TRUE(EqualMat2x2(z, z2));
    }

    TEST(SUIT, stts)
    {
        double w = 2.0 * M_PI * 1e9;
        BL_1c128_t z[4];
        const BL_1c128_t *s, *t, *s2;
        pBL_styz_t calc = BL_styz_new1(2, 50.0);
        BL_2port_LC_section_t section = {
            BL_linear_element_lossy_LC_series,
            BL_2port_topology_shunt_2port,
            1e-9, 1e-12, 50.0,
            1.0e-6, 1.0e-6
        };
        BL_2port_LC_section_z(&section, w, z);
        s = BL_styz_zs(calc, z, 0);
        t = BL_styz_st(calc, s, 1);
        s2 = BL_styz_ts(calc, t, 2);
        ASSERT_TRUE(EqualMat2x2(s, s2));
    }
}