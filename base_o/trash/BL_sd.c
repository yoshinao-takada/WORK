#include    "base_o/BL_sd.h"
#include    <errno.h>
#include    <base_g/BL_matexport.h>

int BL_sd_iterate(const BL_1r32_t *x, BL_1r32_t *x_next, uint32_t n, const void* params, BL_COST_FOR_OPTIMIZATION func)
{
    int err = ESUCCESS;
    BL_1r32_t *r = NULL;
    do {

    } while (0);
    return err;
}

int BL_sd_optimize(const BL_1r32_t *x, uint32_t n, const void* params, BL_COST_FOR_OPTIMIZATION func)
{
    return EINVAL;
}

float BL_dd1(const BL_2r32_t xy0, const BL_2r32_t xy1) 
{
    return (xy1[1] - xy0[1])/(xy1[0] - xy0[0]);
}

float BL_dd2(const BL_2r32_t xy0, const BL_2r32_t xy1, const BL_2r32_t xy2)
{
    return (BL_dd1(xy1, xy2) - BL_dd1(xy0, xy1)) / (xy2[0] - xy0[0]);
}

float BL_lambda_m(const BL_2r32_t xy0, const BL_2r32_t xy1, const BL_2r32_t xy2)
{
    float dd2 = BL_dd2(xy0, xy1, xy2);
    return (dd2 * (xy0[0] + xy0[1]) - BL_dd1(xy0, xy1))/(2.0f * dd2);
}

static int BL_alloc_buffers(uint32_t n, BL_1r32_t** ppx)
{
    int err = ESUCCESS;
    do {
        if (NULL == (*ppx = (BL_1r32_t*)calloc(n, sizeof(BL_1r32_t))))
        {
            err = ENOMEM;
            break;
        }
    } while (0);
    return err;
}

#define BL_IF_ALLOC_ERR_BREAK(e,n,ppx) if (ESUCCESS != (e = BL_alloc_buffers(n, ppx))) { break; }
#define BL_IF_NOTNULL_FREE(ppx) if (*ppx) { free((viod*)(*ppx)); *ppx = NULL; }
static inline void BL_swap_ptr(BL_1r32_t** pp0, BL_1r32_t** pp1)
{
    BL_1r32_t *ptemp = *pp0;
    *pp0 = *pp1;
    *pp1 = ptemp;
}

static inline void BL_swap_val(BL_1r32_t* p0, BL_1r32_t* p1)
{
    BL_1r32_t ftemp = *p0;
    *p0 = *p1;
    *p1 = ftemp;
}

static inline void BL_swap_ptr_and_val(BL_1r32_t** pp0, BL_1r32_t* p0, BL_1r32_t** pp1, BL_1r32_t* p1)
{
    BL_swap_ptr(pp0, pp1);
    BL_swap_val(p0, p1);
}

int BL_powell_interp(
        BL_COST_FOR_OPTIMIZATION fc,
        const BL_1r32_t* vdir,
        const BL_1r32_t* x0,
        uint32_t n,
        const void* params,
        float hmin, float hmax,
        uint32_t iter_max);
{
    int err = ESUCCESS;
    pcBL_matfnr32RM_t fnmat = BL_matfnr32RM();
    BL_1r32_t *hD, *_2hD, *lambda0, *lambda1, *lambda2, *lambda_m;
    BL_1r32_t f0, f1, f2, f_m;
    BL_2r32_t hf012[3];
    uint32_t im = 0;
    do {
        // alloc working arrays
        BL_IF_ALLOC_ERR_BREAK(err,n,&hD);
        BL_IF_ALLOC_ERR_BREAK(err,n,&_2hD);
        BL_IF_ALLOC_ERR_BREAK(err,n,&lambda0);
        BL_IF_ALLOC_ERR_BREAK(err,n,&lambda1);
        BL_IF_ALLOC_ERR_BREAK(err,n,&lambda2);
        BL_IF_ALLOC_ERR_BREAK(err,n,&lambda_m);
        fnmat->muls(vdir,n,1,hmin,hD);
        fnmat->muls(vdir,n,1,(2*hmin),_2hD);
        // Step 1
        BL_copy_array(lambda0, x0, n);
        if (ESUCCESS != (err = fn(&f0,lambda0, n, params))) break;
        fnmat->add(x0,hD,lambda1,n,1);
        if (ESUCCESS != (err = fn(&f1, lambda1, n, params))) break;
        // Step 2
        if (f0 < f1)
        {
            fnmat->sub(x0,hD,lambda2,n,1);
            if (ESUCCESS != (err = fn(&f2, lambda2, n, params))) break;
            // reorder on the search line in vdir direction
            BL_swap_ptr_and_val(&lambda1, &f1, &lambda2, &f2);
            BL_swap_ptr_and_val(&lambda0, &f0, &lambda1, &f1);
            im = 1;
        }
        else
        {
            fnmat->add(x0,_2hD,lambda2, n, 1);
            if (ESUCCESS != (err = fn(&f2, lambda2, n, params))) break;
            im = 0;
        }
        
        for (uint32_t iter = 0; iter != iter_max; iter++)
        {
            // Step 3
            hf012[0][0] = -hmin; hf012[0][1] = f0;
            hf012[1][0] = 0.0f; hf012[1][1] = f1;
            hf012[2][0] = hmin; hf012[2][1] = f2;
            // Step 4
            // Step 5
            // Step 6
        }
    } while (0);
    return err;
}
