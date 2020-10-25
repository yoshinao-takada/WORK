#include    "base_l/BL_futils.h"
#include    "base_o/BL_linesearch.h"
#include    "base_l/BL_errno.h"
#include    <stdlib.h>
static pcBL_matfnr32RM_t matfn = (pcBL_matfnr32RM_t)NULL;

pBL_linesearch_t BL_linesearch_new(
    BL_OBJECTIVE_FUNC fn_eval, const void* params, 
    uint32_t vardim, const BL_1r32_t* xv0, const BL_1r32_t* vdir,
    BL_1r32_t hmin, BL_1r32_t hmax, BL_1r32_t eps)
{
    if (matfn == (pcBL_matfnr32RM_t)NULL)
    {
        matfn = BL_matfnr32RM();
    }
    pBL_linesearch_t p = (pBL_linesearch_t)NULL;
    do {
        size_t cb_block = sizeof(BL_1r32_t) * vardim;
        size_t cb_data = cb_block * 6;
        size_t cb_alloc = sizeof(BL_linesearch_t) + cb_data;
        p = (pBL_linesearch_t)malloc(cb_alloc);
        p->trace = stdout;
        p->vdir = p->buffer;
        p->xv0 = p->vdir + vardim;
        p->xvdir = p->xv0 + vardim;
        p->xv[0] = p->xvdir + vardim;
        p->xv[1] = p->xv[0] + vardim;
        p->xv[2] = p->xv[1] + vardim;
        p->fn_eval = fn_eval;
        p->params = params;
        p->vardim = vardim;
        BL_copy_array(p->xv0, xv0, vardim);
        BL_copy_array(p->vdir, vdir, vardim);
        p->hmin = hmin;
        p->hmax = hmax;
        p->h = sqrtf(hmin * hmax);
        p->eps = eps;
        p->xm = 0.0f;
        p->x[0] = -(p->h);
        p->x[1] = p->x[0] + p->h;
        p->x[2] = p->x[1] + p->h;
    } while (0);
    return p;
}

/*!
\brief objective function evaluation with error break statement
\param e [out] error code
\param n_dim [in] dimension of x
\param x [in] independent variables
\param params[in] fixed parameter of func
\param func [in] objective function
\param pfunc_val [out] evaluated value of func
*/
#define BL_EVAL_FUNC(e,n_dim,x,params,func,pfunc_val) \
if (ESUCCESS != (e = func(n_dim,x,params,pfunc_val))) { break; }

BL_1r32_t BL_linesearch_xm(const BL_3r32_t x, const BL_3r32_t f)
{
    BL_2r32_t xy[3] = { { x[0], f[0] }, { x[1], f[1] }, {x[2], f[2]} };
    BL_1r32_t F012 = BL_dd2(xy[0], xy[1], xy[2]);
    return (F012 * (xy[0][0] + xy[1][0]) - BL_dd1(xy[0], xy[1]))/(2.0f * F012);
}

int BL_linesearch_run(pBL_linesearch_t p, uint32_t iter_max)
{
    int err = ERR_NO_CONVERGENCE;
    int break_flag = 0;
    do {
        for (uint32_t i = 0; i != 3; i++)
        {
            matfn->muls(p->vdir, 1, p->vardim, p->x[i], p->xvdir);
            matfn->add(p->xv0, p->xvdir, p->xv[i], 1, p->vardim);
            BL_EVAL_FUNC(err, p->vardim, p->xv[i], p->params, p->fn_eval, &(p->f[i]));
        }
        if (break_flag) break;
        BL_1r32_t fmax = __max(p->f[0], __max(p->f[1], p->f[2]));
        BL_1r32_t fmin = __min(p->f[0], __min(p->f[1], p->f[2]));
        if ((fmax - fmin) < DFMIN)
        {
            if (p->h*2.0f <= p->hmax)
            {
                p->h *= 2.0f;
            }
            else
            { // hmax is too small or (d/dx)(fn_eval(x)) is too flat
                fprintf(p->trace, "%s,%d,p->h = %f, fmax = %f, fmin = %f\n", __FUNCTION__, __LINE__,
                    p->h, fmax, fmin);
                err = EINVAL;
                break;
            }
        }
        p->xm = BL_linesearch_xm(p->x, p->f);
        if (((p->xm - p->x[2]) > p->hmax) || ((p->x[0] - p->xm) > p->hmax))
        {
            p->h = __min(2.0f * p->h, p->hmax);
        }
        else if ((p->x[0] < p->xm) && (p->xm < p->x[2]))
        {
            p->h = __max(0.5f * p->h, p->hmin);
        }
        if (fabsf(p->x[1] - p->xm) < p->eps)
        {
            err = ESUCCESS;
            break_flag = 1;
        }
        p->x[0] = p->xm - p->h;
        p->x[1] = p->xm;
        p->x[2] = p->xm + p->h;
        BL_LINESEARCH_TRACE(p);
    } while (--iter_max);
    return err;
}

