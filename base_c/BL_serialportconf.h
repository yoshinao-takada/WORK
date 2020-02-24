#ifndef BL_SERIALPORTCONF_H_
#define BL_SERIALPORTCONF_H_
#include <base_l/BL_base.h>
#include <base_l/BL_types.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    BL_parity_none,
    BL_parity_even,
    BL_parity_odd,
} BL_parity_t;

typedef enum {
    BL_stopbits_1,
    BL_stopbits_1p5,
    BL_stopbits_2,
} BL_stopbits_t;

typedef enum {
    BL_networkrole_server,
    BL_networkrole_client,
    BL_networkrole_master,
    BL_networkrole_slave,
    BL_networkrole_repeater,
} BL_networkrole_t;

typedef struct {
    uint32_t baudrate;
    BL_parity_t parity;
    BL_stopbits_t stopbits;
    BL_networkrole_t role;
    uint16_t address;
} BL_serialportconf_t, *pBL_serialportconf_t;

typedef const BL_serialportconf_t *pcBL_serialportconf_t;

typedef struct {
    BL_serialportconf_t base;
    char name[0]; // device name like /dev/ttyS0
} BL_serialportconf_and_name_t, *pBL_serialportconf_and_name_t;

typedef const BL_serialportconf_and_name_t *pcBL_serialportconf_and_name_t;

#define BL_SERIALPORTCONF_DEFAULT   { \
    9600, BL_parity_none, BL_stopbits_1, BL_networkrole_slave, 0x80 \
}

#define BL_SERIALNAME_DEFAULT0  "/dev/ttyUSB0"
#define BL_SERIALNAME_DEFAULT1  "/dev/ttyUSB1"

#define BL_JSONTAG_BAUDRATE     "baudrate"
#define BL_JSONTAG_PARITY       "parity"
#define BL_JSONTAG_STOPBITS     "stopbits"
#define BL_JSONTAG_ROLE         "role"
#define BL_JSONTAG_ADDRESS      "address"
#define BL_JSONTAG_DEVICENAME   "devicename"

const char* BL_serialportconf_tojson(pcBL_serialportconf_and_name_t conf);

int BL_serialportconf_fromjson(const char* src_string, pBL_serialportconf_and_name_t* ppconf);

/*!
\brief create a memory block with the name buffer which can hold a name string up to max_name_len characters
    with null termination; i.e. the total buffer length is max_name_len + 1.
\param max_name_len [in] name buffer size counted in bytes. Actual buffer size is it + 1. +1 is for
    null termination.
\return the allocated pointer. nullptr if available memory is insufficient.
*/
pBL_serialportconf_and_name_t BL_serialportconf_and_name_new(uint32_t max_name_len);

/*!
\brief create a memory block with a device name of initial_name.
\param initial_name [in] initial device name. name member is initialized with the length of initial_name + 1.
\return the allocated pointer. nullptr if available memory is insufficient.
*/
pBL_serialportconf_and_name_t BL_serialportconf_and_name_new2(const char* initial_name);
#ifdef __cplusplus
}
#endif
#endif /* BL_SERIALPORTCONF_H_ */