#ifndef BL_STYZ_H_
#define BL_STYZ_H_
#include    "base_g/BL_matexport.h"
#ifdef __cplusplus
extern "C" {
#endif

/*!
\file BL_styz.h
\brief Work number array for S, T, Y, Z linear circuit matrix operations in row major layout
*/
#define BL_STYZ_WORKSIZE    16
typedef struct {
    uint32_t port_count;
    BL_1c128_t *Sz; // diag[sqrt(Zc0), sqrt(Zc1),...] square matrix
    BL_1c128_t *Sy; // diag[sqrt(Yc0),sqrt(Yc1),...] square matrix
    BL_1c128_t *IMat; // identity matrix
    BL_1c128_t *work[BL_STYZ_WORKSIZE]; // work area
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
\param index_offset [in] index to the first work[*] element
\return S-parameter is held by the returned pointer
*/
const BL_1c128_t* BL_styz_zs(pBL_styz_t p, const BL_1c128_t* z, uint32_t index_offset);

/*!
\brief conversion from S to Z
\param p [in,out]linear circuit matrix object
\param s [in] S-parameter
\return The impedance matrix is held by the returned pointer
*/
const BL_1c128_t* BL_styz_sz(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset);

/*!
\brief conversion from Y to S
\param p [in,out]linear circuit matrix object
\param y [in] admittance matrix
\return The S-parameter is held by the returned pointer
*/
const BL_1c128_t* BL_styz_ys(pBL_styz_t p, const BL_1c128_t* y, uint32_t index_offset);

/*!
\brief conversion from S to Y
\param p [in,out]linear circuit matrix object
\param s [in] S-parameter
\return The admittance matrix is held by the returned pointer
*/
const BL_1c128_t* BL_styz_sy(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset);

/*!
\brief conversion from T to S
\param p [in,out]linear circuit matrix object
\param t [in] T-parameter
\return The S-parameter is held by the returned pointer
*/
const BL_1c128_t* BL_styz_ts(pBL_styz_t p, const BL_1c128_t* t, uint32_t index_offset);

/*!
\brief conversion from S to T
\param p [in,out]linear circuit matrix object
\param s [in] S-parameter
\return The T-parameter is held by the returned pointer
*/
const BL_1c128_t* BL_styz_st(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset);
#ifdef __cplusplus
}
#endif
#endif /* BL_STYZ_H_ */
