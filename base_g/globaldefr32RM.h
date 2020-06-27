#include    "base_g/catmacro.h"
#define STFNTABLE_T STTYPE(matfn,r32,RM)
#define PSTFNTABLE_T PSTTYPE(matfn,r32,RM)
#define CPSTFNTABLE_T CPSTTYPE(matfn,r32,RM)
#define FNGET   FNNAME(matfn,r32,RM)
#ifdef __cplusplus
extern "C" {
#endif
// Matrix value, Scalar variable function (fill, unit, 3D rotation, etc.)
typedef CPnum   (* FNTYPE(M_Sm,r32,RM) )(Tnum s0, Pnum m);
typedef CPnum   (* FNTYPE(M_SmN,r32,RM) )(Tnum s0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_SmNN,r32,RM) )(Tnum s0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix variable function (transpose, etc.)
typedef CPnum   (* FNTYPE(M_Mm,r32,RM) )(CPnum m0, Pnum m);
typedef CPnum   (* FNTYPE(M_MmN,r32,RM) )(CPnum m0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MmNN,r32,RM) )(CPnum m0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix-Matrix variable function (add, subtract, multiply, etc.)
typedef CPnum   (* FNTYPE(M_MMm,r32,RM) )(CPnum m0, CPnum m1, Pnum m);
typedef CPnum   (* FNTYPE(M_MMmN,r32,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MMmNN,r32,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
typedef CPnum   (* FNTYPE(M_MMmNNN,r32,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr);
typedef CPnum   (* FNTYPE(M_MmmN,r32,RM) )(CPnum m0, Pnum m1, Pnum mwork, Tsize n);
// Matrix value, Matrix-Scalar variable function (muls)
typedef CPnum   (* FNTYPE(M_MNNSm,r32,RM) )(CPnum m0, Tsize nc0, Tsize nr0, Tnum s0, Pnum m);
// submatrix set operation
typedef CPnum    (* FNTYPE(M_mMNNNNNN,r32,RM) )(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// submatrix get operation
typedef CPnum    (* FNTYPE(M_MmNNNNNN,r32,RM) )(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// formated I/O
typedef Tenum   (* FNTYPE(E_MNNF,r32,RM) )(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
typedef Tenum   (* FNTYPE(E_pmnnF,r32,RM) )(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
// dense matrix linear equation
typedef Tenum   (* FNTYPE(E_mNNm,r32,RM))(Pnum m0, Tsize nc0, Tsize nr0, Pnum m1);
// vector equality discrimination
typedef Tenum   (* FNTYPE(E_MMNS,r32,RM))(CPnum m0, CPnum m1, Tsize nc0, Tnum s0);
typedef struct {
    const FNTYPE(M_Sm,r32,RM)   rotxrad; // rotation matrix around x-axis, angle unit: radian
    const FNTYPE(M_Sm,r32,RM)   rotyrad; // rotation matrix around y-axis, angle unit: radian
    const FNTYPE(M_Sm,r32,RM)   rotzrad; // rotation matrix around z-axis, angle unit: radian
    const FNTYPE(M_Sm,r32,RM)   rotxdeg; // rotation matrix around x-axis, angle unit: degree
    const FNTYPE(M_Sm,r32,RM)   rotydeg; // rotation matrix around y-axis, angle unit: degree
    const FNTYPE(M_Sm,r32,RM)   rotzdeg; // rotation matrix around z-axis, angle unit: degree
    const FNTYPE(M_Mm,r32,RM)   translate; // translate matrix, linearly move by a 3D vector.
    const FNTYPE(M_SmN,r32,RM)  unit; // unit matrix multiplied by a scalar number
    const FNTYPE(M_SmNN,r32,RM) fill; // fill with a scalar number
    const FNTYPE(M_MmNN,r32,RM) transpose; // transpose nc, nr are columns and rows of transposed matrix.
    const FNTYPE(M_MMmNN,r32,RM) add; // add two matrices
    const FNTYPE(M_MMmNN,r32,RM) sub; // subtract a matrix from another
    const FNTYPE(M_MMmNNN,r32,RM) mul; // multiply two matrices
    const FNTYPE(M_MNNSm,r32,RM)   muls; // multiply matrix and scalar number
    const FNTYPE(M_MmmN,r32,RM) inv; // invert a matrix
    const FNTYPE(M_mMNNNNNN,r32,RM) setsubmat; // set submatrix
    const FNTYPE(M_MmNNNNNN,r32,RM) getsubmat; // get submatrix
    const FNTYPE(E_MNNF,r32,RM) writef; // write as formated text
    const FNTYPE(E_MNNF,r32,RM) writeb; // write as CPU dependent binary
    const FNTYPE(E_pmnnF,r32,RM) readf; // read as formated text
    const FNTYPE(E_pmnnF,r32,RM) readb; // read as CPU dependent binary
    const FNTYPE(E_mNNm,r32,RM) soldense; // solve dense linear equation
    const FNTYPE(E_MMNS,r32,RM) equalv; // 1: m0 == m1 within s0 tolerance
}   STFNTABLE_T, * PSTFNTABLE_T ;
typedef const STFNTABLE_T * CPSTFNTABLE_T;
CPSTFNTABLE_T   FNGET();
#ifdef __cplusplus
}
#endif
