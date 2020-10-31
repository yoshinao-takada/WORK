#include    <gtest/gtest.h>
#include    <base_z/BL_snparrayIO2.h>
#include    <base_z/BL_2port.h>
#include    <base_z/BL_styz.h>
#include    <base_l/BL_base.h>
#include    <base_l/BL_futils.h>
#include    <cmath>
#include    <ccomplex>
#include    <complex.h>
#include    <base_g/BL_matexport.h>
#define     SUIT    multisection
#define     SUIT_RESULT_DIR RESULT_DIR "multisection" STR_DIRSEP
#define     TEST0_RESULT_CSV    SUIT_RESULT_DIR "bwlpff5.csv"
static pcBL_matfnc128RM_t matfnc = BL_matfnc128RM(); // matrix function table

namespace
{
    // create 5 stage Butterworth lowpass filter with fc=500 MHz
    TEST(SUIT, test0)
    {
        #pragma region create_LC_sections
        double fc = 500e6;
        double wc = 2.0 * M_PI * fc;
        std::complex<double> z0(50.0, 0.0);
        double prototype_elements[] = { 1.0, 1.618, 2.0, 1.618, 1.0 }; // Butterworth filter lowpass prototype
        BL_2port_LC_section_t sections[ARRAYSIZE(prototype_elements)];
        for (uint32_t i = 0; i != ARRAYSIZE(prototype_elements); i++)
        {
            sections[i].Gs = sections[i].Rs = 1.0e-6;
            sections[i].R = 0.0;
            if (0 == (i & 1)) 
            { // create shunt section
                sections[i].element_type = BL_linear_element_lossy_LC_series;
                sections[i].topology = BL_2port_topology_shunt_2port;
                sections[i].C = prototype_elements[i]/(wc * z0.real());
                sections[i].L = 2e-9; // parasitic series inductance
            }
            else
            { // seriese section
                sections[i].element_type = BL_linear_element_lossy_LC_parallel;
                sections[i].topology = BL_2port_topology_series_2port;
                sections[i].L = prototype_elements[i]*z0.real()/wc;
                sections[i].C = 0.5e-12; // parasitic parallel capacitance
            }
        }
        #pragma endregion create_LC_sections
        #pragma region init_calculator_and_data_array
        pBL_styz_t calc = BL_styz_new1(2, z0.real());
        uint32_t point = 201;
        pBL_snparray_t a = BL_snparray_new(point, 2);
        double fmax = 4.0 * fc;
        double delta_f = fmax / (double)(point - 1);
        BL_snparray_setZc(a, *(BL_1c128_t*)&z0);
        BL_1c128_t sec_mat[ARRAYSIZE(sections)][4];
        for (uint32_t i = 0; i != point; i++)
        {
            double w = (double)(2 * i) * delta_f * M_PI;
            ASSERT_EQ(ESUCCESS, BL_snparray_set_angularfreq(a, w, i));
            for (int j = 0; j != ARRAYSIZE(sections); j++)
            {
                const BL_1c128_t* s = nullptr;
                if (0 == (j & 1))
                { // shunt section
                    ASSERT_EQ(ESUCCESS, BL_2port_LC_section_z(&sections[j], w, sec_mat[j])); // calc z
                    s = BL_styz_zs(calc, sec_mat[j], 0); // convert z to s
                }
                else
                { // series section
                    ASSERT_EQ(ESUCCESS, BL_2port_LC_section_y(&sections[j], w, sec_mat[j])); // calc y
                    s = BL_styz_ys(calc, sec_mat[j], 0); // convert y to s
                }
                const BL_1c128_t* t = BL_styz_st(calc, s, 1); // convert s to t
                BL_copy4(sec_mat[j], t);
            } // sec_mat[*] was filled with T-parameter of the corresponding section.
            // calc->work[0] = sec_mat[0] * sec_mat[1] * ... * sec_mat[4]
            matfnc->mul(sec_mat[0], sec_mat[1], calc->work[1], 2, 2, 2);
            matfnc->mul(calc->work[1], sec_mat[2], calc->work[0], 2, 2, 2);
            matfnc->mul(calc->work[0], sec_mat[3], calc->work[1], 2, 2, 2);
            matfnc->mul(calc->work[1], sec_mat[4], calc->work[0], 2, 2, 2);
            const BL_1c128_t* s = BL_styz_ts(calc, calc->work[0], 1);
            ASSERT_EQ(ESUCCESS, BL_snparray_set_mat(a, s, i));
        }
        #pragma endregion init_calculator_and_data_array
        FILE* pf = nullptr;
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, TEST0_RESULT_CSV, "w"));
        ASSERT_EQ(ESUCCESS, BL_snparrayIO2_writeMA(pf, a));
        fclose(pf);
        fprintf(stdout, "S-parameter plot was saved in %s\n", TEST0_RESULT_CSV);
    }
}