#ifndef BL_SNPARRAYIO_H_
#define BL_SNPARRAYIO_H_
#include    "base_z/BL_snparrayIO.h"
#include    "base_l/BL_futils.h"
#include    "base_l/BL_errno.h"
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum {
        BL_unit_hz,
        BL_unit_killohz,
        BL_unit_megahz,
        BL_unit_gigahz,
        BL_unit_terahz,
        BL_unit_picosec,
        BL_unit_nanosec,
        BL_unit_microsec,
        BL_unit_millisec,
        BL_unit_sec,
        BL_unit_nounit,
    } BL_snpunit_t;
    #define BL_UNIT_LABELS { \
        "Hz", "kHz", "MHz", "GHz", "THz", \
        "ps", "ns", "ms", "us", "s", "-", "NOUNIT" }

    typedef enum {
        BL_complexnumberformat_DB,
        BL_complexnumberformat_MA,
        BL_complexnumberformat_RI,
    } BL_complexnumberformat_t;
    #define BL_COMPLEX_FORMAT_LABELS { "DB", "MA", "RI" }


    typedef enum {
        BL_snpparamtype_S,
        BL_snpparamtype_T,
        BL_snpparamtype_Y,
        BL_snpparamtype_Z,
    } BL_snpparamtype_t;
    #define BL_SNPPARAMTYPE_LABELS { "S", "T", "Y", "Z" }

    #define BL_SNPCOMMENT_HEADER    "!"

    typedef struct {
        BL_snpunit_t unit; // default is GHz
        BL_complexnumberformat_t format; // default is MA
        BL_snpparamtype_t type; // default is S
        double refres; // reference resistance, default is 50ohms
    } BL_snpheaderinfo_t, *pBL_snpheaderinfo_t;

    typedef const BL_snpheaderinfo_t *pcBL_snpheaderinfo_t;

    int BL_snparrayIO_write(FILE* pf, pcBL_snparray_t p, pcBL_snpheaderinfo_t headerinfo);

    int BL_snparrayIO_write1(const char* filepath, pcBL_snparray_t p, pcBL_snpheaderinfo_t headerinfo);

    int BL_snparrayIO_read(FILE* pf, pBL_snparray_t* pp, pBL_snpheaderinfo_t headerinfo);

    int BL_snparrayIO_read1(const char* filepath, pBL_snparray_t* pp, pBL_snpheaderinfo_t headerinfo);


    int BL_snparrayIO_parseHeader(const char* header, pBL_snpheaderinfo_t headerinfo)
#ifdef __cplusplus
}
#endif
#endif /* BL_SNPARRAYIO_H_ */
