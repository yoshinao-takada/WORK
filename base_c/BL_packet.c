#include "base_c/BL_packet.h"
#include "base_c/BL_crc.h"
#include <errno.h>
#include <stdlib.h>


void BL_packet_header_set(pBL_packet_header_t ph, uint8_t ra, uint8_t sa, uint8_t pt, uint8_t opt)
{
    ph->e.RA = ra;
    ph->e.SA = sa;
    ph->e.PT = pt;
    ph->e.POPT = opt;
    BL_crc_t crc;
    BL_crc_init(&crc, CRC_CCITT_LE, 0);
    BL_crc_puts(&crc, (uint8_t*)ph, 4);
    ph->e.crc = crc.crc.u16[0];
}

void BL_packet_header_set_option(pBL_packet_header_t ph, uint8_t opt)
{
    ph->e.POPT = opt;
    BL_crc_t crc;
    BL_crc_init(&crc, CRC_CCITT_LE, 0);
    BL_crc_puts(&crc, (uint8_t*)ph, 4);
    ph->e.crc = crc.crc.u16[0];
}

int BL_packet_header_check_CRC(pcBL_packet_header_t ph)
{
    BL_crc_t crc;
    BL_crc_init(&crc, CRC_CCITT_LE, 0);
    BL_crc_puts(&crc, (const uint8_t*)ph, 6);
    return crc.crc.u16[0] == 0 ? ESUCCESS : EINVAL;
}

int BL_packet_payload_set_CRC(pcBL_packet_header_t ph, uint8_t* pl)
{
    if ((0x80 & ph->e.PT) == 0)
    {
        return EINVAL;
    }
    uint32_t payload_length = (uint32_t)(ph->e.POPT);
    BL_crc_t crc;
    BL_crc_init(&crc, CRC_CCITT_LE, 0);
    BL_crc_puts(&crc, pl, payload_length);
    *(uint16_t*)(pl + payload_length) = crc.crc.u16[0];
    return ESUCCESS;
}

int BL_packet_payload_check_CRC(pcBL_packet_header_t ph, const uint8_t* pl)
{
    if ((0x80 & ph->e.PT) == 0)
    {
        return EINVAL;
    }
    uint32_t payload_length = (uint32_t)(ph->e.POPT);
    BL_crc_t crc;
    BL_crc_init(&crc, CRC_CCITT_LE, 0);
    BL_crc_puts(&crc, pl, payload_length + 2);
    return (crc.crc.u16[0] == 0) ? ESUCCESS : EINVAL;
}

pBL_packet_header_t BL_Packet_create_buffer(uint8_t max_payload_size)
{
    uint32_t cballoc = ((uint32_t)max_payload_size + sizeof(BL_packet_header_t) + 2);
    return (pBL_packet_header_t)calloc(cballoc, 1);
}