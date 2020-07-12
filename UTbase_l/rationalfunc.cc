#include    <gtest/gtest.h>
#include    <base_l/BL_rationalfunc.h>
#include    <base_l/BL_errno.h>
#include    <base_l/BL_futils.h>
#include    <ccomplex>

#define     SUIT    rationalfunc
#define     RATIONAL_DATA_DIR       DATA_DIR    "rational/"
#define     RATIONAL1DR_CSV         RATIONAL_DATA_DIR   "RATIONAL1DR.csv"
#define     RATIONAL1DC_CSV         RATIONAL_DATA_DIR   "RATIONAL1DC.csv"

namespace
{
    int ProvisionTest()
    {
        int err = ESUCCESS;
        do {
            if (!BL_futils_exists(RATIONAL_DATA_DIR))
            {
                if (ESUCCESS != (err = BL_futils_create_directories(RATIONAL_DATA_DIR)))
                {
                    break;
                }
            }
        } while (false);
        return err;
    }

    // {(x-1)(x+1)(x-2)}/{(x-1)(x+1)} = (x - 2)
    // x = [-2.0,2.0], 0.1 step
    // save as a csv (RATIONAL1DC_CSV)
    TEST(SUIT, rationalr)
    {
        ASSERT_EQ(ESUCCESS, ProvisionTest());
        pBL_linkable_array_t pnum = BL_linkable_array_new(4, BL_1r32_t);
        { // setup numerator
            BL_ptr_t i_coeff = BL_linkable_array_begin(pnum);
            *i_coeff._1r32++ = 2.0f; // x^0 coeff
            *i_coeff._1r32++ = -1.0f; // x^1 coeff
            *i_coeff._1r32++ = -2.0f; // x^2 coeff
            *i_coeff._1r32++ = 1.0f; // x^3 coeff
        }
        pBL_linkable_array_t pden = BL_linkable_array_new(3, BL_1r32_t);
        { // setup denominator
            BL_ptr_t i_coeff = BL_linkable_array_begin(pden);
            *i_coeff._1r32++ = -1.0f; // x^0 coeff
            *i_coeff._1r32++ = 0.0f; // x^1 coeff
            *i_coeff._1r32++ = 1.0f; // x^2 coeff            
        }
        BL_linkable_link_next(&pnum->link, &pden->link);
        const uint32_t array_size = 41;
        BL_1r32_t *x = (BL_1r32_t*)calloc(2*array_size, sizeof(BL_1r32_t));
        BL_1r32_t *y = x + array_size;
        BL_1r32_t x0 = -1.95f, dx = 0.1f;
        for (uint32_t i = 0; i < array_size; i++)
        {
            x[i] = x0 + (BL_1r32_t)i * dx;
        }
        ASSERT_EQ(ESUCCESS, BL_rationalfunc_rvaluev(pnum, array_size, x, y));
        BL_linkable_delete_all((pBL_linkable_t)pnum);
        { // save x, y as csv
            FILE* pf = nullptr;
            ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, RATIONAL1DR_CSV, "w"));
            fprintf(pf, "x,y\n");
            for (uint32_t i = 0; i != array_size; i++)
            {
                fprintf(pf, "%f,%f\n", x[i], y[i]);
            }
            fclose(pf);
        }
        free(x);
    }

    TEST(SUIT, rationalc)
    {
        ASSERT_EQ(ESUCCESS, ProvisionTest());
        pBL_linkable_array_t pnum = BL_linkable_array_new(4, BL_1c64_t);
        { // setup numerator
            BL_ptr_t i_coeff = BL_linkable_array_begin(pnum);
            *i_coeff._1c64++ = 2.0f + _Complex_I*0.0;
            *i_coeff._1c64++ = -1.0f + _Complex_I*0.0f;
            *i_coeff._1c64++ = -2.0f + _Complex_I*0.0f;
            *i_coeff._1c64++ = 1.0f + _Complex_I*0.0f;
        }
        pBL_linkable_array_t pden = BL_linkable_array_new(3, BL_1c64_t);
        { // setup denominator
            BL_ptr_t i_coeff = BL_linkable_array_begin(pden);
            *i_coeff._1c64++ = -1.0f + _Complex_I*0.0f;
            *i_coeff._1c64++ = 0.0f + _Complex_I*0.0f;
            *i_coeff._1c64++ = 1.0f + _Complex_I*0.0f;
        }
        BL_linkable_link_next(&pnum->link, &pden->link);
        const uint32_t array_size = 41;
        BL_1c64_t *x = (BL_1c64_t*)calloc(2*array_size*array_size, sizeof(BL_1c64_t));
        BL_1c64_t *y = x + array_size*array_size;
        BL_1r32_t x0_re = -1.95f, dx_re = 0.1f, x0_im = -1.95f, dx_im = 0.1f;
        for (uint32_t i_im = 0; i_im != array_size; i_im++)
        {
            for (uint32_t i_re = 0; i_re != array_size; i_re++)
            {
                x[i_re + i_im * array_size] = 
                    x0_re + dx_re * (BL_1r32_t)i_re + _Complex_I* (x0_im + dx_im * (BL_1r32_t)i_im);
            }
        }
        ASSERT_EQ(ESUCCESS, BL_rationalfunc_cvaluev(pnum, array_size*array_size, x, y));
        BL_linkable_delete_all((pBL_linkable_t)pnum);
        { // save x, y as csv
            FILE* pf = nullptr;
            ASSERT_EQ(ESUCCESS, BL_futils_open(&pf, RATIONAL1DC_CSV, "w"));
            fprintf(pf, "x.re,x.im,y.re,y.im\n");
            for (uint32_t i = 0; i != array_size*array_size; i++)
            {
                fprintf(pf, "%f,%f,%f,%f\n", crealf(x[i]), cimagf(x[i]), crealf(y[i]), cimagf(y[i]));
            }
            fclose(pf);
        }
        free(x);
    }

    TEST(SUIT, rationalMDr)
    {
        ASSERT_EQ(ESUCCESS, ProvisionTest());
    }
}