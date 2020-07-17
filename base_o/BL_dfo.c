#include    "base_o/BL_dfo.h"
#include    "base_g/BL_matexport.h"
#include    <stdlib.h>

pcBL_matfnr32RM_t matfn = NULL;

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
    qsort((void*)p->xf, p->vardim + 1, sizeof(BL_varfuncpair_t), compare_varfuncpair);
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
\return ESUCCESS if an adequate position is found and the new element was inserted. Otherwise ERANGE
*/
int insert(uint32_t element_count, pBL_varfuncpair_t vfarray, pcBL_varfuncpair_t vfnew)
{
    int err = ERANGE;
    do {
        pcBL_varfuncpair_t found_seg = find_segment(element_count - 1, vfarray, vfnew);
        if (!found_seg)
        {
            break;
        }
        found_seg++;
        pBL_varfuncpair_t position_under_operation = vfarray + element_count - 1;
        while (position_under_operation != found_seg)
        {
            pBL_varfuncpair_t pos_next = position_under_operation - 1;
            position_under_operation->f = pos_next->f;
            position_under_operation->xv = pos_next->xv;
            position_under_operation = pos_next;
        }
        position_under_operation--;
        position_under_operation->f = vfnew->f;
        position_under_operation->xv = vfnew->xv;
    } while (0);
    return err;
}

