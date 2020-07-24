#define BL_DFO_C_
#include    "base_o/BL_dfo.h"
#include    "base_g/BL_matexport.h"
#include    "base_l/BL_base.h"
#include    <stdlib.h>
#include    <assert.h>

static pcBL_matfnr32RM_t matfn = NULL;

/*!
\brief evaluate objective function and count up cout_eval_OF.
\param p_ [in] BL_nelderMD_t optimizer object
*/
int EVAL_OF(pBL_nelderMD_t p_, uint32_t i)
{
    p_->count_eval_OF++;
    return p_->fn_eval(p_->vardim, p_->xf[i].xv, p_->params, &(p_->xf[i].f));
}

int EVAL_OF2(pBL_nelderMD_t p_, const BL_1r32_t* x, BL_1r32_t* pf)
{
    p_->count_eval_OF++;
    return p_->fn_eval(p_->vardim, x, p_->params, pf);
}
/*!
\brief comparison callback for qsort API
\param vf0 [in] casted pcBL_varfuncpair_t
\param vf1 [in] casted pcBL_varfuncpair_t
\return
*/
int compare_varfuncpair(const void* vf0, const void* vf1)
{
    pcBL_varfuncpair_t vf0_ = (pcBL_varfuncpair_t)vf0;
    pcBL_varfuncpair_t vf1_ = (pcBL_varfuncpair_t)vf1;
    int comparison = 0;
    if (vf0_->f > vf1_->f)
    {
        comparison = 1;
    }
    else if (vf0_->f < vf1_->f)
    {
        comparison = -1;
    }
    return comparison;
}

void sort_varfunc(pBL_nelderMD_t p)
{
    qsort((void*)(p->xf + 1), p->vardim + 1, sizeof(BL_varfuncpair_t), compare_varfuncpair);
}

/*!
\brief check if a segment satisfies the condition (vfarray->f) <= (vftarget->f) < ((vfarray+1)->f)
\param vfarray [in] a pointer pointing an element of sorted array
\param vftarget [in] search target
\return true if the target is fell in the segment
*/
bool is_matched_segment(pcBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vftarget)
{
    return (vfarray->f <= vftarget->f) && (vftarget->f < (vfarray + 1)->f);
}

/*!
\brief find a segment which contains a target
\param seg_count [in] number of segments
\param vfarray [in] sorted array
\param vftarget [in] search target
\return null if no segment satisfy the condition, else return the pointer pointing the lower
element of the segment satisfying the condition. The condition is defined by is_match_segment().
*/
pcBL_varfuncpair_t find_segment(uint32_t seg_count, pcBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vftarget)
{
    pcBL_varfuncpair_t found = (pcBL_varfuncpair_t)NULL;
    do {
        if (is_matched_segment(vfarray, vftarget))
        {
            found = vfarray;
            break;
        }
        vfarray++;
    } while (--seg_count);
    return found;
}

/*!
\brief insert a new value func pair in an existing sorted array of value func pairs.
\param element_count [in] number of elements in the sorted array
\param vfarray [in,out] the sorted array of BL_varfuncpair_t
\param vfnew [in] a new element
\param vardim [in] variable dimension
\return ESUCCESS if an adequate position is found and the new element was inserted. Otherwise ERANGE
*/
int insert(uint32_t element_count, pBL_varfuncpair_t vfarray, pBL_varfuncpair_t vfnew, uint32_t vardim)
{
    int err = ESUCCESS;
    do {
        pcBL_varfuncpair_t insert_pos = find_segment(element_count-2, vfarray, vfnew);
        if (insert_pos == NULL)
        { // there is no position to insert the new element
            err = ERANGE;
            break;
        }
        insert_pos++;
        pBL_varfuncpair_t work_pos1 = vfarray + element_count;
        pBL_varfuncpair_t work_pos = work_pos1 - 1;
        BL_varfuncpair_t discarded = { work_pos->xv, work_pos->f };
        do {
            work_pos1--;
            work_pos--;
            work_pos1->f = work_pos->f;
            work_pos1->xv = work_pos->xv;
        } while (work_pos != insert_pos);
        work_pos->f = vfnew->f;
        work_pos->xv = vfnew->xv;
        vfnew->f = discarded.f;
        vfnew->xv = discarded.xv;
    } while (0);
    return err;
}


void printf_varfuncpair(FILE* pf, pcBL_varfuncpair_t vfp, uint32_t vardim)
{
    fprintf(pf, "%f", vfp->f);
    for (uint32_t i = 0; i != vardim; i++)
    {
        fprintf(pf, ",%f", vfp->xv[i]);
    }
    fprintf(pf, "\n");
}

