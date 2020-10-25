#ifndef BL_SNPARRAYIO2_H_
#define BL_SNPARRAYIO2_H_
/*!
\file BL_snparrayIO2.h
\brief CSV reader/writer for circuit matrix array
*/
#include    "base_z/BL_snparray.h"
#include    <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief save SnP array in a .csv file in real-imaginary format
\param pf [in,out] FILE pointer
\param a [in] 
*/
int BL_snparrayIO2_writeRI(FILE* pf, pcBL_snparray_t a);
/*!
\brief save SnP array in a .csv file in magnitude-angle(degree) format
\param pf [in,out] FILE pointer
\param a [in] 
*/
int BL_snparrayIO2_writeMA(FILE* pf, pcBL_snparray_t a);
/*!
\brief save SnP array in a .csv file in decibel-angle(degree) format
\param pf [in,out] FILE pointer
\param a [in] 
*/
int BL_snparrayIO2_writeDA(FILE* pf, pcBL_snparray_t a);
#ifdef __cplusplus
}
#endif
#endif /* BL_SNPARRAYIO2_H_ */