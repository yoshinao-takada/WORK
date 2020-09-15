#ifndef BL_STYZ_H_
#define BL_STYZ_H_
#include    "base_g/BL_matexport.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\file BL_styz.h
\brief S, T, Y, Z linear circuit matrix operations in row major layout
*/
#define BL_STYZ_WORKSIZE    16
typedef struct {
    uint32_t port_count;
    const BL_1c128_t *Sz; // diag[Zc0,Zc1,...]
    const BL_1c128_t *Sy; // diag[Yc0,Yc1,...]
    const BL_1c128_t *U; // unit matrix
    const BL_1c128_t *work[BL_STYZ_WORKSIZE]; // work area
    BL_1c128_t buf[0];
} BL_styz_t, *pBL_styz_t;
typedef const BL_styz_t *pcBL_styz_t;

/*!
\brief constructor
\param port_count [in] number of ports which character impedances are all 50 ohms.
\return pointer to the constructed object which can be freed by BL_SAFEFREE()
*/
pBL_styz_t BL_styz_new(uint32_t port_count);

/*!
\brief constructor
\param port_count [in] number of ports
\param Zc [in] characteristic impedance is common to all ports.
\return pointer to the constructed object which can be freed by BL_SAFEFREE()
*/
pBL_styz_t BL_styz_new1(uint32_t port_count, double Zc);

/*!
\brief constructor
\param port_count [in] number of ports
\param Zc [in] characteristic impedances are defined for each ports.
\return pointer to the constructed object which can be freed by BL_SAFEFREE()
*/
pBL_styz_t BL_styz_new2(uint32_t port_count, const double *Zc);


/*!
\brief conversion from Z to S
\param p [in,out]linear circuit matrix object
\param z [in] impedance matrix
\return S-parameter is held by 
*/
const BL_1c128_t* BL_styz_zs(pBL_styz_t p, const BL_1c128_t z);

const BL_1c128_t* BL_styz_sz(pBL_styz_t p, const BL_1c128_t s);

const BL_1c128_t* BL_styz_ys(pBL_styz_t p, const BL_1c128_t y);

const BL_1c128_t* BL_styz_sy(pBL_styz_t p, const BL_1c128_t s);

const BL_1c128_t* BL_styz_ts(pBL_styz_t p, const BL_1c128_t t);

const BL_1c128_t* BL_styz_st(pBL_styz_t p, const BL_1c128_t s);
#ifdef __cplusplus
}
#endif
#endif /* BL_STYZ_H_ */
