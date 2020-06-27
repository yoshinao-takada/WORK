#include    "base_o/BL_steepestdescent.h"
#include    "base_o/BL_gradient.h"
#include    "base_l/BL_base.h"
#include    "base_l/BL_errno.h"
#include    <stdlib.h>

pBL_steepestdescent_t BL_steepestdescent_new(
    uint32_t vardim, const BL_1r32_t* xv0,
    BL_OBJECTIVE_FUNC fn_eval, const void* params,
    BL_1r32_t hmin)
{
    size_t cb_block = sizeof(BL_1r32_t) * vardim;
    size_t cb_data = cb_block * 3;
    size_t cb_alloc = sizeof(BL_steepestdescent_t) + cb_data;
    pBL_steepestdescent_t p = (pBL_steepestdescent_t)NULL;
    int err = ESUCCESS;
    do {
        p = (pBL_steepestdescent_t)malloc(cb_alloc);
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->vardim = vardim;
        p->xv = p->buf;
        p->deltaxv = p->xv + vardim;
        p->epsxv = p->deltaxv + vardim;
        BL_copy_array(p->xv, xv0, vardim);
        p->hmax = 1.0e4f * (p->hmin = hmin);
        for (uint32_t i = 0; i != vardim; i++)
        {
            p->deltaxv[i] = hmin;
            p->epsxv[i] = hmin * 1.0e-2f;
        }
        p->fn_eval = fn_eval;
        p->params = params;
        p->f = 0.0f;
        p->trace = stdout;
    } while (0);
    if (err)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

/*!
\brief if (p->xv is nearly equal xv) return ESUCCESS else return ERR_NO_CONVERGENCE
\param p [in] 
\param xv [in] minimum line-search result
\return convergence check result
*/
static int BL_steepestdescent_check_convergence(pBL_steepestdescent_t p, const BL_1r32_t* xv)
{
    int err = ESUCCESS;
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        if (fabsf(p->xv[i] - xv[i]) > p->epsxv[i])
        {
            err = ERR_NO_CONVERGENCE;
            break;
        }
    }
    return err;
}

int BL_steepestdescent_run(pBL_steepestdescent_t p, uint32_t max_iter)
{
    pcBL_matfnr32RM_t matfn = BL_matfnr32RM();
    int err = ESUCCESS;
    pBL_linesearch_t ls = NULL;
    BL_1r32_t *gradient = NULL;
    do {
        ls = BL_linesearch_new(
            p->fn_eval, p->params, p->vardim, p->xv, p->xv, p->hmin, p->hmax, p->hmin * 1.0e-2f);
        ls->trace = p->trace;
        size_t cb_alloc = sizeof(BL_1r32_t) * p->vardim;
        gradient = (BL_1r32_t*)malloc(cb_alloc);
        int no_convergence_flag = 1;
        do {
            // set the line search director vector
            if (ESUCCESS != (err = BL_gradientv(p->vardim, p->xv, p->params, p->fn_eval, p->deltaxv, ls->vdir)))
            {
                fprintf(p->trace, "%s,%d fail\n", __FUNCTION__, __LINE__);
                break;
            }
            BL_1r32_t norm2_grad;
            matfn->mul(ls->vdir, ls->vdir, &norm2_grad, 1, p->vardim, 1); // L2 norm of the gradient
            matfn->muls(ls->vdir, p->vardim, 1, -1.0f/sqrtf(norm2_grad), ls->vdir); // normalize vdir
            BL_copy_array(ls->xv0, p->xv, p->vardim); // set line-search initial point
            if (ESUCCESS != (err = BL_linesearch_run(ls, 10)))
            {
                fprintf(p->trace, "%s,%d fail\n", __FUNCTION__, __LINE__);
                break;
            }
            if (ESUCCESS == (err = BL_steepestdescent_check_convergence(p, ls->xv[1])))
            {
                no_convergence_flag = 0;
            }
            p->f = ls->f[1];
            BL_copy_array(p->xv, ls->xv[1], p->vardim);
            BLSD_TRACE(p);
        } while (--max_iter && no_convergence_flag);
    } while (0);
    BL_SAFEFREE(&ls);
    BL_SAFEFREE(&gradient);
    return err;
}

void BL_steepestdescent_trace_base(pcBL_steepestdescent_t p)
{
    if (!(p->trace)) return;
    fprintf(p->trace, "BL_steepestdescent,xv");
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        fprintf(p->trace, ",%f", p->xv[i]);
    }
    fprintf(p->trace, ",f,%f\n", p->f);
}