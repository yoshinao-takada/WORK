#ifndef BL_TWOLINES_H_
#define BL_TWOLINES_H_
#include    "base_g/BL_matexport.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    BL_3r32_t   Pa, Da; // binding point and unit direction vector of line A
    BL_3r32_t   Pb, Db; // binding point and unit direction vector of line B
} BL_twolines_t, *pBL_twolines_t;
typedef const BL_twolines_t *pcBL_twolines_t;

/*!
\brief calculate two nearest point on the two lines
\param conf [in] two line configuration
\param PTa [out] the point on the line-A
\param PTb [out] the point on the line-B
\return ERR_MATRIX_SINGULAR: the two lines are almost parallel.
*/
int BL_twolines_nearest(pcBL_twolines_t conf, BL_3r32_t PTa, BL_3r32_t PTb);
#ifdef __cplusplus
}
#endif
#endif
