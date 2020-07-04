#include    "base_o/BL_optc.h"

BL_1r32_t BL_evaleqc(BL_1r32_t m, BL_1r32_t f)
{
    return m * f * f;
}

BL_1r32_t BL_evalneqc(BL_1r32_t m, BL_1r32_t f)
{
    return expf(m*f) - 1.0f;
}

static void Copy_array_2uptr(pBL_array_t dst, pcBL_array_t src)
{
    BL_cptr_t i_src = BL_array_cbegin(src);
    BL_ptr_t i_dst = BL_array_begin(dst);
    for (uint32_t i = 0; i != src->unit_count; i++)
    {
        BL_copy2(*i_dst._2uptr, *i_src._2uptr);
        i_dst._2uptr++;
        i_src._2uptr++;
    }
}

int BL_OptConstranied_create(
    pcBL_OptFnParam_t problem_functions, const BL_1r32_t* xvini, BL_1r32_t xvtol, BL_1r32_t hmin,
    pBL_OptConstrained_t* ppcgparams, pBL_cg_t* ppcg)
{
    int err = ESUCCESS;
    do {
        if (NULL == (*ppcgparams = (pBL_OptConstrained_t)malloc(sizeof(BL_OptConstrained_t))))
        {
            break;
        }
        pBL_OptConstrained_t pcgparams = *ppcgparams;
        pcgparams->base.eqc = problem_functions->eqc;
        pcgparams->base.neqc = problem_functions->neqc;
        pcgparams->base.objective = problem_functions->objective;
        pcgparams->base.objective_params = problem_functions->objective_params;
        pcgparams->base.vardim = problem_functions->vardim;
        pcgparams->meqc = pcgparams->mneqc = 1.0f;
        pcgparams->xvini = BL_array_new(pcgparams->base.vardim, BL_1r32_t);
    } while (0);
    return err;
}