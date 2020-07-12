#ifndef BL_RATIONALFUNC_H_
#define BL_RATIONALFUNC_H_
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region single_variable_calculation
/*!
\brief 1-D polynomial value with real coefficients and a real variable
\param coeff [in] coefficients
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_polynomial_rvalue(pcBL_array_t coeff, BL_1r32_t x, BL_1r32_t* y);

/*!
\brief 1-D polynomial value with complex coefficients and a complex variable
\param coeff [in] coefficients
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_polynomial_cvalue(pcBL_array_t coeff, BL_1c64_t x, BL_1c64_t* y);

/*!
\brief 1-D rational func value with real coefficients and a real variable
\param coeff [in] coefficients; 1st node: numerator, 2nd node: denominator
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_rationalfunc_rvalue(pcBL_linkable_array_t coeff, BL_1r32_t x, BL_1r32_t* y);


/*!
\brief 1-D rational func value with complex coefficients and a complex variable
\param coeff [in] coefficients; 1st node: numerator, 2nd node: denominator
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_rationalfunc_cvalue(pcBL_linkable_array_t coeff, BL_1c64_t x, BL_1c64_t* y);


/*!
\brief multi-dimensional rational func value with real coefficients and a real variable
\param coeff [in] coefficients; 1st node: numerator, 2nd node: denominator
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_rationalfunc_rvalueMD(pcBL_linkable_array_t coeff, BL_1r32_t x, BL_1r32_t* y);


/*!
\brief multi-dimensional rational func value with complex coefficients and a complex variable
\param coeff [in] coefficients; 1st node: numerator, 2nd node: denominator
\param x [in] independent variable
\param y [in] depedent variable
\return unix errno compatible number
*/
int BL_rationalfunc_cvalueMD(pcBL_linkable_array_t coeff, BL_1c64_t x, BL_1c64_t* y);
#pragma endregion
#pragma region mass_variable_calculation
/*!
\brief 1-D polynomial with multiple variables
\param coeff [in] polynomial coefficients
\param xdim [in] array size of x
\param x [in] independent variables
\param y [out] polynomial values for x
\return unix errno compatible number
*/
int BL_polynomial_rvaluev(pcBL_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y);
int BL_polynomial_cvaluev(pcBL_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y);

int BL_rationalfunc_rvaluev(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y);
int BL_rationalfunc_cvaluev(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y);

int BL_rationalfunc_rvalueMDv(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1r32_t* x, BL_1r32_t* y);
int BL_rationalfunc_cvalueMDv(pcBL_linkable_array_t coeff, uint32_t xdim, const BL_1c64_t* x, BL_1c64_t* y);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif /* BL_RATIONALFUNC_H_ */