pBL_nelderMD_t  BL_nelderMD_alloc(uint32_t vardim)
{
    // matfn is a global var private in BL_dfo.c
    if (matfn == NULL)
    {
        matfn = BL_matfnr32RM();
    }
    uint32_t cb_head = sizeof(BL_nelderMD_t);
    uint32_t count_polytope_vertex = vardim + 2u + (uint32_t)BL_dfo_polytope_work_vertex_count;
    uint32_t count_data_element = vardim * count_polytope_vertex;
    uint32_t cb_alloc = 
        cb_head + 
        BL_ALIGN8(sizeof(BL_1r32_t) * count_data_element) + 
        BL_ALIGN8(sizeof(BL_varfuncpair_t) * count_polytope_vertex);
    return (pBL_nelderMD_t)malloc(cb_alloc);
}

pBL_nelderMD_t  BL_nelderMD_new(uint32_t vardim, const BL_1r32_t* xv0, BL_1r32_t dx, BL_1r32_t eps,
    BL_OBJECTIVE_FUNC fn_eval, const void* params)
{
    int err = ESUCCESS;
    pBL_nelderMD_t p = NULL;
    do {
        if (NULL == (p = BL_nelderMD_alloc(vardim)))
        {
            err = ENOMEM;
            break;
        }
        uint32_t count_polytope_vertex = vardim + 2;
#if defined(_DEBUG)
        p->pf = stdout;
#else
        p->pf = NULL;
#endif
        p->fn_eval = fn_eval;
        p->params = params;
        p->eps = eps;
        p->vardim = vardim;
        p->count_eval_OF = 0;
        p->xf = p->data;
        p->work = p->xf + (vardim + 2u);
        BL_1r32_t *pxv = (BL_1r32_t*)(p->work + (ptrdiff_t)BL_dfo_polytope_work_vertex_count);
        for (uint32_t i = 0; i != count_polytope_vertex; i++)
        {
            p->xf[i].xv = pxv;
            pxv += p->vardim;
        }
        for (uint32_t i = 0; i != (uint32_t)BL_dfo_polytope_work_vertex_count; i++)
        {
            p->work[i].xv = pxv;
            pxv += p->vardim;
        }
        BL_copy_array(p->xf[0].xv, xv0, p->vardim);
        p->xf[0].f = -FLT_MAX;        
        BL_copy_array(p->xf[1].xv, xv0, p->vardim);
        if (ESUCCESS != (err = EVAL_OF(p, 1)))
        {
            break;
        }
        for (uint32_t i = 2; i != count_polytope_vertex; i++)
        {
            BL_copy_array(p->xf[i].xv, p->xf[1].xv, p->vardim);
            p->xf[i].xv[i-2] += dx;
            if (ESUCCESS != (err = EVAL_OF(p, i)))
            {
                break;
            }
        }
        if (err)
        {
            break;
        }
        sort_varfunc(p);
    } while (0);
    if (err)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

// calculate the centroid of the existing vertices except the worst
void BL_nelderMD_centroid(pBL_nelderMD_t p)
{
    pBL_varfuncpair_t centroid = &(p->work[BL_dfo_polytope_centroid]);
    // clear centroid result array
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        centroid->xv[i] = 0.0f;
    }
    // calculate centroid of p->xf[*] except the worst.
    for (uint32_t i = 1; i <= p->vardim; i++)
    {
        BL_accumulate_array(centroid->xv,p->xf[i].xv,p->vardim);
    }
    BL_1r32_t normlizing_coeff = 1.0f/(BL_1r32_t)(p->vardim);
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        centroid->xv[i] *= normlizing_coeff;
    }
}

// calculate candidate vertex coordinates
void BL_nelderMD_candidates(uint32_t vardim, pcBL_varfuncpair_t worst, pBL_varfuncpair_t work_)
{
    pBL_varfuncpair_t cent_worst = &work_[BL_dfo_polytope_worst_to_centroid];
    pBL_varfuncpair_t work = &work_[BL_dfo_polytope_work];
    pcBL_varfuncpair_t centroid = &work_[BL_dfo_polytope_centroid];
    // calculate (centroid - worst)
    matfn->sub(centroid->xv, worst->xv, cent_worst->xv, vardim, 1);

    // calculate shrink position
    matfn->muls(cent_worst->xv, vardim, 1, 0.5f, work->xv);
    matfn->add(worst->xv, work->xv, work_[BL_dfo_polytope_shrink].xv, vardim, 1);

    // calculate reflection postion
    matfn->add(centroid->xv, cent_worst->xv,
        work_[BL_dfo_polytope_reflect].xv, vardim, 1);

    // calculate shrink reflection postion
    matfn->add(centroid->xv, work->xv,
        work_[BL_dfo_polytope_shrink_reflect].xv, vardim, 1);

    // calculate extend reflection position
    matfn->add(work_[BL_dfo_polytope_reflect].xv, cent_worst->xv,
        work_[BL_dfo_polytope_extend_reflect].xv, vardim , 1);
}

