#ifndef BL_DEBUTIL_H_
#define BL_DEBUTIL_H_
#include    "base_l/BL_containers.h"
#include    <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief write an element pointed by ptr and increment it.
\param pf [out] output stream
\param ptr [in,out] generic pointer pointing an element of an array
\param first_delimiter [in] "" if the element is the first in a row, otherwise ",".
*/
typedef void (*BL_DEBUTIL_FWRITE_ELEMENT)(FILE* pf, BL_cptr_t* ptr);

/*!
\brief write an array in .csv file.
\param pf [in] output stream
\param array [in] an array to write.
\param header [in] CSV header string excluding new line character or characters
\param element_writer [in] a function to render an element in array.
*/
void BL_debutil_fwrite_array2D(FILE* pf, pcBL_arrayMD_t array, const char* header, BL_DEBUTIL_FWRITE_ELEMENT element_writer);

/*!
\brief element writer for BL_2r32_t element.
*/
void BL_debutil_write_2r32(FILE* pf, BL_cptr_t* ptr);

/*!
\brief element writer for BL_1u16_t element
*/
void BL_debutil_write_1u16(FILE* pf, BL_cptr_t* ptr);

/*!
\brief element writer for BL_1u32_t element
*/
void BL_debutil_write_1u32(FILE* pf, BL_cptr_t* ptr);

/*!
\brief write an array in .csv file.
\param pf [in] output stream
\param array [in] an array to write.
\param header [in] csv header string excluding new line character or characters
\param element_writer [in] a function to render an element in array
*/
void BL_debutil_fwrite_array(FILE* pf, pcBL_array_t array, const char* header, BL_DEBUTIL_FWRITE_ELEMENT  element_writer);
#ifdef __cplusplus
}
#endif
#endif