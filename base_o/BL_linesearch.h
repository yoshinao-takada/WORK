#ifndef BL_LINESEARCH_H_
#define BL_LINESEARCH_H_
#include    "base_o/BL_optimize.h"
#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief data structure for working variables for Powell's interpolation for multi-dimensional optimization
line-search.
References
[1] Jan A. Snyman, Daniel N. Wilke,"Practical Methematical Optimization - Basic Optimization Theory and
    Gradient-Based Algorithms, Second Edition", Springer, ISSN 1931-6828, ISSN 1931-6836,
    ISBN 978-3-319-77585-2, ISBN 978-3-319-77586-9
*/
typedef struct 
{
    FILE*       trace; // trace stream for debugging
    BL_OBJECTIVE_FUNC fn_eval;
    const void* params;
    uint32_t    vardim; // dimension of variables
    BL_1r32_t   h, hmin, hmax; // minimum seach step and twice of it.
    BL_1r32_t   *vdir; // search direction unit vector
    BL_1r32_t   *xv0; // search line initial point
    BL_1r32_t   *xvdir; // work vector
    BL_1r32_t*  xv[3]; // xv[1] final value is the minimum of fn_eval
    BL_3r32_t   x, f;
    BL_1r32_t   xm, eps;
    BL_1r32_t   buffer[0];
} BL_linesearch_t, *pBL_linesearch_t;
typedef const BL_linesearch_t *pcBL_linesearch_t;
#define DFMIN   1.0e-4f

/*!
\brief create a new object. free(), BL_SAFEFREE() is applicable to delete the object.
\param fn_eval [in] cost function (objective function)
\param params [in] fixed parameter of fn_eval
\param vardim [in] dimension of independent variable vector of fn_eval
\param x00 [in] initial value of the independent variable
\param vdir [in] search line direction vector
\param hmin, hmax [in] scalar coeff; hmin < h < hmax, h*vdir is the step of numerical derivative
\param eps [in] convergence condition (|xm - x2| < eps)
*/
pBL_linesearch_t BL_linesearch_new(
    BL_OBJECTIVE_FUNC fn_eval, const void* params, 
    uint32_t vardim, const BL_1r32_t* xv0, const BL_1r32_t* vdir,
    BL_1r32_t hmin, BL_1r32_t hmax, BL_1r32_t eps);

/*!
\brief iterate minimum search procedure. The iteration is terminated by reaching iter_max or convergence.
\param iter_max [in] iteration limit
\return result indicator, ESUCCESS or ERR_NO_CONVERGENCE
*/
int BL_linesearch_run(pBL_linesearch_t obj, uint32_t iter_max);

BL_1r32_t BL_linesearch_xm(const BL_3r32_t x, const BL_3r32_t f);

void    BL_linesearch_trace_base(pcBL_linesearch_t p);

#ifdef _DEBUG
#define BL_LINESEARCH_TRACE(p)  BL_linesearch_trace_base(p)
#else
#define BL_LINESEARCH_TRACE(p)  /* do nothing */
#endif
#ifdef __cplusplus
}
#endif
#endif