const char* BL_dfo_work_labels[BL_dfo_polytope_work_vertex_count] = 
{
    "BL_dfo_polytope_shrink",
    "BL_dfo_polytope_shrink_reflect",
    "BL_dfo_polytope_reflect",
    "BL_dfo_polytope_extend_reflect",
    "BL_dfo_polytope_centroid",
    "BL_dfo_polytope_worst_to_centroid",
    "BL_dfo_polytope_work"
};

// evaluate function values of candidate vertices
int BL_nelderMD_eval_candidates(pBL_nelderMD_t p)
{
    int err = ESUCCESS;
    for (uint32_t i = 0; i != (uint32_t)BL_dfo_polytope_centroid; i++)
    {
        if (ESUCCESS != (err = EVAL_OF2(p, p->work[i].xv, &(p->work[i].f))))
        {
            break;
        }
    }
    return err;
}

// find the best candidate
pBL_varfuncpair_t BL_nelderMD_find_best_candidate(pBL_varfuncpair_t work)
{
    pBL_varfuncpair_t best_candidate = NULL;
    BL_1r32_t best_func_value = FLT_MAX;
    for (uint32_t i = 0; i != (uint32_t)BL_dfo_polytope_centroid; i++)
    {
        if (best_func_value > work[i].f)
        {
            best_func_value = work[i].f;
            best_candidate = &work[i];
        }
    }
    return best_candidate;
}

// check convergence
int BL_nelderMD_converged(pBL_nelderMD_t p)
{
    int err = ESUCCESS;
    BL_1r32_t sq_eps = p->eps * p->eps;
    BL_1r32_t sq_distance;
    pcBL_varfuncpair_t centroid = &(p->work[BL_dfo_polytope_centroid]);
    pBL_varfuncpair_t work = &(p->work[BL_dfo_polytope_work]);
    for (uint32_t i = 1; i <= p->vardim; i++)
    {
        // calculate p->xf[i] - centroid
        matfn->sub(p->xf[i].xv, centroid->xv, work->xv, p->vardim, 1);
        // calculate square distance
        matfn->mul(work->xv, work->xv, &sq_distance, 1, p->vardim, 1);
        if (sq_distance > sq_eps)
        {
            err = ERR_NO_CONVERGENCE;
            break;
        }
    }
    return err;
}

static void print_xf(pcBL_nelderMD_t p, const char* caption)
{
    if (p->pf)
    {
        fprintf(p->pf, "\n%s\n", caption);
        for (uint32_t i = 0; i <= p->vardim+1; i++)
        {
            fprintf(p->pf, "p->xf[%d]: ", i);
            printf_varfuncpair(p->pf, &(p->xf[i]), p->vardim);
        }
    }
}
#if defined(_DEBUG)
#define SHOW_POLYTOPE(p, caption) { print_xf(p, caption); }
#else
#define SHOW_POLYTOPE(p, caption) { ; }
#endif

#define BL_DFO_MESSAGE(p,...) if (p->pf) { fprintf(p->pf,__VA_ARGS__); }

int BL_nelderMD_run(pBL_nelderMD_t p, uint32_t iter_max)
{
    int err = ERR_NO_CONVERGENCE;
    do {
        for (uint32_t iter_count = 0; iter_count != iter_max; iter_count++)
        {
            SHOW_POLYTOPE(p, "----------");
            // calculate the centroid of current vertices of the polytope other than the worst vertex.
            BL_nelderMD_centroid(p);

            // check convergence
            if (ESUCCESS == (err = BL_nelderMD_converged(p)))
            { // converged
                break;
            }

            // update the polytope
            BL_nelderMD_candidates(p->vardim, &p->xf[p->vardim+1], p->work); // get candidates of the new vertex
            BL_nelderMD_eval_candidates(p); // evaluate objective function at the candidates.
            pBL_varfuncpair_t best_candidate = BL_nelderMD_find_best_candidate(p->work); // select the best in the candidates.
            // insert the best candidate in the moving polyltope, keeping sorted order.
            if (ESUCCESS == (err = insert(p->vardim + 2, p->xf, best_candidate, p->vardim)))
            { // scceeded in insert() but not converged yet.
                err = ERR_NO_CONVERGENCE;
            }
            else
            { // fail in insert() and cannot converge in current setup.
                BL_DFO_MESSAGE(p, "cannot find better candidate anymore at iter=%u\n", iter_count);
                break;
            }
            BL_DFO_MESSAGE(p, "normal iteration: %u\n", iter_count);
        }
    } while(0);
    return err;
}

void BL_nelderMD_bestxf(pcBL_nelderMD_t p, pBL_varfuncpair_t bestxf)
{
    bestxf->f = p->xf[1].f;
    bestxf->xv = p->xf[1].xv;
}