void    BL_linesearch_trace_base(pcBL_linesearch_t p)
{
    if (!(p->trace)) return;
    fprintf(p->trace, "BL_linesearch,xv[1]");
    for (uint32_t i =0; i != p->vardim; i++)
    {
        fprintf(p->trace, ",%f", p->xv[1][i]);
    }
    fprintf(p->trace, ",x[1],%f,f[1],%f\n", p->x[1], p->f[1]);
}

void    BL_linesearchGS_trace_base(pcBL_linesearchGS_t p)
{
    if (!(p->trace)) return;
    fprintf(p->trace, "BL_linesearch,xv[1]");
    for (uint32_t i =0; i != p->vardim; i++)
    {
        fprintf(p->trace, ",%f", p->xv[1][i]);
    }
    fprintf(p->trace, ",x[1],%f,f[1],%f\n", p->x[1], p->f[1]);
}

#define L0(OBJ) (OBJ->x[3] - OBJ->x[0])

pBL_linesearchGS_t BL_linesearchGS_new(
    BL_OBJECTIVE_FUNC fn_eval, const void* params, 
    uint32_t vardim, const BL_1r32_t* xv0, const BL_1r32_t* vdir, BL_1r32_t h, BL_1r32_t eps)
{
    if (matfn == (pcBL_matfnr32RM_t)NULL)
    {
        matfn = BL_matfnr32RM();
    }
    pBL_linesearchGS_t p = (pBL_linesearchGS_t)NULL;
    BL_1r32_t *xvdir = NULL;
    int err = ESUCCESS;
    do {
        if (NULL == (xvdir = (BL_1r32_t*)calloc(vardim, sizeof(BL_1r32_t))))
        {
            break;
        }
        size_t cb_block = sizeof(BL_1r32_t) * vardim;
        size_t cb_data = cb_block * 6;
        size_t cb_alloc = sizeof(BL_linesearchGS_t) + cb_data;
        p = (pBL_linesearchGS_t)malloc(cb_alloc);
        p->trace = stdout;
        p->vdir = p->buffer;
        p->xv0 = p->vdir + vardim;
        p->xv[0] = p->vdir + vardim;
        p->xv[1] = p->xv[0] + vardim;
        p->xv[2] = p->xv[1] + vardim;
        p->xv[3] = p->xv[2] + vardim;
        p->fn_eval = fn_eval;
        p->params = params;
        p->vardim = vardim;
        BL_copy_array(p->xv0, xv0, vardim);
        BL_copy_array(p->vdir, vdir, vardim);
        p->eps = eps;
        p->r = GOLDENSECTION_R;
        p->r2 = GOLDENSECTION_R * GOLDENSECTION_R;
        p->x[0] = -h;
        p->x[3] = h;
        p->x[1] = p->x[0] + p->r * p->r * L0(p);
        p->x[2] = p->x[0] + p->r * L0(p);
        for (uint32_t i = 0; i != ARRAYSIZE(p->x); i++)
        {
            matfn->muls(p->vdir, p->vardim, 1, p->x[i], xvdir); // xvdir = vdir * x[i]
            matfn->add(xvdir, p->xv0, p->xv[i], vardim, 1); // xv[i] = xv0 + vdir * x[i]
            BL_EVAL_FUNC(err, p->vardim, p->xv[i], p->params, p->fn_eval, &p->f[i]); // f = fn_eval(xv[i][*], param)
        }
    } while (0);
    BL_SAFEFREE(&xvdir);
    return p;
}


