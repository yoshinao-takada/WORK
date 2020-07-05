#include    "base_o/BL_constraint.h"
#include    "base_l/BL_base.h"

pBL_constraint_t BL_constraint_new(
    pBL_array_t eqc,
    pBL_array_t neqc,
    uint32_t vardim_objective,
    BL_OBJECTIVE_FUNC fn_objective,
    const void* params_objective)
{
    pBL_constraint_t p = (pBL_constraint_t)malloc(sizeof(BL_constraint_t));
    if (p)
    {
        p->eqc = eqc;
        p->neqc = neqc;
        p->vardim_objective = vardim_objective;
        p->eqc_dim = eqc->unit_count;
        p->neqc_dim = neqc->unit_count;
        p->vardim = vardim_objective + p->eqc_dim + p->neqc_dim;
        p->fn_objective = fn_objective;
        p->params_objective = params_objective;
    }
    return p;    
}


int BL_constraint_run(uint32_t iter_max_SUMT, uint32_t iter_max_cg)
{
    int err = ESUCCESS;
    do {
        do {
            
        } while (--iter_max_SUMT);
    } while (0);
    return err;
}