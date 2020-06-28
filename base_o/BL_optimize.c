#include    "base_o/BL_optimize.h"
#include    "base_l/BL_errno.h"
#include    "base_l/BL_base.h"
#include    <stdlib.h>

BL_1r32_t   BL_dd1(const BL_2r32_t xy0, const BL_2r32_t xy1)
{
    return (xy1[1] - xy0[1])/(xy1[0] - xy0[0]);
}

BL_1r32_t   BL_dd2(const BL_2r32_t xy0, const BL_2r32_t xy1,  const BL_2r32_t xy2)
{
    return (BL_dd1(xy1, xy2) - BL_dd1(xy0, xy1))/(xy2[0] - xy0[0]);
}


int BL_gradientv(
    uint32_t vardim, 
    const BL_1r32_t* xv, 
    const void* params,
    BL_OBJECTIVE_FUNC func,
    BL_1r32_t* deltaxv, 
    BL_1r32_t* dfdx)
{
    int err = ESUCCESS;
    BL_1r32_t *xv_work = NULL;
    do {
        if (NULL == (xv_work = (BL_1r32_t*)malloc(sizeof(BL_1r32_t) * vardim)))
        {
            err = ENOMEM;
            break;
        }
        for (uint32_t i = 0; i != vardim; i++)
        {
            BL_2r32_t xf[2] = {{deltaxv[i], 0.0f}, {-deltaxv[i], 0.0f}};
            BL_copy_array(xv_work, xv, vardim);
            xv_work[i] += deltaxv[i];
            if (ESUCCESS !=(err = func(vardim, xv_work, params, &xf[0][1])))
            {
                break;
            }
            xv_work[i] -= 2.0f * deltaxv[i];
            if (ESUCCESS !=(err = func(vardim, xv_work, params, &xf[1][1])))
            {
                break;
            }
            dfdx[i] = BL_dd1(xf[0], xf[1]);
        }
    } while (0);
    BL_SAFEFREE(&xv_work);
    return err;
}

pBL_2vecq_t BL_2vecq_new(uint32_t vardim)
{
    size_t cb_block = sizeof(BL_1r32_t) * vardim;
    size_t cb_data = cb_block * 2;
    size_t cb_alloc = sizeof(BL_2vecq_t) + cb_data;
    pBL_2vecq_t p = (pBL_2vecq_t)malloc(cb_alloc);
    if (p)
    {
        p->v[0] = p->buf;
        p->v[1] = p->v[0] + vardim;
    }
    return p;
}

BL_1r32_t* BL_2vecq_peek(pBL_2vecq_t p)
{
    return p->v[1];
}

void    BL_2vecq_push(pBL_2vecq_t p, BL_1r32_t* v)
{
    p->v[1] = p->v[0];
    p->v[0] = v;
}