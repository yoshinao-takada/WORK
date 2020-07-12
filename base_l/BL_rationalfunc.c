#include    "base_l/BL_rationalfunc.h"
#include    <math.h>

int BL_polynomial_rvalue(pcBL_array_t coeff, BL_1r32_t x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    *y = 0.0f;
    do {
        if (!coeff || !coeff->unit_count)
        {
            err = EINVAL;
            break;
        }
        BL_cptr_t i_coeff = BL_array_cbegin(coeff);
        uint32_t count = coeff->unit_count;
        i_coeff._1r32 += count;
        do {
            i_coeff._1r32--;
            *y = (x * (*y) + *i_coeff._1r32);
        } while (--count);
    } while (0);
    return err;
}

int BL_polynomial_cvalue(pcBL_array_t coeff, BL_1c64_t x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    *y = CMPLX(0.0f, 0.0f);
    do {
        if (!coeff || !coeff->unit_count)
        {
            err = EINVAL;
            break;
        }
        BL_cptr_t i_coeff = BL_array_cbegin(coeff);
        uint32_t count = coeff->unit_count;
        i_coeff._1c64 += count;
        do {
            i_coeff._1c64--;
            *y = (x * (*y) + *i_coeff._1c64);
        } while (--count);
    } while (0);
    return err;
}

int BL_rationalfunc_rvalue(pcBL_linkable_array_t coeff, BL_1r32_t x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    do {
        BL_1r32_t denominator, numerator;
        if (ESUCCESS != (err = BL_polynomial_rvalue(&coeff->data, x, &numerator)))
        {
            break;
        }
        coeff = (pcBL_linkable_array_t)coeff->link.next;
        if (ESUCCESS != (err = BL_polynomial_rvalue(&coeff->data, x, &denominator)))
        {
            break;
        }
        if (!isnormal(denominator))
        {
            err = EINVAL;
            break;
        }
        *y = numerator/denominator;
    } while (0);
    return err;
}

int BL_rationalfunc_cvalue(pcBL_linkable_array_t coeff, BL_1c64_t x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    do {
        BL_1c64_t denominator, numerator;
        if (ESUCCESS != (err = BL_polynomial_cvalue(&coeff->data, x, &numerator)))
        {
            break;
        }
        coeff = (pcBL_linkable_array_t)coeff->link.next;
        if (ESUCCESS != (err = BL_polynomial_cvalue(&coeff->data, x, &denominator)))
        {
            break;
        }
        BL_1r32_t re_denom = crealf(denominator);
        BL_1r32_t im_denom = cimagf(denominator);
        if (!isnormal(re_denom*re_denom + im_denom * im_denom))
        {
            err = EINVAL;
            break;
        }
        *y = numerator/denominator;
    } while (0);
    return err;
}

int BL_rationalfunc_rvalueMD(pcBL_linkable_array_t coeff, BL_1r32_t x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    do {
        if (!coeff)
        { // coeff is null.
            err = EINVAL;
            break;
        }
        uint32_t n = BL_linkable_count((pcBL_linkable_t)coeff);
        if (0 == (n & 1))
        { // coeff has odd number of nodes.
            err = EINVAL;
            break;
        }
        n = (n + 1) / 2;
        do {
            if (ESUCCESS != (err = BL_rationalfunc_rvalue(coeff, x, y)))
            {
                break;
            }
            coeff = (pcBL_linkable_array_t)(coeff->link.next->next);
        } while (--n);
    } while (0);
    return err;
}

int BL_rationalfunc_cvalueMD(pcBL_linkable_array_t coeff, BL_1c64_t x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    do {
        if (!coeff)
        { // coeff is null.
            err = EINVAL;
            break;
        }
        uint32_t n = BL_linkable_count((pcBL_linkable_t)coeff);
        if (0 == (n & 1))
        { // coeff has odd number of nodes.
            err = EINVAL;
            break;
        }
        n = (n + 1) / 2;
        do {
            if (ESUCCESS != (err = BL_rationalfunc_cvalue(coeff, x, y)))
            {
                break;
            }
            coeff = (pcBL_linkable_array_t)(coeff->link.next->next);
        } while (--n);
    } while (0);
    return err;
}

int BL_polynomial_rvaluev(pcBL_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_polynomial_rvalue(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}

int BL_polynomial_cvaluev(pcBL_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_polynomial_cvalue(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}

int BL_rationalfunc_rvaluev(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_rationalfunc_rvalue(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}

int BL_rationalfunc_cvaluev(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_rationalfunc_cvalue(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}

int BL_rationalfunc_rvalueMDv(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_rationalfunc_rvalueMD(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}

int BL_rationalfunc_cvalueMDv(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y)
{
    int err = ESUCCESS;
    do {
        for (uint32_t i = 0; i != xdim; i++)
        {
            if (ESUCCESS != (err = BL_rationalfunc_cvalueMD(coeff, *x++, y++)))
            {
                break;
            }
        }
    } while (0);
    return err;
}
