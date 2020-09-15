#ifndef BL_SNPARRAY_H_
#define BL_SNPARRAY_H_
#include    "base_l/BL_errno.h"
#include    "base_l/BL_types.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    double *w; // angular frequency
    BL_1c128_t* e; // matrix element
} BL_snpelements_t, *pBL_snpelements_t;
typedef cosnt BL_snpelements_t *pcBL_snpelements_t;

typedef enum {
    BL_immitance_admittance,
    BL_immitance_impedance,
    BL_immitance_s_parameter,
    BL_immitance_t_parameter,
    BL_immitance_voltage,
    BL_immitance_current,
} BL_immitanceType_t;

typedef enum {
    BL_immitanceSubtype_singleFreq, // frequency domain, plus side
    BL_immitanceSubtype_conjpairFreq, // frequency domain, conjugate pair
    BL_immitanceSubtype_singleTime, // time domain, plus side
    BL_immitanceSubtype_conjpairTime, // time domain, conjugate pair
} BL_immitanceSubtype_t;

typedef struct {
    uint32_t point; // frequency point count
    uint32_t port; // port count
    BL_1c128_t *zc; // port characteristic impedances
    BL_immitanceType_t type;
    BL_immitanceSubtype_t subtype;
} BL_snpheader_t, *pBL_snpheader_t;
typedef const BL_snpheader_t *pcBL_snpheader_t;

typedef struct {
    BL_snpheader_t header;
    BL_snpelements_t begin; // accessor to the beginning element
    BL_snpelements_t end; // accessor to the end element (beginning of out-of-range region)
    BL_snpelements_t iter; // iterator
    uint8_t buf[0];
} BL_snparray_t, *pBL_snparray_t;
typedef const BL_snparray_t *pcBL_snparray_t;

/*!
\brief constructor
\param point [in] frequency point
\param port [in] port count
\param zc [in] port characteristic impedance
\return pointer to the new object which can be released by BL_SAFEFREE().
*/
pBL_snparray_t BL_snparray_new(uint32_t point, uint32_t port);
pBL_snparray_t BL_snparray_new1(uint32_t point, uint32_t port, double zc);
pBL_snparray_t BL_snparray_new2(uint32_t point, uint32_t port, double zc);

/*!
\brief move pointers in p->iter if the position is not end
\param p [in,out] object
\return ERANGE or ESUCCESS
*/
int BL_snparray_gonext(pBL_snparray_t p);
int BL_snparray_goprev(pBL_snparray_t p);

/*!
\brief move the iterator
\param p [in] object
\param iter [in,out] iterator to move
\return ERANGE or ESUCCESS
*/
int BL_snparray_movenext(pcBL_snparray_t p, pBL_snpelements_t iter);
int BL_snparray_moveprev(pcBL_snparray_t p, pBL_snpelements_t iter);
#ifdef __cplusplus
}
#endif
#endif /* BL_SNPARRAY_H_ */
