#include    "base_g/catmacro.h"
#define STFNTABLE_T STTYPE(matfn,c128,CM)
#define PSTFNTABLE_T PSTTYPE(matfn,c128,CM)
#define CPSTFNTABLE_T CPSTTYPE(matfn,c128,CM)
#define FNGET   FNNAME(matfn,c128,CM)
#ifdef __cplusplus
extern "C" {
#endif
// Matrix value, Scalar variable function (fill, unit, 3D rotation, etc.)
typedef CPnum   (* FNTYPE(M_Sm,c128,CM) )(Tnum s0, Pnum m);
typedef CPnum   (* FNTYPE(M_SmN,c128,CM) )(Tnum s0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_SmNN,c128,CM) )(Tnum s0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix variable function (transpose, etc.)
typedef CPnum   (* FNTYPE(M_Mm,c128,CM) )(CPnum m0, Pnum m);
typedef CPnum   (* FNTYPE(M_MmN,c128,CM) )(CPnum m0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MmNN,c128,CM) )(CPnum m0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix-Matrix variable function (add, subtract, multiply, etc.)
typedef CPnum   (* FNTYPE(M_MMm,c128,CM) )(CPnum m0, CPnum m1, Pnum m);
typedef CPnum   (* FNTYPE(M_MMmN,c128,CM) )(CPnum m0, CPnum m1, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MMmNN,c128,CM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
typedef CPnum   (* FNTYPE(M_MMmNNN,c128,CM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr);
typedef CPnum   (* FNTYPE(M_MmmN,c128,CM) )(CPnum m0, Pnum m1, Pnum mwork, Tsize n);
// Matrix value, Matrix-Scalar variable function (muls)
typedef CPnum   (* FNTYPE(M_MNNSm,c128,CM) )(CPnum m0, Tsize nc0, Tsize nr0, Tnum s0, Pnum m);
// submatrix set operation
typedef CPnum    (* FNTYPE(M_mMNNNNNN,c128,CM) )(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// submatrix get operation
typedef CPnum    (* FNTYPE(M_MmNNNNNN,c128,CM) )(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// formated I/O
typedef Tenum   (* FNTYPE(E_MNNF,c128,CM) )(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
typedef Tenum   (* FNTYPE(E_pmnnF,c128,CM) )(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
// dense matrix linear equation
typedef Tenum   (* FNTYPE(E_mNNm,c128,CM))(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1);
// vector equality discrimination
typedef Tenum   (* FNTYPE(E_MMNS,c128,CM))(CPnum m0, CPnum m1, Tsize nc0, Tnum s0);
// vector cross product
typedef CPnum   (* FNTYPE(M_MMm,c128,CM))(CPnum m0, CPnum m1, Pnum m0xm1);
// vector normalize
typedef CPnum   (* FNTYPE(M_MmN,c128,CM))(CPnum m0, Pnum m, Tsize ndim);
typedef struct {
    const FNTYPE(M_Sm,c128,CM)   rotxrad; // rotation matrix around x-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,CM)   rotyrad; // rotation matrix around y-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,CM)   rotzrad; // rotation matrix around z-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,CM)   rotxdeg; // rotation matrix around x-axis, angle unit: degree
    const FNTYPE(M_Sm,c128,CM)   rotydeg; // rotation matrix around y-axis, angle unit: degree
    const FNTYPE(M_Sm,c128,CM)   rotzdeg; // rotation matrix around z-axis, angle unit: degree
    const FNTYPE(M_Mm,c128,CM)   translate; // translate matrix, linearly move by a 3D vector.
    const FNTYPE(M_SmN,c128,CM)  unit; // unit matrix multiplied by a scalar number
    const FNTYPE(M_SmNN,c128,CM) fill; // fill with a scalar number
    const FNTYPE(M_MmNN,c128,CM) transpose; // transpose nc, nr are columns and rows of transposed matrix.
    const FNTYPE(M_MMmNN,c128,CM) add; // add two matrices
    const FNTYPE(M_MMmNN,c128,CM) sub; // subtract a matrix from another
    const FNTYPE(M_MMmNNN,c128,CM) mul; // multiply two matrices
    const FNTYPE(M_MNNSm,c128,CM)   muls; // multiply matrix and scalar number
    const FNTYPE(M_MmmN,c128,CM) inv; // invert a matrix
    const FNTYPE(M_mMNNNNNN,c128,CM) setsubmat; // set submatrix
    const FNTYPE(M_MmNNNNNN,c128,CM) getsubmat; // get submatrix
    const FNTYPE(E_MNNF,c128,CM) writef; // write as formated text
    const FNTYPE(E_MNNF,c128,CM) writeb; // write as CPU dependent binary
    const FNTYPE(E_pmnnF,c128,CM) readf; // read as formated text
    const FNTYPE(E_pmnnF,c128,CM) readb; // read as CPU dependent binary
    const FNTYPE(E_mNNm,c128,CM) soldense; // solve dense linear equation
    const FNTYPE(E_MMNS,c128,CM) equalv; // 1: m0 == m1 within s0 tolerance
    const FNTYPE(M_MMm,c128,CM) crossproduct;   // cross product of two vectors which is 1x3 or 3x1 matrices.
    const FNTYPE(M_MmN,c128,CM) normalize;   // normalize a vector
}   STFNTABLE_T, * PSTFNTABLE_T ;
typedef const STFNTABLE_T * CPSTFNTABLE_T;
CPSTFNTABLE_T   FNGET();
#ifdef __cplusplus
}
#endif
