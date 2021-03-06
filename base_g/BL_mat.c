#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_g/catmacro.h"
#include    <stdio.h>
#if defined(MATLAYOUTRM)
#if defined(NUMTYPEr32)
#include    "base_g/localdefr32.h"
#include    "base_g/globaldefr32RM.h"
#elif defined(NUMTYPEr64)
#include    "base_g/localdefr64.h"
#include    "base_g/globaldefr64RM.h"
#elif defined(NUMTYPEc64)
#include    "base_g/localdefc64.h"
#include    "base_g/globaldefc64RM.h"
#elif defined(NUMTYPEc128)
#include    "base_g/localdefc128.h"
#include    "base_g/globaldefc128RM.h"
#endif /* NUMTYPEXXX */
#elif defined(MATLAYOUTCM)
#if defined(NUMTYPEr32)
#include    "base_g/localdefr32.h"
#include    "base_g/globaldefr32CM.h"
#elif defined(NUMTYPEr64)
#include    "base_g/localdefr64.h"
#include    "base_g/globaldefr64CM.h"
#elif defined(NUMTYPEc64)
#include    "base_g/localdefc64.h"
#include    "base_g/globaldefc64CM.h"
#elif defined(NUMTYPEc128)
#include    "base_g/localdefc128.h"
#include    "base_g/globaldefc128CM.h"
#endif /* NUMTYPEXXX */
#endif  /* MATLAYOUTXX */
#include    <assert.h>
#include    <errno.h>
#include    <stdlib.h>

