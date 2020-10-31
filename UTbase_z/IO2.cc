#include    <gtest/gtest.h>
#include    <base_l/BL_base.h>
#include    <base_l/BL_futils.h>
#include    <base_z/BL_snparrayIO2.h>
#include    <ccomplex>
#define SUIT    IO2
#define SUIT_RESULT_DIR RESULT_DIR "IO2" STR_DIRSEP
#define TEST0_SAVE_PATH SUIT_RESULT_DIR "test0.csv"

namespace 
{
    TEST(SUIT, test0)
    {
        static const uint32_t points = 10;
        static const uint32_t ports = 4;
        pBL_snparray_t a = BL_snparray_new(points, ports);
        BL_snparray_setZc(a, 50.0 + 0.0I);
        double f0 = 100.e6;
        BL_1c128_t mat[ports * ports];
        for (uint32_t i = 0; i != points; i++)
        {
            ASSERT_EQ(ESUCCESS, BL_snparray_set_freq(a, f0 * (double)i, i));
            for (uint32_t row = 0; row != ports; row++)
            {
                for (uint32_t col = 0; col != ports; col++)
                {
                    uint32_t mat_element_index = col + row * ports;
                    *(std::complex<double>*)(mat+mat_element_index) = std::complex<double>((double)row + 0.1 * i , (double)col);
                }
            }
            ASSERT_EQ(ESUCCESS, BL_snparray_set_mat(a, mat, i));
        }
        FILE* pf = nullptr;
        ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, TEST0_SAVE_PATH, "w"));
        ASSERT_EQ(ESUCCESS, BL_snparrayIO2_writeRI(pf, a));
        fclose(pf);
        printf("file save path = %s¥n", TEST0_SAVE_PATH);
    }
}