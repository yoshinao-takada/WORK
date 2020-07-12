#include    "base_o/BL_optc.h"

// evaluate penalty function of equality constraint
static BL_1r32_t eval_eqc(BL_1r32_t m, BL_1r32_t f)
{
    return m * f * f;
}

// evaluate penalty function of non-equality constraint
static BL_1r32_t eval_neqc(const BL_2r32_t m, BL_1r32_t f)
{
    return m[1] * (expf(m[0] * f) - 1.0f);
}

#define BL_OPT_EVAL_BREAK_OBJECTIVE(err_, fn_, vardim_, x_, params_, pf_) if \
(ESUCCESS != (err_=fn_(vardim_,x_,params_,pf_))) { break; }

// unconstrained objective function combining
// (1) main objective function (usual unconstrained objective)
// (2) equality constraints and their penalty multiplier
// (3) nonequality constraints and their penalty multiplier
static int BL_optc_objective(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    int err = ESUCCESS;
    pcBL_ConstrainedObjective_t cobj = (pcBL_ConstrainedObjective_t)params;
    do {
        BL_OPT_EVAL_BREAK_OBJECTIVE(err, cobj->main_objective, cobj->vardim, x, cobj->main_objective_params, f);

        // evaluate penalties of equality constraints
        BL_cptr_t i_eqc = BL_array_cbegin(cobj->eqc);
        for (uint32_t i = 0; i != cobj->eqc->unit_count; i++)
        {
            BL_1r32_t f_eqc;
            BL_OPT_EVAL_BREAK_OBJECTIVE(
                err,
                (BL_OBJECTIVE_FUNC)(*i_eqc._2uptr)[0],
                cobj->vardim,
                x,
                (const void*)(*i_eqc._2uptr)[1],
                &f_eqc
            );
            *f += eval_eqc(cobj->meqc, f_eqc);
        }
        if (err) break;

        // evaluate penalties of nonequality constraints
        BL_cptr_t i_neqc = BL_array_cbegin(cobj->neqc);
        for (uint32_t i = 0; i != cobj->neqc->unit_count; i++)
        {
            BL_1r32_t f_neqc;
            BL_OPT_EVAL_BREAK_OBJECTIVE(
                err,
                (BL_OBJECTIVE_FUNC)(*i_neqc._2uptr)[0],
                cobj->vardim,
                x,
                (const void*)(*i_neqc._2uptr)[1],
                &f_neqc
            );
            *f += eval_neqc(cobj->mneqc, f_neqc);
        }
    } while (0);
    return err;
}

void BL_ConstrainedObjective_ResetPenaltyMultipliers(pBL_ConstrainedObjective_t p)
{
    p->meqc = p->mneqc[0] = p->mneqc[1] = 1.0f;
}

void BL_ConstrainedObjective_ChangePenaltyMultipliers(pBL_ConstrainedObjective_t p, int32_t n)
{
    BL_1r32_t mm = 0.0f;
    if (n > 0)
    {
        mm = 2.0f;
    }
    else if (n < 0)
    {
        mm = 0.5f;
    }
    else
    {
        return;
    }
    n = abs(n);
    do {
        p->meqc *= mm;
        p->mneqc[0] *= mm;
        p->mneqc[1] /= mm;
    } while (--n);
}

int BL_ConstrainedObjective_run
(pBL_ConstrainedObjective_t p, pBL_cg_t pCG, uint32_t iter_SUMT, uint32_t iter_CG, BL_1r32_t* xv)
{
    int err =ESUCCESS;
    do {

    } while (0);
    return err;
}