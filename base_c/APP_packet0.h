#ifndef APP_PACKET0_H_
#define APP_PACKET0_H_
#include "base_c/BL_packethandler.h"

typedef enum {
    APP_packet0_master,
    APP_packet0_slave
} APP_packet0_mstslv_t; // master/slave classifier

typedef struct {
    APP_packet0_mstslv_t mstslv;
    char* port_name;
    uint8_t myaddress;
} APP_packet0_properties_t, *APP_packet0_properties_t;

typedef const APP_packet0_properties_t *pcAPP_packet0_properties_t;

#endif /* APP_PACKET0_H_ */