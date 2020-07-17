#ifndef BL_DFO_H_
#define BL_DFO_H_
#include    "base_o/BL_optimize.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\struct BL_varfuncpair_t
\brief pair of independent variables and a function value
*/
typedef struct {
    BL_1r32_t* xv;
    BL_1r32_t  f;
} BL_varfuncpair_t, *pBL_varfuncpair_t;
typedef const BL_varfuncpair_t *pcBL_varfuncpair_t;

/*!
\struct BL_nelderMD_t
\brief derivertive free multidimensional optimizer using Nelder-Mead algorithm
*/
typedef struct {
    uint32_t vardim;
    pBL_varfuncpair_t xf;
    BL_1r32_t eps;
    uint32_t  count_eval_OF; // counter counting number of evaluating objective function
    BL_OBJECTIVE_FUNC fn_eval;
    const void* params;
    BL_varfuncpair_t data[0];
} BL_nelderMD_t, *pBL_nelderMD_t;
typedef const BL_nelderMD_t *pcBL_nelderMD_t;

/*!
\brief constructor of BL_nelderMD_t object
\param vardim [in] dimension of independent variables
\param xv0 [in] initial value of the independent variables
\param eps [in] convergence decision criteria; |c - xv(farthest)| < eps is the decision condition,
where c is the centroid of all xv.
\param eval_func [in] objective function
\param params [in] fixed parameter of the objective function
\return pointer to a newly created object
*/
pBL_nelderMD_t  BL_nelderMD_new(uint32_t vardim, const BL_1r32_t* xv0, BL_1r32_t eps,
    BL_OBJECTIVE_FUNC fn_eval, const void* params);

/*!
\brief execute optimization
\param p [in,out] optimizer object
\param iter_max [in] limit of iteration
\return ERR_NO_CONVERGENCE of ESUCCESS
*/
int BL_nelderMD_run(pBL_nelderMD_t p, uint32_t iter_max);


typedef enum {
    BL_dfo_polytope_shrink,
    BL_dfo_polytope_shrink_reflect,
    BL_dfo_polytope_reflect,
    BL_dfo_polytope_extend_reflect,
    BL_dfo_polytope_centroid,
    BL_dfo_polytope_worst_to_centroid,
    BL_dfo_polytope_work,
    BL_dfo_polytope_change_count,
} BL_dfo_polytope_change_t;

typedef struct {
    BL_varfuncpair_t varfuncs[(uint32_t)BL_dfo_polytope_change_count];
    BL_1r32_t x[0];
} BL_work_varfuncpair_t, *pBL_work_varfuncpair_t;
typedef const BL_work_varfuncpair_t *pcBL_work_varfuncpair_t;

// prototypes of private functions
#ifdef _DEBUG
int compare_varfuncpair(const void* vf0, const void* vf1);
void sort_varfunc(pBL_nelderMD_t p);
bool is_matched_segment(pcBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vftarget);
pcBL_varfuncpair_t find_segment(uint32_t seg_count, pcBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vftarget);
int insert(uint32_t element_count, pBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vfnew);
pBL_nelderMD_t  BL_nelderMD_new(uint32_t vardim, const BL_1r32_t* xv0, BL_1r32_t eps,
    BL_OBJECTIVE_FUNC fn_eval, const void* params);
pBL_work_varfuncpair_t BL_work_varfuncpair_new(uint32_t vardim);
void BL_nelderMD_centroid(pcBL_nelderMD_t p, pBL_varfuncpair_t varfunc);
void BL_nelderMD_candidates(uint32_t vardim, pcBL_varfuncpair_t worst, pBL_work_varfuncpair_t work_varfuncs);
int BL_nelderMD_eval_candidates(pBL_nelderMD_t p, pBL_work_varfuncpair_t work_varfuncs);
pcBL_varfuncpair_t BL_nelderMD_find_best_candidate(pcBL_work_varfuncpair_t work_varfuncs);
int BL_nelderMD_converged(pcBL_nelderMD_t p, pBL_work_varfuncpair_t work_varfunc);
#endif
#ifdef __cplusplus
}
#endif
#endif /* BL_DFO_H_ */