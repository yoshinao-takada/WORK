#ifndef BL_MATEXPORT_H_
#define BL_MATEXPORT_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
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
#undef      MATLAYOUTRM
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
#undef      MATLAYOUTCM
#ifdef __cplusplus
}
#endif
#endif