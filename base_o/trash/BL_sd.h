#ifndef BL_SD_H_
#define BL_SD_H_
#include    <base_g/BL_matexport.h>
#ifdef  __cplusplus
extern "C" {
#endif
    /*!
    \brief cost function for optimization
    \param f [out] function value (scalar value)
    \param x [in] variables
    \param n [in] dimension of x
    \param params [in] fixed parameters (optional)
    \return error code
    */
    typedef int (*BL_COST_FOR_OPTIMIZATION)(BL_1r32_t* f, const BL_1r32_t *x, uint32_t n, const void* params);

    /*!
    \brief a gradient of a function of a type of BL_COST_FOR_OPTMIZATION
    \param dfdx [out] function value (n-dimensional array)
    \param x [in] variables
    \param n [in] dimension of x
    \param params [in] fixed parameters (optional)
    \return error code
    */
    typedef int (*BL_COST_GRADIENT)(BL_1r32_t* dfdx, const BL_1r32_t *x, uint32_t n, const void* params);

    /*!
    \brief one iteration of BL_sd_optimize()
    \param x [in] variables of func
    \param x_next [out] variables of func for next iteration
    \param n [in] dimension of x 
    \param params [in] fixed parameters of func (optional)
    \param func [in] 
    */
    int BL_sd_iterate(const BL_1r32_t *x, BL_1r32_t *x_next, uint32_t n, const void* params, BL_COST_FOR_OPTIMIZATION func);


    /*!
    \brief one iteration of BL_sd_optimize()
    \param x [in] variables of func
    \param x_opt [out] optimized variables of func after convergence
    \param n [in] dimension of x 
    \param params [in] fixed parameters of func (optional)
    \param func [in] 
    */
    int BL_sd_optimize(const BL_1r32_t *x, BL_1r32_t *x_next, uint32_t n, const void* params, BL_COST_FOR_OPTIMIZATION func);

    /*!
    \brief 1st order divided difference
    \param xy0 [in] 1st point
    \param xy1 [in] 2nd point
    \return the computed value of the divided difference
    */
    float BL_dd1(const BL_2r32_t xy0, const BL_2r32_t xy1);

    float BL_dd2(const BL_2r32_t xy0, const BL_2r32_t xy1, const BL_2r32_t xy2);

    float BL_lambda_m(const BL_2r32_t xy0, const BL_2r32_t xy1, const BL_2r32_t xy2);
    
    /*!
    \brief Powell's quadratic interpolation
    \param fc [in] cost function to minimize
    \param vdir [in] unit direction vector of the search line
    \param x0 [in] initial point of the search
    \param n [in] dimension of vdir and x0
    \param params [in] fixed parameters of func (optional)
    \param hmin [in] minimum step size
    \param hmax [in] maximum step size
    \param iter_max [in] iteration limit
    \return error code
    */
    int BL_powell_interp(
        BL_COST_FOR_OPTIMIZATION fc,
        const BL_1r32_t* vdir,
        const BL_1r32_t* x0,
        uint32_t n,
        const void* params,
        float hmin, float hmax,
        uint32_t iter_max);
#ifdef  __cplusplus
}
#endif
#endif
