#include    "base_g/catmacro.h"
#define STFNTABLE_T STTYPE(matfn,r64,RM)
#define PSTFNTABLE_T PSTTYPE(matfn,r64,RM)
#define CPSTFNTABLE_T CPSTTYPE(matfn,r64,RM)
#define FNGET   FNNAME(matfn,r64,RM)

// Matrix value, Scalar variable function (fill, unit, 3D rotation, etc.)
typedef CPnum   (* FNTYPE(M_Sm,r64,RM) )(Tnum s0, Pnum m);
typedef CPnum   (* FNTYPE(M_SmN,r64,RM) )(Tnum s0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_SmNN,r64,RM) )(Tnum s0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix variable function (transpose, etc.)
typedef CPnum   (* FNTYPE(M_Mm,r64,RM) )(CPnum m0, Pnum m);
typedef CPnum   (* FNTYPE(M_MmN,r64,RM) )(CPnum m0, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MmNN,r64,RM) )(CPnum m0, Pnum m, Tsize nc, Tsize nr);
// Matrix value, Matrix-Matrix variable function (add, subtract, multiply, etc.)
typedef CPnum   (* FNTYPE(M_MMm,r64,RM) )(CPnum m0, CPnum m1, Pnum m);
typedef CPnum   (* FNTYPE(M_MMmN,r64,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize n);
typedef CPnum   (* FNTYPE(M_MMmNN,r64,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize nr);
typedef CPnum   (* FNTYPE(M_MMmNNN,r64,RM) )(CPnum m0, CPnum m1, Pnum m, Tsize nc, Tsize ni, Tsize nr);
typedef CPnum   (* FNTYPE(M_MmmN,r64,RM) )(CPnum m0, Pnum m1, Pnum mwork, Tsize n);
// submatrix set operation
typedef CPnum    (* FNTYPE(M_mMNNNNNN,r64,RM) )(Pnum m0, CPnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// submatrix get operation
typedef CPnum    (* FNTYPE(M_MmNNNNNN,r64,RM) )(CPnum m0, Pnum m1, Tsize nc0, Tsize nr0, Tsize nc1, Tsize nr1, Tsize ncoff, Tsize nroff);
// formated I/O
typedef Tenum   (* FNTYPE(E_MNNF,r64,RM) )(CPnum m0, Tsize nc0, Tsize nr0, FILE* pf);
typedef Tenum   (* FNTYPE(E_pmnnF,r64,RM) )(Pnum* ppm0, Psize nc0, Psize nr0, FILE* pf);
typedef struct {
    const FNTYPE(M_Sm,r64,RM)   rotxrad; // rotation matrix around x-axis, angle unit: radian
    const FNTYPE(M_Sm,r64,RM)   rotyrad; // rotation matrix around y-axis, angle unit: radian
    const FNTYPE(M_Sm,r64,RM)   rotzrad; // rotation matrix around z-axis, angle unit: radian
    const FNTYPE(M_Sm,r64,RM)   rotxdeg; // rotation matrix around x-axis, angle unit: degree
    const FNTYPE(M_Sm,r64,RM)   rotydeg; // rotation matrix around y-axis, angle unit: degree
    const FNTYPE(M_Sm,r64,RM)   rotzdeg; // rotation matrix around z-axis, angle unit: degree
    const FNTYPE(M_Mm,r64,RM)   translate; // translate matrix, linearly move by a 3D vector.
    const FNTYPE(M_SmN,r64,RM)  unit; // unit matrix multiplied by a scalar number
    const FNTYPE(M_SmNN,r64,RM) fill; // fill with a scalar number
    const FNTYPE(M_MmNN,r64,RM) transpose; // transpose nc, nr are columns and rows of transposed matrix.
    const FNTYPE(M_MMmNN,r64,RM) add; // add two matrices
    const FNTYPE(M_MMmNN,r64,RM) sub; // subtract a matrix from another
    const FNTYPE(M_MMmNNN,r64,RM) mul; // multiply two matrices
    const FNTYPE(M_MmmN,r64,RM) inv; // invert a matrix
    const FNTYPE(M_mMNNNNNN,r64,RM) setsubmat; // set submatrix
    const FNTYPE(M_MmNNNNNN,r64,RM) getsubmat; // get submatrix
    const FNTYPE(E_MNNF,r64,RM) writef; // write as formated text
    const FNTYPE(E_MNNF,r64,RM) writeb; // write as CPU dependent binary
    const FNTYPE(E_pmnnF,r64,RM) readf; // read as formated text
    const FNTYPE(E_pmnnF,r64,RM) readb; // read as CPU dependent binary
}   STTYPE(matfn,r64,RM), * PSTTYPE(matfn,r64,RM) ;
typedef const STTYPE(matfn,r64,RM) * CPSTTYPE(matfn,r64,RM);
CPSTFNTABLE_T   FNGET();
