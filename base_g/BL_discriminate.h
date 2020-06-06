#ifndef BL_DISCRIMINATE_H_
#define BL_DISCRIMINATE_H_
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\brief retrieve centers of circular dots in an image.
step1: blur filter
step2: discriminate by color classification
step3: clustering
step4: calculate center of each cluster
*/
typedef struct {
    int32_t blur_k; // blur box filter K
    uint32_t color_count; // effective color count in colors array
    BL_3u8_t colors[8]; // colors[0]: target color, colors[1..color_count-1]: non-target color
} BL_discriminate_t, *pBL_discriminate_t;
typedef const BL_discriminate_t *pcBL_discriminate_t;

/*!
\brief create an integral image map of a selected channel of multichannel image.
\param image [in] multichannel image, like 3CH-BGR, 4CH-BGRA, ...
\param n_ch [in] channel count in the iamge
\param i_ch [in] selected channel
\param n_padding [in] padding width of integral image
\return integral image map of the selected channel
*/
pBL_arrayMD_t BL_IIu8u32(pcBL_arrayMD_t image, uint32_t n_ch, uint32_t i_ch, uint32_t n_padding);

/*!
\brief calculate partial sum using summed-area table
\param ii [in] summed area table (AKA integral-image)
\param sum_area [in] [0]:org.x, [1]:org.y, [2]:span.x, [3]:span.y
\param n_padding [in] padding width used in creating ii.
\return partial sum value
*/
uint32_t BL_IIu32_partialsum(pcBL_arrayMD_t ii, const BL_4i16_t sum_area, uint32_t n_padding);

/*!
\brief apply a box filter by using an integral image
\param ii [in] integral image map
\param k_2 [in] box filter k size parameter, actual K size is 2*k_2 + 1.
    k_2 = 0: delta function
    k_2 = 1: 3x3 box...
\param n_padding [in] padding width of integral image
\param filtered [out] filtered image of n_ch x uint8_t
\param n_ch [in] channel count of filtered. BGR: n_ch=3, BGRA: n_ch=4, grayscale: n_ch=1
\param i_ch [in] filter output target channel number, 0, ..., n_ch-1
\return unix errno compatible number
*/
int BL_IIu32u8_boxfilter(
    pcBL_arrayMD_t ii,
    uint32_t k_2,
    uint32_t n_padding,
    pBL_arrayMD_t filtered,
    uint32_t n_ch,
    uint32_t i_ch);

/*!
\brief discriminate color image creating a 1/0 monochrome pixel map
\param image [in] source color image
\param color_count [in] element count of colors
\param colors [in] reference colors
\return disciminated monochrome pixel map
*/
pBL_arrayMD_t BL_discriminate(pcBL_arrayMD_t image, uint32_t color_count, const BL_3u8_t *colors);

/*!
\brief discriminate pixels with a given index
\param index_map [in] index map create by BL_discriminate
\param ref_index [in] reference index
\return a binary pixel map, 1: index == ref_index, 0: index != ref_index
*/
pBL_arrayMD_t BL_discriminate2(pcBL_arrayMD_t index_map, uint8_t ref_index);

uint8_t BL_nearest_color(const BL_3u8_t color_under_test, uint32_t color_count, const BL_3u8_t* colors);

/*!
\brief assign segment IDs to separated islands of 1s in binary_map
\param binary_map [in] 0/1 pixel map
\return segment ID map of uint16_t
*/
pBL_arrayMD_t BL_segmentIDs(pcBL_arrayMD_t binary_map);

/*!
\brief get segment ID center pixel coordinates
\param segID_map [in] segment ID map
\return center coordinates of segments in segID_map. element type is BL_2r32_t.
*/
pBL_array_t BL_segment_centers(pcBL_arrayMD_t segID_map);
#ifdef __cplusplus
}
#endif
#endif
