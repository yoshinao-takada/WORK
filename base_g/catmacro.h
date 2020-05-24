#ifndef CATMACRO_H_
#define CATMACRO_H_
#define	FNTYPE(ATI,NTI,LTI)	BL_ ## ATI ## NTI ## LTI ## _ft
#define	FNNAME(FNBASE,NTI,LTI)	BL_ ## FNBASE ## NTI ## LTI
#define	STTYPE(STBASE,NTI,LTI)	BL_ ## STBASE ## NTI ## LTI ## _t
#define	PSTTYPE(STBASE,NTI,LTI)	pBL_ ## STBASE ## NTI ## LTI ## _t
#define	CPSTTYPE(STBASE,NTI,LTI)	pcBL_ ## STBASE ## NTI ## LTI ## _t
#define IDXRM(icol,irow,nc)     (icol + irow * nc)
#define IDXCM(icol,irow,nr)     (irow + icol * nr)
#endif
