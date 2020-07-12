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
typedef struct {
    uint32_t vardim;
    BL_OBJECTIVE_FUNC main_objective;
    const void* main_objective_params;
    // constraint functions
    pBL_array_t eqc; // equality constraint functions and parameters
    pBL_array_t neqc; // nonequality constraint functions and parameters
    BL_1r32_t meqc; // penalty function multiplier for equality constraints
    BL_2r32_t mneqc; // penalty function multiplier for nonequality constraints
} BL_ConstrainedObjective_t, *pBL_ConstrainedObjective_t;
typedef const BL_ConstrainedObjective_t *pcBL_ConstrainedObjective_t;

/*!
\brief set meqc = 1.0f, mneqc = {1.0f, 1.0f}
*/
void BL_ConstrainedObjective_ResetPenaltyMultipliers(pBL_ConstrainedObjective_t p);

/*!
\brief change meqc *= 2.0f, mneqc[0] *= 2.0f, mneqc[1] /= 2.0f, n times
    for negative n, meqc /= 2.0f, mneqc[0] /= 2.0f, mneqc[1] *= 2.0f
\param p [in,out] object under operation
\param n [in] number of operation times
*/
void BL_ConstrainedObjective_ChangePenaltyMultipliers(pBL_ConstrainedObjective_t p, int32_t n);

// default of n of BL_ConstrainedObjective_ChangePenaltyMultipliers(p, n)
#define N_CHANGE        3

/*!
\brief run SUMT optimization using CG solver
\param p [in] constrained optimization problem under operation
\param pCG [in,out] CG (Conjugate Gradient) solver
\param iter_SUMT [in] maximum limit of SUMT iteration
\param iter_CG [in] maximum limit of CG search
*/
int BL_ConstrainedObjective_run(
    pBL_ConstrainedObjective_t p, pBL_cg_t pCG, uint32_t iter_SUMT, uint32_t iter_CG, BL_1r32_t* xv);
#ifdef __cplusplus
}
#endif
#endif /* BL_OPTC_H_ */
