#ifndef BL_OPTC_H_
#define BL_OPTC_H_
/*!
\file defines constraint optimization solver related struct and declares functions.
*/
#include    "base_l/BL_containers.h"
#include    "base_l/BL_types.h"
#include    "base_o/BL_cg.h"
#ifdef __cplusplus
extern "C" {
#endif

// 1. equality constraints are defined as function value is equal to zero.
// eqc element is BL_2uptr_t. The 1st element is uptr_t casted BL_OBJECTIVE_FUNC.
// The 2nd element is (const void*) casted fixed parameters of the constraint function.
// In evaluating the equality constraints, the function values are squared and multiplied by a penalty factor multiplier.
// The penalty factor multiplier is dynamically increasing over progressing optmization.
// 2. non-equality constraints are defined as function value is less than or equal to zero.
// neqc element is BL_2uptr_t.The 1st element is uptr_t casted BL_OBJECTIVE_FUNC.
// The 2nd element is (const void*) casted fixed parameters of the constraint function.
// In evaluating the nonequality constraints, A diode function D(f(x)) = (exp(M*f(x)) - 1.0f) is applied to the constraint
// function value.
typedef struct {
    pBL_array_t eqc; // equality constraints
    pBL_array_t neqc; // non-equality constraints
    BL_OBJECTIVE_FUNC objective;
    const void* objective_params;
    uint32_t vardim; // dimension of independent variable vector of objective and constraint functions.
} BL_OptFnParam_t, *pBL_OptFnParam_t;
typedef const BL_OptFnParam_t *pcBL_OptFnParam_t;

/*!
\brief evaluate equality constraints
\param m [in] penalty factor multiplier
\param f [in] eqaulity constraint function value
\return m * f^2
*/
BL_1r32_t BL_evaleqc(BL_1r32_t m, BL_1r32_t f);

/*!
\brief evaluate nonequality constraints
\param m [in] diode penalty factor multiplier
\param f [in] nonequality constraint function value
\return exp(m*f)-1
*/
BL_1r32_t BL_evalneqc(BL_1r32_t m, BL_1r32_t f);

typedef struct {
    BL_OptFnParam_t base; // objective and constraint functions
    BL_1r32_t meqc; // penalty factor multiplier for eqc
    BL_1r32_t mneqc; // penalty factor multiplier for neqc
    pBL_array_t xvini; // initialization vector of independent variables in base CG solver
} BL_OptConstrained_t, *pBL_OptConstrained_t;
typedef const BL_OptConstrained_t *pcBL_OptConstrained_t;

/*!
\brief create a cg solver and its parameter struct
\param problem_functions [in] objective and constraint functions and their fixed parameters
\param xvini [in] initialization vector of independent variables for base CG solver
\param xvtol [in] convergence limits of xvini in each base CG solver convergence
\param hmin [in] lower limit of linear search step
*/
int BL_OptConstranied_create(
    pcBL_OptFnParam_t problem_functions, const BL_1r32_t* xvini, BL_1r32_t xvtol, BL_1r32_t hmin,
    pBL_OptConstrained_t* ppcgparams, pBL_cg_t* ppcg);

/*!
\brief update a cg solver and its parameter struct.
cg solver is updated with a new initial vector of independent variables.
cg objective function parameter is updated with new penalty multipliers
\param pcgparams [in,out]
\param pcg [in,out]
\return ESUCCESS: optimization was converged. ERR_NO_CONVERGENCE: not yet converged.
*/
int BL_OptConstrained_update(pBL_OptConstrained_t pcgparams, pBL_cg_t pcg);

/*!
\brief objective function of CG optimizer for penalty function method
\param vardim [in] dimension of independent variable vector
\param x [in] independent variable vector
\param params [in] (const void*) casted BL_OptConstrained_t instance
\param f [out] function value
\return unix errno compatible number. Errors in any of objective and constraint functions cause this.
*/
int BL_OPtConstrained_Objective(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);
#ifdef __cplusplus
}
#endif
#endif /* BL_OPTC_H_ */
