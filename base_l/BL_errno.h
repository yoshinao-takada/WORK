#define	ESUCCESS		0
#define	ERR_SUCCESS		ESUCCESS
// error code for matrix operation
#define	ERR_MATRIX		0x4000
#define	ERR_MATRIX_ILLFORMED	ERR_MATRIX
#define	ERR_MATRIX_SINGULAR	    (ERR_MATRIX + 1)
#define	ERR_MATRIX_SIZEMISMATCH	(ERR_MATRIX + 2)
// error code for geometrical operation
#define ERR_GEO                 (ERR_MATRIX + 0x10)
#define ERR_GEO_RANGE           ERR_GEO
#define ERR_GEO_DEPTH_RANGE     (ERR_GEO_RANGE + 1)
// error code for general operation
#define ERR_GENERAL             (ERR_GEO + 0x10)
#define ERR_NO_CONVERGENCE      ERR_GENERAL
#include    <errno.h>
