#include    <math.h>
#include    <float.h>
#include    <complex.h>
#define	Tnum	BL_1c64_t
#define Pnum	BL_1c64_t*
#define	CPnum	const BL_1c64_t*
#define Trnum   BL_1r32_t
#define Prnum   BL_1r32_t*
#define CPrnum  const BL_1r32_t*
#define	Tsize	BL_1u32_t
#define	Psize	BL_1u32_t*
#define	CPsize	const BL_1u32_t*
#define Tenum   int
#define MATH_COS    ccosf
#define MATH_SIN    csinf
#define MATH_TAN    ctanf
#define MATH_ACOS   cacosf
#define MATH_ASIN   casinf
#define MATH_ATAN   catanf
#define MATH_ATAN2  catan2f
#define MATH_ABS    cabsf
#define MATH_PI     CMPLXF(((float)M_PI),0.0f)
#define MATH_PI2    CMPLXF(((float)M_PI_2),0.0f)
#define MATH_PI4    CMPLXF(((float)(M_PI/4.0)),0.0f)
#define MATH_SQRT   csqrtf
#define MATH_COSH   ccoshf
#define MATH_SINH   csinhf
#define MATH_TANH   ctanhf
#define MATH_DEG2RAD(deg)   CMPLXF((MATH_PI * deg / 180.0f),0.0f)
#define _0          CMPLXF(0.0f,0.0f)
#define _1          CMPLXF(1.0f,0.0f)
#define _2          CMPLXF(2.0f,0.0f)
#define MIN_L2SUM   1.0e-32f
#define NUMFORMATS  { "%f,%f", ",%f,%f" }
#define WRITENUMF(pf,fmt,num) fprintf(pf,fmt,num)
#define REAL_PART(_a)   crealf(_a)
#define IMAG_PART(_a)   cimagf(_a)
#define L2NORM(_a)      crealf(sqrtf(_a * conjf(_a)))
#define CONJ(_a)        conjf(_a)
#define RNUM_TO_NUM(_r) CMPLXF(_r,0.0f)
