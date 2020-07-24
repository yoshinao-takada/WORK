#ifndef BL_3DLINE_H_
#define BL_3DLINE_H_
#include    "base_l/BL_types.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    BL_3r32_t dir; // normalized direction vector
    BL_3r32_t p0; // a point on the line
} BL_3Dline_t, *pBL_3Dline_t;
typedef const BL_3Dline_t *pcBL_3Dline_t;

/*!
\brief set a direction vector of line
\param line [out] a reference line
\param dir_unnormalized [in] direction vector without normalization
*/
void BL_3Dline_set_direction(pBL_3Dline_t line, const BL_3r32_t dir_unnormalized);

/*!
\brief get a foot of a normal line drawn from p to the line
\param line [in] a reference line
\param p [in] a point
\param foot [out]
*/
void BL_3Dline_normalfoot(pcBL_3Dline_t line, const BL_3r32_t p, BL_3r32_t foot);

/*!
\brief get a distance between a point and its normal foot on a reference line
\param line [in] a reference line
\param p [in] a point
\param distance [out]
*/
void BL_3Dline_distance(pcBL_3Dline_t line, const BL_3r32_t p, BL_1r32_t* distance);
#ifdef __cplusplus
}
#endif
#endif /* BL_3DLINE_H_ */
