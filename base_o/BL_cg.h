#ifndef BL_CG_H_
#define BL_CG_H_
#include    "base_o/BL_optimize.h"
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t vardim; // dimension of independent variable vector
    BL_1r32_t hmin, hmax; // lower and upper limits of line-search differentiation step
    BL_1r32_t f;  // fn_eval value at the latest iteration
    BL_OBJECTIVE_FUNC   fn_eval;
    const void* params; // fixed parameters of fn_eval
    BL_1r32_t   *dxv; // differentiation step for gradient calculation
    BL_1r32_t   *epsxv; // convergence criteria for xv change
    BL_1r32_t   *xv; // independent variable vector at the latest iteration
    FILE* trace; // output stream for debug trace
    BL_1r32_t   buf[0];
} BL_cg_t, *pBL_cg_t;
typedef const BL_cg_t *pcBL_cg_t;

/*!
\brief create a new CG object
\param vardim [in] dimension of independent variable vector
\param hmin [in] lower limit of line-search differentiation step
\param xv0 [in] initial value of independent variable vector
\param fn_eval [in] objective function of minimization
\param params [in] fixed parameters of fn_eval
*/
pBL_cg_t BL_cg_new(
    uint32_t vardim,
    BL_1r32_t hmin,
    const BL_1r32_t *xv0,
    BL_OBJECTIVE_FUNC fn_eval,
    const void* params);

/*!
\brief execute conjugate gradient optimization procedure
\param p [in,out]
\param iter_max [in] maximum iteration limit
\return ESUCCESS: successfully converted. Others: something bad
*/
int BL_cg_run(pBL_cg_t p, uint32_t iter_max);

/*!
\brief debug trace
*/
void BL_cg_trace_base(pcBL_cg_t p);

#ifdef  _DEBUG
#define BL_CG_TRACE(p)  BL_cg_trace_base(p)
#else
#define BL_CG_TRACE(p)  /* do nothing */
#endif
#ifdef  __cplusplus
}
#endif
#endif
