#include    "base_o/BL_penalty.h"
#include    "base_l/BL_base.h"
pBL_optc_t  BL_optc_new(uint32_t n_eqc, uint32_t n_neqc)
{
    pBL_optc_t p = NULL;
    int err = ESUCCESS;
    do {
        if (NULL == (p = (pBL_optc_t)calloc(1, sizeof(BL_optc_t))))
        {
            err = ENOMEM;
            break;
        }
        p->n_eqc = n_eqc;
        p->n_neqc = n_neqc;
        if (NULL == (p->objectives = BL_array_new(1 + n_eqc + n_neqc, BL_3uptr_t)))
        {
            err = ENOMEM;
            break;
        }
        BL_ptr_t i_objective = BL_array_begin(p->objectives);
        for (uint32_t i = 0; i != (1 + n_eqc + n_neqc); i++)
        {
            (*i_objective._3uptr)[0] = (*i_objective._3uptr)[1] = (*i_objective._3uptr)[2] = 0;
            i_objective._3uptr++;
        }
    } while (0);
    if (err)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

int BL_optc_delete(pBL_optc_t *ppobj)
{
    int err = ESUCCESS;
    do {
        if (*ppobj)
        {
            BL_SAFEFREE(&((*ppobj)->objectives));
            BL_SAFEFREE(ppobj);
        }
        else
        {
            err = EINVAL;
        }
    } while (0);
    return err;
}

int BL_optc_get(pcBL_optc_t optc, uint32_t index, pBL_optc_element_t element)
{
    int err = ESUCCESS;
    do {
        if (index >= (1 + optc->n_eqc + optc->n_neqc))
        {
            err = ERANGE;
            break;          
        }
        BL_cptr_t i_objectives = BL_array_cbegin(optc->objectives);
        const BL_3uptr_t* pelement_ = i_objectives._3uptr + index;
        element->func = (BL_OBJECTIVE_FUNC)(*pelement_)[0];
        element->params = (const void*)(*pelement_)[1];
        BL_copy2(element->weight, (const BL_1r32_t*)(&(*pelement_)[2]));
    } while (0);
    return err;
}

int BL_optc_set(pBL_optc_t optc, uint32_t index, pcBL_optc_element_t element)
{
    int err =ESUCCESS;
    do {
        if (index >= (1 + optc->n_eqc + optc->n_neqc))
        {
            err = ERANGE;
            break;          
        }
        BL_ptr_t i_objectives = BL_array_begin(optc->objectives);
        BL_3uptr_t* pelement_ = i_objectives._3uptr + index;
        (*pelement_)[0] = (uintptr_t)element->func;
        (*pelement_)[1] = (uintptr_t)element->params;
        BL_copy2((BL_1r32_t*)(&(*pelement_)[2]), element->weight);
    } while (0);
    return err;
}

int BL_optc_eval(uint32_t vardim, const BL_1r32_t* xv, const void* optc, BL_1r32_t *pf)
{
    int err = ESUCCESS;
    *pf = 0.0f;
    pcBL_optc_t optc_ = (pcBL_optc_t)optc;
    BL_optc_element_t element;
    do {
        // intrinsic objective
        {
            float f0;
            if (ESUCCESS != (err = BL_optc_get(optc_, 0, &element)))
            {
                break;
            }
            if (ESUCCESS != (err = element.func(vardim, xv, element.params, &f0)))
            {
                break;
            }
            *pf += f0;
        }

        // equality constraints
        for (uint32_t i = 0; i != optc_->n_eqc; i++)
        {
            float f0;
            if (ESUCCESS != (err = BL_OPTC_GET_EQC(optc_, i, &element)))
            {
                break;
            }
            if (ESUCCESS != (err = element.func(vardim, xv, element.params, &f0)))
            {
                break;
            }
            *pf += (element.weight[0] * f0 * f0);
        }
        if (err) break;

        // nonequality constraints
        for (uint32_t i = 0; i != optc_->n_neqc; i++)
        {
            float f0;
            if (ESUCCESS != (err = BL_OPTC_GET_NEQC(optc_, i, &element)))
            {
                break;
            }
            if (ESUCCESS != (err = element.func(vardim, xv, element.params, &f0)))
            {
                break;
            }
            *pf += (element.weight[0] * __max(f0, 0.0f));
        }
        if (err) break;
    } while (0);
    return err;
}

void BL_optc_element_update(pBL_optc_element_t element)
{
    element->weight[0] *= element->weight[1];
}

void BL_optc_element_downdate(pBL_optc_element_t element)
{
    element->weight[0] /= element->weight[1];
}

void BL_optc_update(pBL_optc_t optc)
{
    BL_optc_element_t element;
    for (uint32_t i = 1; i < (1 + optc->n_eqc + optc->n_neqc); i++)
    {
        BL_optc_get(optc, i, &element);
        BL_optc_element_update(&element);
        BL_optc_set(optc, i, &element);
    }
}

void BL_optc_downdate(pBL_optc_t optc)
{
    BL_optc_element_t element;
    for (uint32_t i = 1; i < (1 + optc->n_eqc + optc->n_neqc); i++)
    {
        BL_optc_get(optc, i, &element);
        BL_optc_element_downdate(&element);
        BL_optc_set(optc, i, &element);
    }
}
