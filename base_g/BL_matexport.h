#ifndef BL_MATEXPORT_H_
#define BL_MATEXPORT_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    <stdio.h>
// row-major matrix operations
#define     MATLAYOUTRM
#define     NUMTYPEr32
#include    "base_g/localdefr32.h"
#include    "base_g/globaldefr32RM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEr32
#define     NUMTYPEr64
#include    "base_g/localdefr64.h"
#include    "base_g/globaldefr64RM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEr64
#define     NUMTYPEc64
#include    "base_g/localdefc64.h"
#include    "base_g/globaldefc64RM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEc64
#define     NUMTYPEc128
#include    "base_g/localdefc128.h"
#include    "base_g/globaldefc128RM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEc128
#undef      MATLAYOUTRM

// column-major matrix operations
#define     MATLAYOUTCM
#define     NUMTYPEr32
#include    "base_g/localdefr32.h"
#include    "base_g/globaldefr32CM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEr32
#define     NUMTYPEr64
#include    "base_g/localdefr64.h"
#include    "base_g/globaldefr64CM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEr64
#define     NUMTYPEc64
#include    "base_g/localdefc64.h"
#include    "base_g/globaldefc64CM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEc64
#define     NUMTYPEc128
#include    "base_g/localdefc128.h"
#include    "base_g/globaldefc128CM.h"
#include    "base_g/undeflocal.h"
#include    "base_g/undefglobal.h"
#undef      NUMTYPEc128
#undef      MATLAYOUTCM
#endif