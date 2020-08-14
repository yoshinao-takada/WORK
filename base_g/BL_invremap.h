#ifndef BL_INVREMAP_H_
#define BL_INVREMAP_H_
#include    "base_l/BL_containers.h"
#include    <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#pragma region key_data_structs
/*!
\struct BL_source_shift_map_t
\brief disparity map defined on the source image pixels
*/
typedef struct {
    BL_4u32_t roi; // reliable rectangle region of the map
    BL_2u32_t work_size; // working 2D array size
    BL_2r32_t shift[0]; // 2D shift value from the source pixel coord to the dest pixel coord
} BL_source_shift_map_t, *pBL_source_shift_map_t;
typedef const BL_source_shift_map_t *pcBL_source_shift_map_t;

#include    "base_g/BL_invremap_testgen.h" /* test pattern generator */

/*!
\struct BL_remap_t
\brief opencv2 cv::remap() compatible remapping table.
*/
typedef struct {
    BL_2u32_t work_size;
    BL_2r32_t *source_coord; // pixel coordinates of the source image
    BL_1u8_t *rel; // reliability table, 0: reliable, 1: unreliable
    BL_1u8_t data[0]; // dummy accessor to the beginning of the extra allocated memory block
} BL_remap_t, *pBL_remap_t;
typedef const BL_remap_t *pcBL_remap_t;
#pragma endregion

/*!
\brief create an object of BL_remap_t
\param work_size [in]
\param pprema [out]
\return ESUCCESS or ENOMEM
*/
int BL_remap_new(const BL_2u32_t work_size, pBL_remap_t* ppremap);

/*!
\brief create an inverted-remap-table, which maps a source image to a destination image.
\param ssmap [in] source shift map
\param ppremap [out] resulted remap table
*/
int BL_invremap_create(pcBL_source_shift_map_t ssmap, pBL_remap_t *ppremap);

#pragma region fake_public_method_only_for_debugging
typedef struct {
    BL_2u32_t neighbor_sources[4]; // source pixel indices of neighbor square
    uint32_t flags; // bit0..3:neighbor_sources[0..3] is filled and bit0..3 is set, bit4..31:reserved
} BL_neighbormap_element_t, *pBL_neighbormap_element_t;
typedef const BL_neighbormap_element_t *pcBL_neighbormap_element_t;

typedef struct {
    BL_2u32_t work_size;
    BL_neighbormap_element_t elements[0];
} BL_neighbormap_t, *pBL_neighbormap_t;
typedef const BL_neighbormap_t *pcBL_neighbormap_t;

int BL_neighbormap_new(const BL_2u32_t work_size, pBL_neighbormap_t *ppmap);
void BL_neighbormap_fill(pBL_neighbormap_t nbmap, pcBL_source_shift_map_t ssmap);
void BL_neighbormap_refine(pBL_neighbormap_t nbmap);
int BL_calc_source_coord(const BL_1r32_t *xy, const BL_1r32_t *uv, BL_2r32_t u0v0);
void BL_neighbormap_savecsv(FILE* pf, pcBL_neighbormap_t nbmap);
void BL_remap_savecsv(FILE* pf, pcBL_remap_t remap);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif /* BL_INVREMAP_H_ */
