#ifndef BL_RATIONAL_H_
#define BL_RATIONAL_H_
#include    "base_l/BL_types.h"
#ifdef  __cplusplus
extern "C" {
#endif
/*!
\brief 有理数の実数値を求める
\param r [in] r[0]:numeratr, r[1]:denominator
\return real number r[0]/r[1]
*/
float BL_rational_rvalue(const BL_2u32_t r);

/*!
\brief 有理数を生成する
\param row [in] 
\param pos [in] 各斜め行の左下から数えた位置(pos=0,1,...row)
\param r [out] 有理数
*/
void    BL_rational_create(uint32_t row, uint32_t pos, BL_2u32_t r);

#ifdef  __cplusplus
}
#endif
#endif /* BL_RATIONAL_H_ */
