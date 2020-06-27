#include    "base_o/BL_cg.h"
#include    "base_l/BL_errno.h"
#include    "base_o/BL_gradient.h"
#include    "base_o/BL_linesearch.h"
#include    "base_g/BL_matexport.h"
#include    <stdlib.h>

static pcBL_matfnr32RM_t matfn = NULL; // utility function table for geometry and matrix operation

pBL_cg_t BL_cg_new(
    uint32_t vardim,
    BL_1r32_t hmin,
    const BL_1r32_t *xv0,
    BL_OBJECTIVE_FUNC fn_eval,
    const void* params)
{
    if (!matfn) matfn = BL_matfnr32RM();
    size_t cb_block = sizeof(BL_1r32_t) * vardim;
    size_t cb_data = cb_block * 3;
    size_t cb_alloc = sizeof(BL_cg_t) + cb_data;
    pBL_cg_t p = (pBL_cg_t)malloc(cb_alloc);
    if (p)
    {
        p->vardim = vardim;
        p->hmin = hmin;
        p->hmax = hmin * 1.0e4f;
        p->trace = stdout;
        p->fn_eval = fn_eval;
        p->params = params;
        p->dxv = p->buf;
        p->epsxv = p->dxv + vardim;
        p->xv = p->epsxv + vardim;
        BL_copy_array(p->xv, xv0, vardim);
        for (uint32_t i = 0; i != vardim; i++)
        {
            p->dxv[i] = hmin;
            p->epsxv[i] = hmin * 1.0e-2f;
        }
    }
    return p;
}

static int BL_cg_converged(pcBL_cg_t cg, BL_1r32_t *xv)
{
    int err = ESUCCESS;
    for (uint32_t i = 0; i != cg->vardim; i++)
    {
        BL_1r32_t diff_i = xv[i] - cg->xv[i];
        if (fabsf(diff_i) > cg->epsxv[i])
        {
            err = ERR_NO_CONVERGENCE;
            break;
        }
    }
    return err;
}

// get a unit length vector with the same direction as v.
static void normalize(uint32_t vardim, const BL_1r32_t* v, BL_1r32_t* vnormalized)
{
    BL_1r32_t vdotv;
    matfn->mul(v, v, &vdotv, 1, vardim, 1);
    BL_1r32_t normalizing_coeff = 1.0f/sqrtf(vdotv);
    matfn->muls(v, vardim, 1, normalizing_coeff, vnormalized);
}

int BL_cg_run(pBL_cg_t p, uint32_t iter_max)
{
    int err = ESUCCESS;
    pBL_2vecq_t gradf = NULL, u = NULL;
    BL_1r32_t norm_gradfi, norm_gradfi_1, betai;
    BL_1r32_t *betai_ui;
    pBL_linesearch_t ls = NULL;
    uint32_t vardim = p->vardim;
    do {
        gradf = BL_2vecq_new(vardim);
        u = BL_2vecq_new(vardim);
        betai_ui = (BL_1r32_t*)calloc(vardim, sizeof(BL_1r32_t));
        ls = BL_linesearch_new(p->fn_eval, p->params, vardim, p->xv, p->xv, p->hmin, p->hmax, p->epsxv[0]);
        if (!gradf || !u || !betai_ui || !ls)
        {
            err = ENOMEM;
            break;
        }
        ls->trace = p->trace;
        { // 1st iteration
            BL_1r32_t* gradfx0 = BL_2vecq_peek(gradf);
            if (ESUCCESS != (err = BL_gradientv(vardim, p->xv, p->params, p->fn_eval, p->dxv, gradfx0)))
            { // error in calculating gradient(f(x0))
                break;
            }
            BL_2vecq_push(gradf, gradfx0);
            BL_1r32_t* u1 = BL_2vecq_peek(u);
            matfn->muls(gradfx0, vardim, 1, -1.0f, u1);
            BL_2vecq_push(u, u1);
            normalize(vardim, u1, ls->vdir);
            if (ESUCCESS != (err = BL_linesearch_run(ls, 10)))
            {
                break;
            }
            BL_copy_array(p->xv, ls->xv[1], vardim);
            p->f = ls->f[1];
            BL_CG_TRACE(p); ////// Debug trace
        }
        int not_converged = 1;
        do {
            BL_1r32_t* gradfxi = BL_2vecq_peek(gradf);
            if (ESUCCESS != (err = BL_gradientv(vardim, p->xv, p->params, p->fn_eval, p->dxv, gradfxi)))
            { // error in calculating gradient(f(xi))
                break;
            }
            BL_2vecq_push(gradf, gradfxi);
            matfn->mul(gradf->v[0], gradf->v[0], &norm_gradfi, 1, vardim, 1);
            matfn->mul(gradf->v[1], gradf->v[1], &norm_gradfi_1, 1, vardim, 1);
            betai = norm_gradfi/norm_gradfi_1;
            matfn->muls(u->v[0], vardim, 1, betai, betai_ui);
            BL_1r32_t* ui_1 = BL_2vecq_peek(u);
            matfn->sub(betai_ui, gradf->v[0], ui_1, vardim, 1);
            BL_2vecq_push(u, ui_1);
            normalize(vardim, ui_1, ls->vdir);
            BL_copy_array(ls->xv0, p->xv, vardim);
            if (ESUCCESS != (err = BL_linesearch_run(ls, 10)))
            {
                break;
            }
            if (ESUCCESS == BL_cg_converged(p, ls->xv[1]))
            {
                not_converged = 0;
            }
            BL_copy_array(p->xv, ls->xv[1], vardim);
            p->f = ls->f[1];
            BL_CG_TRACE(p); ////// Debug trace
        } while (--iter_max && not_converged);
    } while (0);
    BL_SAFEFREE(&gradf);
    BL_SAFEFREE(&u);
    BL_SAFEFREE(&betai_ui);
    BL_SAFEFREE(&ls);
    return err;
}

void BL_cg_trace_base(pcBL_cg_t p)
{
    if (!(p->trace)) return;
    fprintf(p->trace, "BL_cg,%s,xv");
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        fprintf(p->trace, ",%f", p->xv[i]);
    }
    fprintf(p->trace, "f(xv),%f\n", p->f);
}
