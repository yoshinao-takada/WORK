#include    <math.h>
#define	Tnum	BL_1r64_t
#define Pnum	BL_1r64_t*
#define	CPnum	const BL_1r64_t*
#define Trnum   BL_1r64_t
#define Prnum   BL_1r64_t*
#define CPrnum  const BL_1r64_t*
#define	Tsize	BL_1u32_t
#define	Psize	BL_1u32_t*
#define	CPsize	const BL_1u32_t*
#define Tenum   int
#define MATH_COS    cos
#define MATH_SIN    sin
#define MATH_TAN    tan
#define MATH_ACOS   acos
#define MATH_ASIN   asin
#define MATH_ATAN   atan
#define MATH_ATAN2  atan2
#define MATH_ABS    fabs
#define MATH_PI     M_PI
#define MATH_PI2    M_PI_2
#define MATH_PI4    (M_PI/4.0)
#define MATH_SQRT   sqrt
#define MATH_COSH   cosh
#define MATH_SINH   sinh
#define MATH_TANH   tanh
#define MATH_DEG2RAD(deg)   (MATH_PI * deg / 180.0)
#define _0          0.0
#define _1          1.0
#define _2          2.0
#define MIN_L2SUM   1.0e-64
#define NUMFORMATS  { "%f", ",%f" }
#define WRITENUMF(pf,fmt,num) fprintf(pf,fmt,num)
#define REAL_PART(_a)   (_a)
#define IMAG_PART(_a)   0.0
#define L2NORM(_a)      sqrt(_a * _a)
#define CONJ(_a)        (_a)
#define RNUM_TO_NUM(_r) (_r)