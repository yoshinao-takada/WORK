#ifndef BL_SNPARRAY_H_
#define BL_SNPARRAY_H_
#include    "base_l/BL_errno.h"
#include    "base_l/BL_types.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    BL_snparray_type_UNKNOWN,
    BL_snparray_type_S, // S-parameter
    BL_snparray_type_T, // T-parameter
    BL_snparray_type_Y, // Admittance
    BL_snparray_type_Z, // Impedance
} BL_snparray_type_t;

typedef struct {
    BL_snparray_type_t type;
    uint32_t points;
    uint32_t ports;
    uint32_t element_size; // = ports * ports + 1 (matrix elements and complex angular frequency)
    BL_1c128_t* zc;
    BL_1c128_t* elements;
    BL_1c128_t data[0];
} BL_snparray_t, *pBL_snparray_t;
typedef const BL_snparray_t *pcBL_snparray_t;

pBL_snparray_t BL_snparray_new(uint32_t points, uint32_t ports);

void BL_snparray_setZc(pBL_snparray_t a, BL_1c128_t zc);
void BL_snparray_setZc2(pBL_snparray_t a, const BL_1c128_t* zc);

// copy all
int BL_snparray_copy(pBL_snparray_t dst, pcBL_snparray_t src);

// partial copy
int BL_snparray_partial_copy(
    pBL_snparray_t dst, pcBL_snparray_t src, uint32_t dst_begin, uint32_t src_begin, uint32_t length);

int BL_snparray_set_freq(pBL_snparray_t dst, double freq, uint32_t index);

int BL_snparray_set_angularfreq(pBL_snparray_t dst, double angularfreq, uint32_t index);

int BL_snparray_set_mat(pBL_snparray_t dst, const BL_1c128_t* mat, uint32_t index);

int BL_snparray_set_mat_element(pBL_snparray_t dst, BL_1c128_t element, uint32_t index, uint32_t row, uint32_t col);

int BL_snparray_get_freq(pcBL_snparray_t src, double* freq, uint32_t index);

int BL_snparray_get_angularfreq(pcBL_snparray_t src, double* angularfreq, uint32_t index);

int BL_snparray_get_mat(pcBL_snparray_t src, BL_1c128_t *mat, uint32_t index);

int BL_snparray_get_mat_element(pcBL_snparray_t src, BL_1c128_t *element, uint32_t index, uint32_t row, uint32_t col);


#ifdef __cplusplus
}
#endif
#endif /* BL_SNPARRAY_H_ */