// int BL_linesearchGS_run(pBL_linesearchGS_t p, uint32_t iter_max)
// {
//     int err = ESUCCESS;
//     do {
//         if (p->f[0] >= p->f[1] && p->f[1] >= p->f[2] && p->f[2] >= p->f[3])
//         {
//             err = BL_linesearchGS_search_forward(p, iter_max);
//         }
//         else if (p->f[0] <= p->f[1] && p->f[1] <= p->f[2] && p->f[2] <= p->f[3])
//         {
//             err = BL_linesearchGS_search_backward(p, iter_max);
//         }
//         else if (p->f[0] <= p->f[1] && p->f[2] >= p->f[3])
//         {
//             err = BL_linesearchGS_search_both(p, iter_max);
//         }
//         else
//         {
//             err = BL_linesearchGS_search_core(p, iter_max);
//         }
//     } while (0);
//     return err;
// }

// int BL_linesearchGS_search_core(pBL_linesearchGS_t p, uint32_t iter_max)
// {
//     int err = ESUCCESS;
//     do {
//         uint32_t i_begin, i_end;
//         while (L0(p) > p->eps)
//         {
//             if (p->f[1] > p->f[2])
//             {
//                 p->x[0] = p->x[1];
//                 i_begin = 1;
//                 i_end = 4;
//             }
//             else
//             {
//                 i_begin = 0;
//                 i_end = 3;
//                 p->x[3] = p->x[2];
//             }
//             p->x[1] = p->x[0] + p->r2 * L0(p);
//             p->x[2] = p->x[0] + p->r * L0(p);
//             for (uint32_t i = i_begin; i != i_end; i++)
//             {
//                 fn_eval->muls(p->vdir, p->vardim, 1, p->x[i], xvdir);
//                 fn_eval->add(xvdir, p->xv0, p->xv[i], vardim, 1);
//                 BL_EVAL_FUNC(err, p->vardim, p->xv[i], p->params, p->fn_eval, &p->f);
//             }
//         }
//     } while (0);
//     return err;
// }


// int BL_linesearchGS_search_forward(pBL_linesearchGS_t p, uint32_t iter_max)
// {
//     int err = ERR_NO_CONVERGENCE;
//     do {
//         uint32_t iter_forward = iter_max;
//         BL_1r32_t step = p->x[3] - p->x[2];
//         do {
//             p->x[0] += step;
//             p->x[3] += step;
//             p->x[1] = p->x[0] + p->r2 * L0(p);
//             p->x[2] = p->x[0] + p->r * L0(p);
//             for (uint32_t i = 0; i != p->vardim; i++)
//             {
//                 fn_eval->muls(p->vdir, p->vardim, 1, p->x[i], xvdir);
//                 fn_eval->add(xvdir, p->xv0, p->xv[i], vardim, 1);
//                 BL_EVAL_FUNC(err, p->vardim, p->xv[i], p->params, p->fn_eval, &p->f);
//             }
//             if (p->x[0] > p->x[1] && p0>x[2] < p->x[3])
//             {
//                 err = ESUCCESS;
//                 break;
//             }
//         } while (--iter_forward);
//         if (err) break;
//         err = BL_linesearchGS_search_core(p, iter_max);
//     } while (0);
//     return err;
// }

// int BL_linesearchGS_search_backward(pBL_linesearchGS_t p, uint32_t iter_max)
// {
//     int err = ERR_NO_CONVERGENCE;
//     do {
//         uint32_t iter_backward = iter_max;
//         BL_1r32_t step = p->x[3] - p->x[2];
//         do {
//             p->x[0] -= step;
//             p->x[3] -= step;
//             p->x[1] = p->x[0] + p->r2 * L0(p);
//             p->x[2] = p->x[0] + p->r * L0(p);
//             for (uint32_t i = 0; i != p->vardim; i++)
//             {
//                 fn_eval->muls(p->vdir, p->vardim, 1, p->x[i], xvdir);
//                 fn_eval->add(xvdir, p->xv0, p->xv[i], vardim, 1);
//                 BL_EVAL_FUNC(err, p->vardim, p->xv[i], p->params, p->fn_eval, &p->f);
//             }
//             if (p->x[0] > p->x[1] && p0>x[2] < p->x[3])
//             {
//                 err = ESUCCESS;
//                 break;
//             }
//         } while (--iter_backward);
//         if (err) break;
//         err = BL_linesearchGS_search_core(p, iter_max);
//     } while (0);
//     return err;
// }

// int BL_linesearchGS_search_both(pBL_linesearchGS_t p, uint32_t iter_max)
// {
//     int err = ESUCCESS;
//     do {
//         if (ESUCCESS == (err = BL_linesearchGS_search_forward(p, iter_max)))
//         {
//             break;
//         }
//         err = BL_linesearchGS_search_backward(p, iter_max);
//     } while (0);
//     return err;
// }
