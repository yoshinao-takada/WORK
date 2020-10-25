#include    <math.h>
#include    <complex.h>
#define	Tnum	BL_1c128_t
#define Pnum	BL_1c128_t*
#define	CPnum	const BL_1c128_t*
#define Trnum   BL_1r64_t
#define Prnum   BL_1r64_t*
#define CPrnum  const BL_1r64_t*
#define	Tsize	BL_1u32_t
#define	Psize	BL_1u32_t*
#define	CPsize	const BL_1u32_t*
#define Tenum   int
#define MATH_COS    ccos
#define MATH_SIN    csin
#define MATH_TAN    ctan
#define MATH_ACOS   cacos
#define MATH_ASIN   casin
#define MATH_ATAN   catan
#define MATH_ATAN2  catan2
#define MATH_ABS    cfabs
#define MATH_PI     CMPLX(M_PI,0.0)
#define MATH_PI2    CMPLX(M_PI_2,0.0)
#define MATH_PI4    CMPLX((M_PI/4.0),0.0)
#define MATH_SQRT   csqrt
#define MATH_COSH   ccosh
#define MATH_SINH   csinh
#define MATH_TANH   ctanh
#define MATH_DEG2RAD(deg)   CMPLX((MATH_PI * deg / 180.0),0.0)
#define _0          CMPLX(0.0,0.0)
#define _1          CMPLX(1.0,0.0)
#define _2          CMPLX(2.0,0.0)
#define MIN_L2SUM   1.0e-64
#define NUMFORMATS  { "%f,%f", ",%f,%f" }
#define WRITENUMF(pf,fmt,num) fprintf(pf,fmt,num)
#define REAL_PART(_a)   creal(_a)
#define IMAG_PART(_a)   cimag(_a)
#define L2NORM(_a)      creal(sqrt(_a * conj(_a)))
#define CONJ(_a)        conj(_a)
#define RNUM_TO_NUM(_r) CMPLX(_r,0.0)