// declare prototypes
#pragma region matlayout_independent
static CPnum fill(Tnum s0, Pnum m, Tsize nc, Tsize nr);
static CPnum unit(Tnum s0, Pnum m, Tsize n);
static CPnum add(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
static CPnum sub(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
static void swap(Pnum a, Pnum b);
static CPnum rotxdeg(Tnum s0, Pnum m);
static CPnum rotydeg(Tnum s0, Pnum m);
static CPnum rotzdeg(Tnum s0, Pnum m);
static Tenum writeb(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
static Tenum readb(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
static Tenum equalv(CPnum m0, CPnum m1, Tsize nc0, Tnum s0);
static CPnum muls(CPnum m0, Tsize nc0, Tsize nr0, Tnum s0, Pnum m);
static CPnum crossproduct(CPnum m0, CPnum m1, Pnum m);
static CPnum normalize(CPnum m0, Pnum m, Tsize ndim);
#pragma endregion
#pragma region matlayout_dependent
static CPnum transpose(CPnum m0, Pnum m, Tsize nc, Tsize nr);
static CPnum rotxrad(Tnum s0, Pnum m);
static CPnum rotyrad(Tnum s0, Pnum m);
static CPnum rotzrad(Tnum s0, Pnum m);
static CPnum translate(CPnum m0, Pnum m);
static CPnum mul(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr);
static CPnum setsub(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
static CPnum getsub(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
static CPnum inv(CPnum m0, Pnum m, Pnum mwork, Tsize n);
static Tenum soldense(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1);
static Tenum writef(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
static Tenum readf(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
#if defined(MATLAYOUTRM)
static Tenum calc_normalized_norm(CPnum ipv, Tsize nsum, Prnum normalized_norm);
static Tsize find_best_pivot_row(CPnum m0, Tsize nc, Tsize nr, Tsize icol);
#elif defined(MATLAYOUTCM)
static Tenum calc_normalized_norm(CPnum ipv, Tsize nsum, Tsize nr, Prnum normalized_norm);
static Tsize find_best_pivot_row(CPnum m0, Tsize nr, Tsize icol);
#endif
#pragma endregion

#pragma region matlayout_independent_impl
// all matrix elements are filled with a number, s0.
static CPnum fill(Tnum s0, Pnum m, Tsize nc, Tsize nr)
{
    Tsize n = nc * nr;
    Pnum im = m;
    do {
        *im++ = s0;
    } while (--n);
    return m;
}

// create a unit matrix. diagonal element is multiplied by s0.
static CPnum unit(Tnum s0, Pnum m, Tsize n)
{
    fill(_0, m, n, n);
    Tsize nstep = n + 1;
    Pnum im = m;
    do {
        *im = s0;
        im += nstep;
    } while (--n);
    return m;
}

// add two matrices
static CPnum add(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr)
{
    Tsize n = nc * nr;
    Pnum im = m;
    CPnum im0 = m0;
    CPnum im1 = m1;
    do {
        *im++ = *im0++ + *im1++;
    } while (--n);
    return m;
}

// subtract m1 from m0
static CPnum sub(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr)
{
    Tsize n = nc * nr;
    Pnum im = m;
    CPnum im0 = m0;
    CPnum im1 = m1;
    do {
        *im++ = *im0++ - *im1++;
    } while (--n);
    return m;
}

static CPnum muls(CPnum m0, Tsize nc0, Tsize nr0, Tnum s0, Pnum m)
{
    Tsize n = nc0 * nr0;
    Pnum im = m;
    CPnum im0 = m0;
    do {
        *im++ = *im0++ * s0;
    } while (--n);
    return m;
}

// swap two scalar variables, a and b.
static void swap(Pnum a, Pnum b)
{
    Tnum c = *a;
    *a = *b;
    *b = c;
}

// create a homogeneous coordinate rotation matrix around x-axis.
// rotation angle is given in degrees
static CPnum rotxdeg(Tnum s0, Pnum m)
{
    return rotxrad(MATH_DEG2RAD(s0),m);
}

// create a homogeneous coordinate rotation matrix around y-axis.
// rotation angle is given in degrees
static CPnum rotydeg(Tnum s0, Pnum m)
{
    return rotyrad(MATH_DEG2RAD(s0),m);
}

// create a homogeneous coordinate rotation matrix around z-axis.
// rotation angle is given in degrees
static CPnum rotzdeg(Tnum s0, Pnum m)
{
    return rotzrad(MATH_DEG2RAD(s0),m);
}

// write a matrix into a file in platform dependent, matrix layout dependent binary format.
static Tenum writeb(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf)
{
    Tenum err = ESUCCESS;
    do {
        if (fwrite((const void*)&nc0, sizeof(Tsize), 1, pf) != 1)
        {
            err = EFAULT;
            break;
        }
        if (fwrite((const void*)&nr0, sizeof(Tsize), 1, pf) != 1)
        {
            err = EFAULT;
            break;
        }
        size_t block_count = (size_t)(nc0 * nr0);
        if (fwrite((const void*)m0, sizeof(Tnum), block_count, pf) != block_count)
        {
            err = EFAULT;
            break;
        }
    } while (0);
    return err;
}

// read a matrix from a file in the binary format
static Tenum readb(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf)
{
    Tsize ntotal = 0, n[2] = {0,0};
    Tsize read_block_count = 0;
    Tenum err = ESUCCESS;
    do {
        if (2 != (read_block_count = fread((void*)n, sizeof(Tsize), 2, pf)))
        {
            err = EFAULT;
            break;
        }
        *nc0 = n[0];
        *nr0 = n[1];
        ntotal = n[0] * n[1];
        if (NULL == (*ppm0 = (Pnum)calloc(ntotal, sizeof(Tnum))))
        {
            err = ENOMEM;
            break;
        }
        if (ntotal != fread((void*)*ppm0, sizeof(Tnum), ntotal, pf))
        {
            err = EFAULT;
            break;
        }
    } while (0);
    return err;
}

// check vector equality
static Tenum equalv(CPnum m0, CPnum m1, Tsize nc0, Tnum s0)
{
    Trnum sumnormdiff = REAL_PART(_0), sumnormadd = REAL_PART(_0);
    for (Tsize i = 0; i != nc0; i++)
    {
        Tnum diff_i = m0[i] - m1[i];
        Tnum add_i = m0[i] + m1[i];
        sumnormdiff += L2NORM(diff_i);
        sumnormadd += L2NORM(add_i);
    }
    Trnum norm_s0 = L2NORM(s0);
    if (sumnormadd < norm_s0)
    {
        sumnormadd += norm_s0;
    }
    return (sumnormdiff / sumnormadd) < norm_s0 ? 1 : 0;
}

static CPnum crossproduct(CPnum m0, CPnum m1, Pnum m)
{
    m[0] = m0[1] * m1[2] - m0[2] * m1[1];
    m[1] = m0[2] * m1[0] - m0[0] * m1[2];
    m[2] = m0[0] * m1[1] - m0[1] * m1[0];
    return m;
}

static CPnum normalize(CPnum m0, Pnum m, Tsize ndim)
{
    Tnum L2norm;
    mul(m0, m0, &L2norm, 1, ndim, 1);
    Tnum normalize_coeff = _1 / MATH_SQRT(L2norm);
    return muls(m0, ndim, 1, normalize_coeff, m);
}
#pragma endregion
#if defined(MATLAYOUTRM)
#pragma region matlayoutRM_impl
// transpose a matrix
static CPnum transpose(CPnum m0, Pnum m, Tsize nc, Tsize nr)
{
    Pnum im = m;
    for (Tsize irow = 0; irow != nr; irow++)
    {
        for (Tsize icol = 0; icol != nc; icol++)
        {
            *im++ = m0[IDXRM(irow,icol,nr)];
        }
    }
    return m;
}

// create a homogeneous coordinate rotation matarix around x-axis. rotation angle is given in radian.
static CPnum rotxrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[5] = m[10] = MATH_COS(s0);
    m[6] = -(m[9] = MATH_SIN(s0));
    return m;
}

// create a homogeneous coordinate rotation matarix around y-axis. rotation angle is given in radian.
static CPnum rotyrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[0] = m[10] = MATH_COS(s0);
    m[8] = -(m[2] = MATH_SIN(s0));
    return m;
}

// create a homogeneous coordinate rotation matarix around z-axis. rotation angle is given in radian.
static CPnum rotzrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[0] = m[5] = MATH_COS(s0);
    m[1] = -(m[4] = MATH_SIN(s0));
    return m;
}

// param v [in] 3D vector of moving
// param m [in] work matrix holding 4x4 homogenous translate matrix
static CPnum translate(CPnum v, Pnum m)
{
    unit(_1, m, 4);
    m[3] = v[0];
    m[7] = v[1];
    m[11] = v[2];
    return m;
}

static CPnum mul(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr)
{
    Pnum im = m;
    for (Tsize irow = 0; irow != nr; irow++)
    {
        for (Tsize icol = 0; icol != nc; icol++)
        {
            *im = _0;
            CPnum im0 = m0 + irow * ni;
            CPnum im1 = m1 + icol;
            for (Tsize iinter = 0; iinter != ni; iinter++)
            {
                *im += *im0++ * *im1;
                im1 += nc;
            }
            im++;
        }
    }
    return m;
}

static CPnum setsub(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff)
{
    assert(ncoff + nc1 <= nc0);
    assert(nroff + nr1 <= nr0);
    CPnum im1 = m1;
    Pnum im00 = m0 + ncoff + nroff * nc0;
    for (Tsize irow = 0; irow != nr1; irow++)
    {
        Pnum im0 = im00;
        for (Tsize icol = 0; icol != nc1; icol++)
        {
            *im0++ = *im1++;
        }
        im00 += nc0;
    }
    return m0;
}

static CPnum getsub(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff)
{
    assert(ncoff + nc1 <= nc0);
    assert(nroff + nr1 <= nr0);
    Pnum im1 = m1;
    CPnum im00 = m0 + ncoff + nroff * nc0;
    for (Tsize irow = 0; irow != nr1; irow++)
    {
        CPnum im0 = im00;
        for (Tsize icol = 0; icol != nc1; icol++)
        {
            *im1++ = *im0++;
        }
        im00 += nc0;
    }
    return m1;
}

// calculate normalized pivot norm
static Tenum calc_normalized_norm(CPnum ipv, Tsize nsum, Prnum normalized_norm)
{
    Tenum err = EFAULT;
    do {
        Trnum pivot_norm = L2NORM(*ipv);
        Trnum partial_sum = pivot_norm;
        for (Tsize i = 1; i < nsum; i++)
        {
            ipv++;
            partial_sum += L2NORM(*ipv);
        }
        *normalized_norm = _0;
        if (partial_sum > (MIN_L2SUM))
        {
            err = ESUCCESS;
            *normalized_norm = RNUM_TO_NUM(pivot_norm / partial_sum);
        }
    } while (0);
    return err;
}

// find the best pivot row for the column icol
// param m0 [in] any matrix with nc >= nr
// param nc, nr [in] columns and rows of m0
// param icol [in] working column of pivot operation
// return selected best pivit row number. return nr if the matrix is nearly singular.
static Tsize find_best_pivot_row(CPnum m0, Tsize nc, Tsize nr, Tsize icol)
{
    Tsize irow_found = nr;
    Trnum normalized_norm = _0;
    do {
        CPnum ipv = m0 + icol + icol * nc;
        Tsize n_partial_sum_range = nr - icol;
        Trnum normalized_norm_temp = _0;
        for (Tsize irow = icol; irow != nr; irow++)
        {
            if (EFAULT == calc_normalized_norm(ipv, n_partial_sum_range, &normalized_norm_temp))
            { // singular matrix error
                irow_found = nr;
                break;
            }
            if (L2NORM(normalized_norm_temp) > L2NORM(normalized_norm))
            {
                normalized_norm = normalized_norm_temp;
                irow_found = irow;
            }
            ipv += nc;
        }
    } while (0);
    return irow_found;
}

// calculate inverse of m0.
// param m0 [in] any non-singular square matrix
// m [in] work area used as the return value (nxn square matrix)
// mwork [in] work area used for row operation (nx2n matrix; side-by-side concatination of two square matrix)
static CPnum inv(CPnum m0, Pnum m, Pnum mwork, Tsize n)
{
    Tsize _2n = 2 * n;
    setsub(mwork, m0, _2n, n, n, n, 0, 0); // copy m0 in the left half of mwork
    setsub(mwork, unit(_1, m, n), _2n, n, n, n, n, 0); // fill the right half of mwork with a unit matrix
    Pnum ipivot = mwork;
    Tsize step_pivot = _2n + 1;
    Tsize n_row_length = _2n;
    Tsize n_partial_sum_range = n;
    // Gaussian elimination
    for (int irow = 0; irow != (int)n; irow++, ipivot += step_pivot, n_row_length--, n_partial_sum_range--)
    {
        Tsize irow_best = find_best_pivot_row(mwork, _2n, n, irow);
        if (irow_best == n)
        {
            return (CPnum)NULL; // singular matrix error
        }
        if (irow_best != irow)
        { // swap rows if a better swap row is found.
            Pnum i0 = ipivot;
            Pnum i1 = i0 + _2n * (irow_best - irow);
            for (Tsize i = 0; i != n_row_length; i++, i0++, i1++)
            {
                swap(i0, i1);
            }
        }
        { // normalize the pivot element
            Tnum coeff = _1 / *ipivot; // reciprocal of the pivot element
            Pnum i0 = ipivot;
            Tsize iter_ctr = n_row_length;
            do {
                *i0++ *= coeff;
            } while (--iter_ctr);
        }
        // delete pivot column elements except pivot element itself.
        for (int irow1 = 0; irow1 != (int)n; irow1++)
        {
            if (irow == irow1) continue; // skip the pivot row
            Pnum itarget = ipivot + (int)_2n * (irow1 - irow); // begining of the row under pivot delete operation
            Pnum isource = ipivot;
            Tnum coeff = -(*itarget);
            Tsize iter_ctr = n_row_length;
            do {
                *itarget++ += coeff * *isource++;
            } while (--iter_ctr);
        }
    }
    return getsub(mwork, m, _2n, n, n, n, n, 0);
}

static Tenum soldense(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1)
{
    Tenum err = ESUCCESS;
    Pnum ipivot = m0;
    Tsize n_row_length = nc0;
    Tsize nr0_1 = nr0 - 1;
    Tsize step_pivot = nc0 + 1;
    do {
        if (nc0 <= nr0)
        {
            err = ERR_MATRIX_ILLFORMED;
            break;
        }
        // Gaussian elimination to get upper triangle matrix
        for (Tsize irow=0; irow != (int)nr0; irow++, n_row_length--, ipivot += step_pivot)
        { // swap rows if a better swap row is found
            Tsize irow_best = find_best_pivot_row(m0, nc0, nr0, irow);
            if (irow_best == nr0)
            {
                err = ERR_MATRIX_SINGULAR;
                break;
            }
            if (irow_best != irow)
            { // different best row was found, swap the default pivot row and the best one.
                Pnum i0 = ipivot;
                Pnum i1 = i0 + nc0 * (irow_best - irow);
                for (Tsize i = 0; i != n_row_length; i++, i0++, i1++)
                {
                    swap(i0, i1);
                }
            }
            { // normalize the pivot row
                Tnum coeff = _1 / *ipivot; // reciprocal of the pivot element
                Pnum i0 = ipivot;
                Tsize iter_ctr = n_row_length;
                do {
                    *i0++ *= coeff;
                } while (--iter_ctr);
            }
            if (irow != nr0_1)
            { // delete elements below pivot in the same column
                for (Tsize irow1 = irow + 1; irow1 != nr0; irow1++)
                {
                    Pnum itarget = ipivot + nc0 * (irow1 - irow); // target element of the elimination
                    Pnum isource = ipivot;
                    Tnum coeff = -(*itarget);
                    Tsize iter_ctr = n_row_length;
                    do {
                        *itarget++ += coeff * *isource++;
                    } while (--iter_ctr);
                }
            }
        }
        if (err == ESUCCESS)
        { // If no error was found in the Gaussian elimination above, do back-substitution.
            Tsize irow = nr0;
            Tsize ncols_x = nc0 - nr0; // solution matrix column count
            do {
                --irow;
                Pnum ix = m1 + ncols_x * irow; // solution element ptr
                CPnum iy = m0 + nc0 * irow + nr0; // equation RHS element ptr
                for (Tsize icolx = 0; icolx != ncols_x; icolx++, ix++, iy++)
                {
                    *ix = *iy;
                    CPnum iLHS = m0 + irow * nc0 + irow + 1;
                    CPnum ix_determined = ix + ncols_x;
                    for (Tsize icolm0 = irow+1; icolm0 < nr0; icolm0++, ix_determined+=ncols_x)
                    {
                        *ix -= *iLHS++ * *ix_determined;
                    }
                }
            } while (irow);
        }        
    } while (0);
    return err;
}

static Tenum writef(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf)
{
    // this implementation is not general. restricted only to float and double
    static const char* block_format[] = NUMFORMATS;
    for (Tsize ir = 0; ir != nr0; ir++)
    {
        for (Tsize ic = 0; ic != nc0; ic++)
        {
            WRITENUMF(pf, block_format[(ic != 0) ? 1 : 0], *m0++);
        }
        fprintf(pf, "\n");
    }
    fprintf(pf, "\n");
    return ESUCCESS;
}

static Tenum readf(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf)
{
    assert(0);
    return EINVAL;
}
#pragma endregion matlayoutRM_impl
#elif defined(MATLAYOUTCM)
#pragma region matlayoutCM_impl
static CPnum transpose(CPnum m0, Pnum m, Tsize nc, Tsize nr)
{
    Pnum im = m;
    for (Tsize icol = 0; icol != nc; icol++)
    {
        for (Tsize irow = 0; irow != nr; irow++)
        {
            *im++ = m0[IDXCM(irow,icol,nc)];
        }
    }
    return m;
}

static CPnum rotxrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[5] = m[10] = MATH_COS(s0);
    m[9] = -(m[6] = MATH_SIN(s0));
    return m;
}

static CPnum rotyrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[0] = m[10] = MATH_COS(s0);
    m[2] = -(m[8] = MATH_SIN(s0));
    return m;
}

static CPnum rotzrad(Tnum s0, Pnum m)
{
    unit(_1, m, 4);
    m[0] = m[5] = MATH_COS(s0);
    m[4] = -(m[1] = MATH_SIN(s0));
    return m;
}

// param v [in] 3D vector of moving
// param m [in] work matrix holding 4x4 homogenous translate matrix
static CPnum translate(CPnum v, Pnum m)
{
    unit(_1, m, 4);
    m[12] = v[0];
    m[13] = v[1];
    m[14] = v[2];
    return m;
}

static CPnum mul(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr)
{
    Pnum im = m;
    for (Tsize icol = 0; icol != nc; icol++)
    {
        for (Tsize irow = 0; irow != nr; irow++)
        {
            *im = _0;
            CPnum im0 = m0 + irow;
            CPnum im1 = m1 + icol * ni;
            for (Tsize iinter = 0; iinter != ni; iinter++)
            {
                *im += *im0 * *im1++;
                im0 += nr;
            }
            im++;
        }
    }
    return m;
}

static CPnum setsub(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff)
{
    assert(ncoff + nc1 <= nc0);
    assert(nroff + nr1 <= nr0);
    CPnum im1 = m1;
    Pnum im00 = m0 + nroff + ncoff * nr0;
    for (Tsize icol = 0; icol != nc1; icol++)
    {
        Pnum im0 = im00;
        for (Tsize irow = 0; irow != nr1; irow++)
        {
            *im0++ = *im1++;
        }
        im00 += nr0;
    }
    return m0;
}

static CPnum getsub(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff)
{
    assert(ncoff + nc1 <= nc0);
    assert(nroff + nr1 <= nr0);
    Pnum im1 = m1;
    CPnum im00 = m0 + nroff + ncoff * nr0;
    for (Tsize icol = 0; icol != nc1; icol++)
    {
        CPnum im0 = im00;
        for (Tsize irow = 0; irow != nr1; irow++)
        {
            *im1++ = *im0++;
        }
        im00 += nr0;
    }
    return m1;
}

static Tenum calc_normalized_norm(CPnum ipv, Tsize nsum, Tsize nr, Prnum normalized_norm)
{
    Tenum err = EFAULT;
    do {
        Trnum pivot_norm = L2NORM(*ipv);
        Trnum partial_sum = pivot_norm;
        for (Tsize i = 1; i < nsum; i++)
        {
            ipv += nr;
            partial_sum += L2NORM(*ipv);
        }
        *normalized_norm = _0;
        if (partial_sum > (MIN_L2SUM))
        {
            err = ESUCCESS;
            *normalized_norm = pivot_norm / partial_sum;
        }
    } while (0);
    return err;
}

static Tsize find_best_pivot_row(CPnum m0, Tsize nr, Tsize icol)
{
    Tsize irow_found = nr;
    Trnum normalized_norm = _0;
    do {
        CPnum ipv = m0 + icol + icol * nr;
        Tsize n_partial_sum_range = nr - icol;
        Trnum normalized_norm_temp = _0;
        for (Tsize irow = icol; irow != nr; irow++)
        {
            if (EFAULT == calc_normalized_norm(ipv, n_partial_sum_range, nr, &normalized_norm_temp))
            { // singular matrix error
                irow_found = nr;
                break;
            }
            if (normalized_norm_temp > normalized_norm)
            {
                normalized_norm = normalized_norm_temp;
                irow_found = irow;
            }
            ipv ++;
        }
    } while (0);
    return irow_found;
}

static CPnum inv(CPnum m0, Pnum m, Pnum mwork, Tsize n)
{
    Tsize _2n = 2 * n;
    setsub(mwork, m0, _2n, n, n, n, 0, 0); // copy m0 in the left half of mwork
    setsub(mwork, unit(_1, m, n), _2n, n, n, n, n, 0); // fill the right half of mwork with a unit matrix
    Pnum ipivot = mwork;
    Tsize step_pivot = n + 1;
    Tsize n_row_length = _2n;
    Tsize n_partial_sum_range = n;
    // delete lower-left triangle of the left half of mwork
    for (int irow = 0; irow != (int)n; irow++, ipivot += step_pivot, n_row_length--, n_partial_sum_range--)
    {
        Tsize irow_best = find_best_pivot_row(mwork, n, irow);
        if (irow_best != irow)
        { // swap rows if a better swap row is found.
            Pnum i0 = ipivot;
            Pnum i1 = i0 + (irow_best - irow);
            for (Tsize i = 0; i != n_row_length; i++, i0+=n, i1+=n)
            {
                swap(i0, i1);
            }
        }
        { // normalize the pivot row
            Tnum coeff = _1 / *ipivot; // reciprocal of the pivot element
            Pnum i0 = ipivot;
            Tsize iter_ctr = n_row_length;
            do {
                *i0 *= coeff;
                i0 += n;
            } while (--iter_ctr);
        }
        // delete pivot column elements except pivot element itself.
        for (int irow1 = 0; irow1 != (int)n; irow1++)
        {
            if (irow == irow1) continue; // skip the pivot row
            Pnum itarget = ipivot + (irow1 - irow); // begining of the row under pivot delete operation
            Pnum isource = ipivot;
            Tnum coeff = -(*itarget);
            Tsize iter_ctr = n_row_length;
            do {
                *itarget += coeff * *isource;
                itarget += n;
                isource += n;
            } while (--iter_ctr);
        }
    }
    return getsub(mwork, m, _2n, n, n, n, n, 0);
}

static Tenum soldense(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1)
{
    Tenum err = ESUCCESS;
    Pnum ipivot = m0;
    Tsize n_row_length = nc0;
    Tsize nr0_1 = nr0 - 1;
    Tsize step_pivot = nr0 + 1;
    do {
        if (nc0 <= nr0)
        {
            err = ERR_MATRIX_ILLFORMED;
            break;
        }
        // Gaussian elimination to get upper triangle matrix
        for (Tsize irow = 0; irow != nr0; irow++, n_row_length--, ipivot += step_pivot)
        {
            Tsize irow_best = find_best_pivot_row(m0, nr0, irow);
            if (irow_best == nr0)
            {
                err = ERR_MATRIX_SINGULAR;
                break;
            }
            if (irow_best != irow)
            { // swap rows if a better swap row is found
                Pnum i0 = ipivot;
                Pnum i1 = i0 + (irow_best - irow);
                for (Tsize i = 0; i != n_row_length; i++, i0+=nr0, i1+=nr0)
                {
                    swap(i0, i1);
                }
            }
            { // normalize the pivot row
                Tnum coeff = _1 / *ipivot; // reciprocal of the pivot element
                Pnum i0 = ipivot;
                Tsize iter_ctr = n_row_length;
                do {
                    *i0 *= coeff;
                    i0 += nr0; // move to the next column
                } while (--iter_ctr);
            }
            if (irow != nr0_1)
            { // delete pivot column elements below the pivot
                for (Tsize irow1 = irow+1; irow1 != nr0; irow1++)
                {
                    Pnum itarget = ipivot + irow1 - irow;
                    Pnum isource = ipivot;
                    Tnum coeff = -(*itarget);
                    Tsize iter_ctr = n_row_length;
                    do {
                        *itarget += coeff * *isource;
                        itarget += nr0; // move to the next column
                        isource += nr0;
                    } while (--iter_ctr);
                }
            }
        }
        if (err == ESUCCESS)
        { // if no error was found in the Gaussian elimination above, do back-substitution
            Tsize irow = nr0;
            Tsize ncols_x = nc0 - nr0;
            do {
                --irow;
                Pnum ix = m1 + irow; // solution element ptr
                CPnum iy = m0 + nr0 * nr0 + irow; // equation RHS element ptr
                for (Tsize icolx = 0; icolx != ncols_x; icolx++, ix+=nr0, iy+=nr0)
                {
                    *ix = *iy;
                    CPnum iLHS = m0 + nr0 * irow + irow + nr0;
                    CPnum ix_determined = ix + 1;
                    for (Tsize icolm0 = irow+1; icolm0 < nr0; icolm0++)
                    {
                        *ix -= *iLHS * *ix_determined;
                        iLHS += nr0; // move to next column
                        ix_determined++;
                    }
                }
            } while (irow);
        }
    } while (0);
    return err;
}

static Tenum writef(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf)
{
    // this implementation is not general. restricted only to float and double
    static const char* block_format[] = NUMFORMATS;
    for (Tsize ir = 0; ir != nr0; ir++)
    {
        for (Tsize ic = 0; ic != nc0; ic++)
        {
            WRITENUMF(pf, block_format[(ic != 0) ? 1 : 0], m0[IDXCM(ic,ir,nr0)]);
        }
        fprintf(pf, "\n");
    }
    fprintf(pf, "\n");
    return ESUCCESS;
}

static Tenum readf(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf)
{
    assert(0);
    return EINVAL;
}

#pragma endregion matlayoutCM_impl
#endif

static const STFNTABLE_T g_fntable = {
    rotxrad,rotyrad,rotzrad,
    rotxdeg,rotydeg,rotzdeg,
    translate,
    unit, fill, transpose,
    add, sub, mul, muls, inv,
    setsub, getsub,
    writef,writeb,readf,readb,
    soldense,
    equalv,
    crossproduct, normalize
};

CPSTFNTABLE_T   FNGET() { return &g_fntable; }