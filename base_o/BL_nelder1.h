#ifndef BL_NELDER1_H_
#define BL_NELDER1_H_
#include    "base_o/BL_optimize.h"
#ifdef  __cplusplus
extern "C" {
#endif
typedef struct {
    FILE* pf;
    BL_OBJECTIVE_FUNC fn_eval;
    const void* params;
    BL_2r32_t x;
    BL_2r32_t f;
    BL_1r32_t eps;
} BL_nelder1_t, *pBL_nelder1_t;
typedef const BL_nelder1_t *pcBL_nelder1_t;

/*!
\brief constructor for BL_nelder1_t object
*/
pBL_nelder1_t BL_nelder1_new(BL_OBJECTIVE_FUNC fn_eval, const void* params, BL_1r32_t x0, BL_1r32_t eps);

/*!
\brief optimizing operation
*/
int BL_nelder1_run(pBL_nelder1_t p, uint32_t iter_max);

/*!
\brief test objective function
*/
int BL_nelder1_testfunc(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);
#ifdef  __cplusplus
}
#endif
#endif /* BL_NELDER1_H_ */