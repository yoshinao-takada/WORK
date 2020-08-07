#include    "base_g/catmacro.h"
#define STFNTABLE_T STTYPE(matfn,c128,RM)
#define PSTFNTABLE_T PSTTYPE(matfn,c128,RM)
#define CPSTFNTABLE_T CPSTTYPE(matfn,c128,RM)
#define FNGET   FNNAME(matfn,c128,RM)
#ifdef __cplusplus
extern "C" {
#endif
// Matrix value, Scalar variable function (fill, unit, 3D rotation, etc.)
typedef CPnum   (* FNTYPE(M_Sm,c128,RM) )(Tnum s0, Pnum m);
typedef CPnum   (* FNTYPE(M_SmN,c128,RM) )(Tnum s0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_SmNN,c128,RM) )(Tnum s0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix variable function (transpose, etc.)
typedef CPnum   (* FNTYPE(M_Mm,c128,RM) )(CPnum m0, Pnum m);
typedef CPnum   (* FNTYPE(M_MmN,c128,RM) )(CPnum m0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MmNN,c128,RM) )(CPnum m0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix-Matrix variable function (add, subtract, multiply, etc.)
typedef CPnum   (* FNTYPE(M_MMm,c128,RM) )(CPnum m0, CPnum m1, Pnum m);
typedef CPnum   (* FNTYPE(M_MMmN,c128,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MMmNN,c128,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
typedef CPnum   (* FNTYPE(M_MMmNNN,c128,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr);
typedef CPnum   (* FNTYPE(M_MmmN,c128,RM) )(CPnum m0, Pnum m1, Pnum mwork, Tsize n);
// Matrix value, Matrix-Scalar variable function (muls)
typedef CPnum   (* FNTYPE(M_MNNSm,c128,RM) )(CPnum m0, Tsize nc0, Tsize nr0, Tnum s0, Pnum m);
// submatrix set operation
typedef CPnum    (* FNTYPE(M_mMNNNNNN,c128,RM) )(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// submatrix get operation
typedef CPnum    (* FNTYPE(M_MmNNNNNN,c128,RM) )(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// formated I/O
typedef Tenum   (* FNTYPE(E_MNNF,c128,RM) )(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
typedef Tenum   (* FNTYPE(E_pmnnF,c128,RM) )(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
// dense matrix linear equation
typedef Tenum   (* FNTYPE(E_mNNm,c128,RM))(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1);
// vector equality discrimination
typedef Tenum   (* FNTYPE(E_MMNS,c128,RM))(CPnum m0, CPnum m1, Tsize nc0, Tnum s0);
// vector cross product
typedef CPnum   (* FNTYPE(M_MMm,c128,RM))(CPnum m0, CPnum m1, Pnum m0xm1);
// vector normalize
typedef CPnum   (* FNTYPE(M_MmN,c128,RM))(CPnum m0, Pnum m, Tsize ndim);
typedef struct {
    const FNTYPE(M_Sm,c128,RM)   rotxrad; // rotation matrix around x-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,RM)   rotyrad; // rotation matrix around y-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,RM)   rotzrad; // rotation matrix around z-axis, angle unit: radian
    const FNTYPE(M_Sm,c128,RM)   rotxdeg; // rotation matrix around x-axis, angle unit: degree
    const FNTYPE(M_Sm,c128,RM)   rotydeg; // rotation matrix around y-axis, angle unit: degree
    const FNTYPE(M_Sm,c128,RM)   rotzdeg; // rotation matrix around z-axis, angle unit: degree
    const FNTYPE(M_Mm,c128,RM)   translate; // translate matrix, linearly move by a 3D vector.
    const FNTYPE(M_SmN,c128,RM)  unit; // unit matrix multiplied by a scalar number
    const FNTYPE(M_SmNN,c128,RM) fill; // fill with a scalar number
    const FNTYPE(M_MmNN,c128,RM) transpose; // transpose nc, nr are columns and rows of transposed matrix.
    const FNTYPE(M_MMmNN,c128,RM) add; // add two matrices
    const FNTYPE(M_MMmNN,c128,RM) sub; // subtract a matrix from another
    const FNTYPE(M_MMmNNN,c128,RM) mul; // multiply two matrices
    const FNTYPE(M_MNNSm,c128,RM)   muls; // multiply matrix and scalar number
    const FNTYPE(M_MmmN,c128,RM) inv; // invert a matrix
    const FNTYPE(M_mMNNNNNN,c128,RM) setsubmat; // set submatrix
    const FNTYPE(M_MmNNNNNN,c128,RM) getsubmat; // get submatrix
    const FNTYPE(E_MNNF,c128,RM) writef; // write as formated text
    const FNTYPE(E_MNNF,c128,RM) writeb; // write as CPU dependent binary
    const FNTYPE(E_pmnnF,c128,RM) readf; // read as formated text
    const FNTYPE(E_pmnnF,c128,RM) readb; // read as CPU dependent binary
    const FNTYPE(E_mNNm,c128,RM) soldense; // solve dense linear equation
    const FNTYPE(E_MMNS,c128,RM) equalv; // 1: m0 == m1 within s0 tolerance
    const FNTYPE(M_MMm,c128,RM) crossproduct;   // cross product of two vectors which is 1x3 or 3x1 matrices.
    const FNTYPE(M_MmN,c128,RM) normalize;   // normalize a vector
}   STFNTABLE_T, * PSTFNTABLE_T ;
typedef const STFNTABLE_T * CPSTFNTABLE_T;
CPSTFNTABLE_T   FNGET();
#ifdef __cplusplus
}
#endif
