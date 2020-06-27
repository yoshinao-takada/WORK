#ifndef BL_STEEPESTDESCENT_H_
#define BL_STEEPESTDESCENT_H_
#include    "base_o/BL_linesearch.h"
#include    "base_l/BL_futils.h"
#ifdef  __cplusplus
extern "C" {
#endif
typedef struct {
    // independent variable vector and its dimension
    uint32_t vardim;
    BL_1r32_t* xv;
    
    // objective function
    BL_OBJECTIVE_FUNC fn_eval;
    const void* params;
    BL_1r32_t   f; // function value at current iteration

    // small numbers like tolerences, differentiation steps, etc.
    BL_1r32_t hmin, hmax; // line search differentiation step limits
    BL_1r32_t* deltaxv; // gradient differentation step
    BL_1r32_t* epsxv; // x convergence limit

    FILE* trace;
    BL_1r32_t buf[0];
} BL_steepestdescent_t, *pBL_steepestdescent_t;
typedef const BL_steepestdescent_t *pcBL_steepestdescent_t;

pBL_steepestdescent_t BL_steepestdescent_new(
    uint32_t vardim, const BL_1r32_t* xv0,
    BL_OBJECTIVE_FUNC fn_eval, const void* params,
    BL_1r32_t hmin);

/*!
\brief execute steepest descent optimization procedure
\param p [in,out]
\param max_iter [in] iteration limit
*/
int BL_steepestdescent_run(pBL_steepestdescent_t p, uint32_t max_iter);

void BL_steepestdescent_trace_base(pcBL_steepestdescent_t p);

#ifdef _DEBUG
#define BLSD_TRACE(p)   BL_steepestdescent_trace_base(p)
#else
#define BLSD_TRACE(p) /* do nothing */
#endif

#ifdef  __cplusplus
}
#endif
#endif
