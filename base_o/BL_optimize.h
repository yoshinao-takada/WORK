#ifndef BL_OPTIMIZE_H_
#define BL_OPTIMIZE_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_g/BL_matexport.h"
#include    "base_l/BL_futils.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief objective function of optimization (usually minimization)
\param vardim [in] dimension of x[]
\param x [in] vector of independent variables
\param params [in] fixed parameters
\param f [out] scalar function value
\return result code (ESUCCESS: success in function evaluation, others: something bad in function evaluation)
*/
typedef int (*BL_OBJECTIVE_FUNC)(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);

typedef BL_OBJECTIVE_FUNC   BL_CONSTRAINTEQ_FUNC; // equality constraint function
typedef BL_OBJECTIVE_FUNC   BL_CONSTRAINTGE_FUNC; // greater-than-or-equal-to constraint function

/*!
\brief 1st and 2nd order divided difference
https://en.wikipedia.org/wiki/Divided_differences
*/
BL_1r32_t   BL_dd1(const BL_2r32_t xy0, const BL_2r32_t xy1);
BL_1r32_t   BL_dd2(const BL_2r32_t xy0, const BL_2r32_t xy1,  const BL_2r32_t xy2);


/*!
\brief gradient of func
\param vardim [in] dimension of independent varialbles
\param xv [in] indipendent variable vector
\param params [in] fixed parameters
\param func [in] function
\param deltav [in] difference of xv in numeric differentiation
\param dfdx [out] resutled gradient vector
*/
int BL_gradientv(
    uint32_t vardim, 
    const BL_1r32_t* xv, 
    const void* params,
    BL_OBJECTIVE_FUNC func,
    BL_1r32_t* deltaxv, 
    BL_1r32_t* dfdx);

typedef struct 
{
    BL_1r32_t* v[2];
    BL_1r32_t buf[0];
} BL_2vecq_t, *pBL_2vecq_t;

typedef const BL_2vecq_t *pcBL_2vecq_t;

/*!
\brief create a two element queue of BL_1r32_t vectors
\param vardim [in] dimension of the element vector
\return pointer to the created object
*/
pBL_2vecq_t BL_2vecq_new(uint32_t vardim);

/*!
\brief get p->v[1]
\param p [in] a pointer to an object
*/
BL_1r32_t* BL_2vecq_peek(pBL_2vecq_t p);

/*!
\brief Please follow the usage below
Step 1: BL_1r32_t* v = BL_2vecq_peek(p);
Step 2: Some_operation(v);
Step 3: BL_2vecq_push(p, v); // *p is updated as p->v[1] = p->v[0]; p->v[0] = v;
\param p [in,out] a pointer to an object
\param v [in] vector to push into the queue
*/
void    BL_2vecq_push(pBL_2vecq_t p, BL_1r32_t* v);
#ifdef __cplusplus
}
#endif
#endif
