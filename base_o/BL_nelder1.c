#include    "base_o/BL_nelder1.h"
#include    <stdio.h>
#include    <stdlib.h>
#include    <math.h>
pBL_nelder1_t BL_nelder1_new(BL_OBJECTIVE_FUNC fn_eval, const void* params, BL_1r32_t x0, BL_1r32_t eps)
{
    pBL_nelder1_t p = (pBL_nelder1_t)malloc(sizeof(pBL_nelder1_t));
    p->pf = stdout;
    p->fn_eval = fn_eval;
    p->params = params;
    p->x[0] = x0;
    p->x[1] = x0 + 1.0f;
    p->eps = eps;
    return p;
}

typedef enum {
    BL_nelder1_contract,
    BL_nelder1_contract_reflect,
    BL_nelder1_reflect,
    BL_nelder1_extend_reflect,
    BL_nelder1_positions,
} next_position_t;

int BL_nelder1_update(pBL_nelder1_t p, const BL_2u32_t ranking)
{
    BL_1r32_t c = p->x[ranking[0]]; // centroid of good points
    BL_1r32_t c_x_n = c - p->x[ranking[1]]; // c - x(n)
    BL_1r32_t x_contract = p->x[ranking[1]] + 0.5f * c_x_n;
    {
        BL_1r32_t f_next[BL_nelder1_positions];
        BL_1r32_t x_next[BL_nelder1_positions] =
        {
            c - 0.5f * c_x_n,
            c + 0.5f * c_x_n,
            c + c_x_n,
            c + 2.0f * c_x_n
        };
        BL_1r32_t f_min = FLT_MAX;
        BL_1r32_t x_argmin = 0.0f;
        for (uint32_t i = 0; i != (uint32_t)BL_nelder1_positions; i++)
        {
            p->fn_eval(1, &x_next[i], p->params, &f_next[i]);
            f_min = __min(f_min, f_next[i]);
            if (f_min == f_next[i])
            {
                x_argmin = x_next[i];
            }
        }
        p->x[ranking[1]] = x_argmin;
        p->f[ranking[1]] = f_min;
    }
    return (fabsf(p->x[0] - p->x[1]) < p->eps) ? ESUCCESS : ERR_NO_CONVERGENCE;
}

static void PrintHeader(FILE* pf)
{
    if (pf)
    {
        fprintf(pf, "x[0],x[1],f(x[1]),f(x[0])\n");
    }
}

static void PrintData(pBL_nelder1_t p)
{
    if (p->pf)
    {
        fprintf(p->pf, "%f,%f,%f,%f\n", p->x[0], p->x[1], p->f[0], p->f[1]);
    }
}

int BL_nelder1_run(pBL_nelder1_t p, uint32_t iter_max)
{
    int err = ERR_NO_CONVERGENCE;
    PrintHeader(p->pf);
    for (uint32_t i = 0; i != iter_max; i++)
    {
        p->fn_eval(1, &p->x[0], p->params, &p->f[0]);
        p->fn_eval(1, &p->x[1], p->params, &p->f[1]);
        PrintData(p);
        BL_2u32_t ranking;
        if (p->f[0] > p->f[1])
        {
            ranking[0] = 1;
            ranking[1] = 0;
        }
        else
        {
            ranking[0] = 0;
            ranking[1] = 1;
        }
        if (ESUCCESS == (err = BL_nelder1_update(p, ranking)))
        { // detected convergence
            break;
        }
    }
    return err;
}

int BL_nelder1_testfunc(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    *f = fabsf(x[0]*x[0] - 1.0f);
    return 0;
}
