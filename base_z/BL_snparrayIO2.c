#include    "base_z/BL_snparrayIO2.h"
#include    <math.h>

int BL_snparrayIO2_writeRI(FILE* pf, pcBL_snparray_t a)
{
    int err = ESUCCESS;
    do {
        fprintf(pf, "freq");
        for (uint32_t row=0; row != a->ports; row++)
        {
            for (uint32_t col=0; col != a->ports; col++)
            {
                fprintf(pf, ",re(%u:%u),im(%u:%u)", row, col, row, col);
            }
        }
        fprintf(pf, "\n");
        const BL_1c128_t* p = a->elements;
        for (uint32_t i_point = 0; i_point != a->points; i_point++)
        {
            fprintf(pf, "%f", creal(*p++)/(2.0 * M_PI));
            for (uint32_t row = 0; row != a->ports; row++)
            {
                for (uint32_t col = 0; col != a->ports; col++)
                {
                    fprintf(pf, ",%f,%f", creal(*p), cimag(*p));
                    p++;
                }
            }
            fprintf(pf, "\n");
        }
    } while (0);
    return err;
}

int BL_snparrayIO2_writeMA(FILE* pf, pcBL_snparray_t a)
{
    int err = ESUCCESS;
    do {
        fprintf(pf, "freq");
        for (uint32_t row=0; row != a->ports; row++)
        {
            for (uint32_t col=0; col != a->ports; col++)
            {
                fprintf(pf, ",mag(%u:%u),ph-deg(%u:%u)", row, col, row, col);
            }
        }
        fprintf(pf, "\n");
        const BL_1c128_t* p = a->elements;
        for (uint32_t i_point = 0; i_point != a->points; i_point++)
        {
            fprintf(pf, "%f", creal(*p++)/(2.0 * M_PI));
            for (uint32_t row = 0; row != a->ports; row++)
            {
                for (uint32_t col = 0; col != a->ports; col++)
                {
                    fprintf(pf, ",%f,%f", cabs(*p), carg(*p)/(2.0*M_PI));
                    p++;
                }
            }
            fprintf(pf, "\n");
        }
    } while (0);
    return err;
}

static double mag_to_dB(double mag)
{
    return 20.0 * log10(mag);
}

int BL_snparrayIO2_writeDA(FILE* pf, pcBL_snparray_t a)
{
    int err = ESUCCESS;
    do {
        fprintf(pf, "freq");
        for (uint32_t row=0; row != a->ports; row++)
        {
            for (uint32_t col=0; col != a->ports; col++)
            {
                fprintf(pf, ",dB(%u:%u),ph-deg(%u:%u)", row, col, row, col);
            }
        }
        fprintf(pf, "\n");
        const BL_1c128_t* p = a->elements;
        for (uint32_t i_point = 0; i_point != a->points; i_point++)
        {
            fprintf(pf, "%f", creal(*p++)/(2.0 * M_PI));
            for (uint32_t row = 0; row != a->ports; row++)
            {
                for (uint32_t col = 0; col != a->ports; col++)
                {
                    fprintf(pf, ",%f,%f", mag_to_dB(cabs(*p)), carg(*p)/(2.0*M_PI));
                    p++;
                }
            }
            fprintf(pf, "\n");
        }
    } while (0);
    return err;
}