pBL_nelderMD_t  BL_nelderMD_new(uint32_t vardim, const BL_1r32_t* xv0, BL_1r32_t eps,
    BL_OBJECTIVE_FUNC fn_eval, const void* params)
{
    int err = ESUCCESS;
    pBL_nelderMD_t p = NULL;
    if (matfn == NULL)
    {
        matfn = BL_matfnr32RM();
    }
    do {
        uint32_t polytope_vertex_count = vardim + 1;
        uint32_t cb_head = sizeof(BL_nelderMD_t);
        uint32_t count_polytope_vertex = vardim + 1;
        uint32_t count_data_element = vardim * count_polytope_vertex;
        uint32_t cb_alloc = cb_head + sizeof(BL_1r32_t) * count_data_element + sizeof(BL_varfuncpair_t) * count_polytope_vertex;
        if (NULL == (p = (pBL_nelderMD_t)malloc(cb_alloc)))
        {
            err = ENOMEM;
            break;
        }
        p->fn_eval = fn_eval;
        p->params = params;
        p->eps = eps;
        p->vardim = vardim;
        p->xf = p->data;
        BL_1r32_t *pxv = (BL_1r32_t*)(p->xf + count_polytope_vertex);
        for (uint32_t i = 0; i != count_polytope_vertex; i++)
        {
            p->xf[i].xv = pxv + i * vardim;
        }
        BL_copy_array(p->xf[0].xv, xv0, vardim);
        if (ESUCCESS != (err = EVAL_OF(p, 0)))
        {
            break;
        }
        for (uint32_t i = 1; i != count_polytope_vertex; i++)
        {
            BL_copy_array(p->xf[i].xv, p->xf[0].xv, vardim);
            p->xf[i].xv[i-1] += 1.0f;
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

pBL_work_varfuncpair_t BL_work_varfuncpair_new(uint32_t vardim)
{
    int err = ESUCCESS;
    pBL_work_varfuncpair_t p = NULL;
    do {
        size_t cb_x = BL_dfo_polytope_change_count * vardim * sizeof(BL_1r32_t);
        size_t cb_alloc = cb_x + sizeof(BL_work_varfuncpair_t);
        p = (pBL_work_varfuncpair_t)malloc(cb_alloc);
        p->varfuncs[0].xv = p->x;
        for (uint32_t i = 1; i != (uint32_t)BL_dfo_polytope_change_count; i++)
        {
            p->varfuncs[i].xv = p->varfuncs[i-1].xv + vardim;
        }
    } while (0);
    if (err)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

// calculate the centroid of the existing vertices except the worst
void BL_nelderMD_centroid(pcBL_nelderMD_t p, pBL_varfuncpair_t varfunc)
{
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        varfunc->xv[i] = 0.0f;
    }
    // TODO: calculate centroid of p->xf[*] except the worst.
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        BL_accumulate_array(varfunc->xv,p->xf[i].xv,p->vardim);
    }
    BL_1r32_t normlizing_coeff = 1.0f/(BL_1r32_t)(p->vardim);
    for (uint32_t i = 0; i != p->vardim; i++)
    {
        varfunc->xv[i] *= normlizing_coeff;
    }
}

// calculate candidate vertex coordinates
void BL_nelderMD_candidates(uint32_t vardim, pcBL_varfuncpair_t worst, pBL_work_varfuncpair_t work_varfuncs)
{
    pBL_varfuncpair_t cent_worst = &work_varfuncs->varfuncs[BL_dfo_polytope_worst_to_centroid];
    pBL_varfuncpair_t work = &work_varfuncs->varfuncs[BL_dfo_polytope_work];
    // calculate (centroid - worst)
    matfn->sub(worst->xv, work_varfuncs->varfuncs[BL_dfo_polytope_centroid].xv, cent_worst->xv, vardim, 1);

    // calculate shrink position
    matfn->muls(cent_worst->xv, vardim, 1, 0.5f, work->xv);
    matfn->add(worst->xv, work->xv, work_varfuncs->varfuncs[BL_dfo_polytope_shrink].xv, vardim, 1);

    // calculate reflection postion
    matfn->add(work_varfuncs->varfuncs[BL_dfo_polytope_centroid].xv, cent_worst->xv,
        work_varfuncs->varfuncs[BL_dfo_polytope_reflect].xv, vardim, 1);

    // calculate shrink reflection postion
    matfn->add(work_varfuncs->varfuncs[BL_dfo_polytope_centroid].xv, work->xv,
        work_varfuncs->varfuncs[BL_dfo_polytope_shrink_reflect].xv, vardim, 1);

    // calculate extend reflection position
    matfn->add(work_varfuncs->varfuncs[BL_dfo_polytope_reflect].xv, cent_worst->xv,
        work_varfuncs->varfuncs[BL_dfo_polytope_extend_reflect].xv, vardim , 1);
}

// evaluate function values of candidate vertices
int BL_nelderMD_eval_candidates(pBL_nelderMD_t p, pBL_work_varfuncpair_t work_varfuncs)
{
    int err = ESUCCESS;
    for (uint32_t i = 0; i != (uint32_t)BL_dfo_polytope_centroid; i++)
    {
        if (ESUCCESS != (err = EVAL_OF2(p, work_varfuncs->varfuncs[i].xv, &work_varfuncs->varfuncs[i].f)))
        {
            break;
        }
    }
    return err;
}

// find the best candidate
pcBL_varfuncpair_t BL_nelderMD_find_best_candidate(pcBL_work_varfuncpair_t work_varfuncs)
{
    pcBL_varfuncpair_t best_candidate = NULL;
    BL_1r32_t best_func_value = FLT_MAX;
    for (uint32_t i = 0; i != (uint32_t)BL_dfo_polytope_centroid; i++)
    {
        if (best_func_value > work_varfuncs->varfuncs[i].f)
        {
            best_func_value = work_varfuncs->varfuncs[i].f;
            best_candidate = &work_varfuncs->varfuncs[i];
        }
    }
    return best_candidate;
}

// check convergence
int BL_nelderMD_converged(pcBL_nelderMD_t p, pBL_work_varfuncpair_t work_varfunc)
{
    int err = ESUCCESS;
    BL_1r32_t sq_eps = p->eps;
    BL_1r32_t sq_distance;
    pcBL_varfuncpair_t centroid = &work_varfunc->varfuncs[BL_dfo_polytope_centroid];
    pBL_varfuncpair_t work = &work_varfunc->varfuncs[BL_dfo_polytope_work];
    for (uint32_t i = 0; i != p->vardim; i++)
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

int BL_nelderMD_run(pBL_nelderMD_t p, uint32_t iter_max)
{
    int err = ERR_NO_CONVERGENCE;
    pBL_work_varfuncpair_t work_varfuncs = NULL;
    do {
        if (NULL == (work_varfuncs = BL_work_varfuncpair_new(p->vardim)))
        {
            err = ENOMEM;
            break;
        }
        do {
            // calculate the centroid of current vertices of the polytope other than the worst vertex.
            BL_nelderMD_centroid(p, &work_varfuncs->varfuncs[BL_dfo_polytope_centroid]);

            // check convergence
            if (ESUCCESS == (err = BL_nelderMD_converged(p, work_varfuncs)))
            { // converged
                break;
            }

            // update the polytope
            BL_nelderMD_candidates(p->vardim, &p->xf[p->vardim], work_varfuncs);
            BL_nelderMD_eval_candidates(p, work_varfuncs);
            pcBL_varfuncpair_t best_candidate = BL_nelderMD_find_best_candidate(work_varfuncs);
            if (ESUCCESS != (err = insert(p->vardim, p->xf, best_candidate)))
            { // fail in inserting the new vertex
                break;
            }
        } while (--iter_max);
    } while(0);
    BL_SAFEFREE(&work_varfuncs);
    return err;
}