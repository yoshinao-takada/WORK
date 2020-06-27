#include    "base_o/BL_gradient.h"
#include    "base_l/BL_containers.h"
#include    "base_l/BL_errno.h"
#include    "base_l/BL_base.h"

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