#ifndef BL_PENALTY_H_
#define BL_PENALTY_H_
#include    "base_o/BL_dfo.h"
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief constrained optimization problem definitions
*/
typedef struct {
    uint32_t n_eqc, n_neqc; // numbers of equality constraints and of non-equality constraints

    // objective and constraint functions are stored in BL_array_t.
    // objective and constraint functions are casted to uintptr_t
    // their fixed parameters are also casted to uintptr_t
    // k is a weighting coefficient in calculating a penalty function as a weighted sum of the
    // objective and constraint functions.
    pBL_array_t objectives; // array of uintptr_t[3] = { BL_OBJECTIVE_FUNC fn, const void* params, float k }
} BL_optc_t, *pBL_optc_t;
typedef const BL_optc_t *pcBL_optc_t;

/*!
\brief create an object of a constrained optimization problem
\param n_eqc [in] number of equality constraints
\param n_neqc [in] number of non-equality constraints
\return pointer to the object
*/
pBL_optc_t  BL_optc_new(uint32_t n_eqc, uint32_t n_neqc);

/*!
\brief delete an object of a constrained optimizatin problem
*/
int BL_optc_delete(pBL_optc_t *ppobj);

typedef struct {
    BL_OBJECTIVE_FUNC func;
    const void* params;
    BL_2r32_t weight;
} BL_optc_element_t, *pBL_optc_element_t;
typedef const BL_optc_element_t *pcBL_optc_element_t;

/*!
\brief update penalty weight coefficient
\param element [in,out]
*/
void BL_optc_element_update(pBL_optc_element_t element);

/*!
\brief downdate penalty weight coefficient
\param element [in,out]
*/
void BL_optc_element_downdate(pBL_optc_element_t element);

/*!
\brief get an element of optc->objectives as a usable form
\param optc [in] constrained optimization problem object
\param index [in] index for accessing optc->objectives
\param element [out] acquired contents of optc->objectives
\return ERANGE if index is out of range
*/
int BL_optc_get(pcBL_optc_t optc, uint32_t index, pBL_optc_element_t element);

/*!
\brief set an element of optc->objectives
\param optc [in,out] constrained optimization problem object
\param index [in] index for accessing optc->objectives
\param element [in] contents of optc->objectives to insert
\return ERANGE if index is out of range
*/
int BL_optc_set(pBL_optc_t optc, uint32_t index, pcBL_optc_element_t element);

#define BL_OPTC_GET_EQC(optc_,eqc_index_,element_)    BL_optc_get((optc_), (1+(eqc_index_)), (element_))
#define BL_OPTC_GET_NEQC(optc_,neqc_index_,element_)  BL_optc_get((optc_), (1+((optc_)->n_eqc)+(neqc_index_)), (element_))
#define BL_OPTC_SET_EQC(optc_,eqc_index_,element_)    BL_optc_set((optc_), (1+(eqc_index_)), (element_))
#define BL_OPTC_SET_NEQC(optc_,neqc_index_,element_)  BL_optc_set((optc_), (1+((optc_)->n_eqc)+(neqc_index_)), (element_))

/*!
\brief weighted sum of an intrinsic objective, equality constraints, and non-equality constraints which
are contained by optc. optc is casted from pcBL_optc_t.
*/
int BL_optc_eval(uint32_t vardim, const BL_1r32_t* xv, const void* optc, BL_1r32_t *pf);

/*!
\brief update weight coefficients on constraint functions
\param optc [in,out] constrained optimization problem object
*/
void BL_optc_update(pBL_optc_t optc);


/*!
\brief downdate weight coefficients on constraint functions
\param optc [in,out] constrained optimization problem object
*/
void BL_optc_downdate(pBL_optc_t optc);

#ifdef __cplusplus
}
#endif
#endif /* BL_PENALTY_H_ */