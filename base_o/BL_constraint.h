#ifndef BL_CONSTRAINT_H_
#define BL_CONSTRAINT_H_
/*
type1: Equality constraint (== 0.0) optmization solver
type2: Non-equality constraint (>= 0.0) and equality constraint (== 0.0) optimization solver
*/
#include    "base_l/BL_containers.h"
#include    "base_l/BL_types.h"
#include    "base_o/BL_optimize.h"

typedef struct {
    // constraint functions and their fixed parameters
    pBL_array_t eqc;
    pBL_array_t neqc;

    // intrinsic objective function and its fixed parameters
    BL_OBJECTIVE_FUNC fn_objective;
    const void* params_objective;

    // vardim_objective holds number of variables of objective function.
    // Lagrangian vardim is equal to vardim_objective + number of equality constraints.
    // Augmented Lagrangian vardim is equal to vardim_objective + number of eqality and nonequality constraints.
    uint32_t vardim_objective;
    uint32_t eqc_dim, neqc_dim; // number of equality constraints and nonequality constraints
    uint32_t vardim; // total vardim
} BL_constraint_t, *pBL_constraint_t;
typedef const BL_constraint_t *pcBL_constraint_t;

/*!
\brief create a set of functions, 1) intrinsic objective function, 2) equliaty constraints, 3) nonequality
constraints.
\param vardim [in] dimension of independent variable vector
\param hmin [in] lower limit of line-search differentiation step
\param xv0 [in] initial value of independent variable vector
\param fn_eval [in] objective function of minimization
\param params [in] fixed parameters of fn_eval
\param eqc [in] array of equality constraint functions and fixed parameters
\param neqc [in] array of nonequality constraint functions and fixed parameters
*/
pBL_constraint_t BL_constraint_new(
    pBL_array_t eqc,
    pBL_array_t neqc,
    uint32_t vardim_objective,
    BL_OBJECTIVE_FUNC fn_objective,
    const void* params_objective);

#endif /* BL_CONSTRAINT_H_ */