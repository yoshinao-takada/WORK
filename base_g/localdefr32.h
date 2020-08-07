#include    <math.h>
#include    <float.h>
#define	Tnum	BL_1r32_t
#define Pnum	BL_1r32_t*
#define	CPnum	const BL_1r32_t*
#define Trnum   BL_1r32_t
#define Prnum   BL_1r32_t*
#define CPrnum  const BL_1r32_t*
#define	Tsize	BL_1u32_t
#define	Psize	BL_1u32_t*
#define	CPsize	const BL_1u32_t*
#define Tenum   int
#define MATH_COS    cosf
#define MATH_SIN    sinf
#define MATH_TAN    tanf
#define MATH_ACOS   acosf
#define MATH_ASIN   asinf
#define MATH_ATAN   atanf
#define MATH_ATAN2  atan2f
#define MATH_ABS    fabsf
#define MATH_PI     ((float)M_PI)
#define MATH_PI2    ((float)M_PI_2)
#define MATH_PI4    ((float)(M_PI/4.0))
#define MATH_SQRT   sqrtf
#define MATH_COSH   coshf
#define MATH_SINH   sinhf
#define MATH_TANH   tanhf
#define MATH_DEG2RAD(deg)   (MATH_PI * deg / 180.0f)
#define _0          0.0f
#define _1          1.0f
#define _2          2.0f
#define MIN_L2SUM   1.0e-32f
#define NUMFORMATS  { "%f", ",%f" }
#define WRITENUMF(pf,fmt,num) fprintf(pf,fmt,num)
#define REAL_PART(_a)   (_a)
#define IMAG_PART(_a)   0.0f
#define L2NORM(_a)      sqrtf(_a * _a)
#define CONJ(_a)        (_a)
#define RNUM_TO_NUM(_r) (_r)
