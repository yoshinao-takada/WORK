#ifndef BL_PACKET_H_
#define BL_PACKET_H_
#include <stdint.h>
#include "base_l/BL_base.h"
#ifdef __cplusplus
extern "C" {
#endif
    typedef union {
        uint8_t bytes[6];
        uint16_t u16[3];
        struct {
            uint8_t RA, SA, PT, POPT;
            uint16_t crc;
        } e;
    } BL_packet_header_t, *pBL_packet_header_t;

    typedef const BL_packet_header_t *pcBL_packet_header_t;

    // get payload size in bytes excluding CRC.
    #define BL_packet_payload_size(ph)  ((ph->e.PT & 0x80) ? (int32_t)(ph->e.POPT) : (int32_t)0)

    #define BL_packet_alloc(cb_payload) (pBL_packet_header_t)calloc(sizeof(BL_packet_header_t) +  cb_payload + 2,1)

// packet type definitions without payload
    #define BLPT(n)     ((uint8_t)n)

    #define BLPT_IFRES      BLPT(0)
    #define BLPT_DEVIFRES   BLPT(1)
    #define BLPT_DEVRES     BLPT(2)
    #define BLPT_SYSRES     BLPT(3)
    #define BLPT_TIMESYN    BLPT(4)
    #define BLPT_SRQ        BLPT(5)
    #define BLPT_ACK        BLPT(6)
    #define BLPT_MSTBUSREL  BPLT(7)
    #define BLPT_MSTBUSOCC  BPLT(8)
    #define BLPT_DEVDESC    BLPT(9)
    #define BLPT_DEVSTAT    BLPT(10)
    
    // packet type definitions with payload
    #define BLPT2(n)    ((uint8_t)(0x80 | n))
    #define BLPT_APPCMD     BLPT2(0)
    #define BLPT_APPREPLY   BLPT2(1)
    #define BLPT_DEVDESCREPLY   BLPT2(2)
    #define BLPT_DEVSTATREPLY   BLPT2(3)
    #define BLPT_CONTINUE   BLPT2(4)

    /*!
    \brief set packet header info
    \param ph [in] packet header pointer
    \param ra [in] recipiend address
    \param sa [in] sender address
    \param pt [in] packet type
    \param opt [in] packet option or payload size depending on 'pt'
    */
    void BL_packet_header_set(pBL_packet_header_t ph, uint8_t ra, uint8_t sa, uint8_t pt, uint8_t opt);

    /*!
    \brief set packet header option
    \param ph [in] packet header pointer
    \param opt [in] packet option or payload size depending on 'pt'
    */
    void BL_packet_header_set_option(pBL_packet_header_t ph, uint8_t opt);

    /*!
    \brief check CRC error
    \param ph [in] packet header pointer
    \return ESUCCESS: no CRC error, EINVAL: CRC error
    */
    int BL_packet_header_check_CRC(pcBL_packet_header_t ph);

    /*!
    \brief set payload CRC
    \param ph packet header pointer
    \param pl payload pointer
    */
    int BL_packet_payload_set_CRC(pcBL_packet_header_t ph, uint8_t* pl);

    /*!
    \brief chek CRC err of payload
    \param ph [in] packet header pointer
    \param pl payload pointer
    \return ESUCCESS: no CRC error, EINVAL: CRC error
    */
    int BL_packet_payload_check_CRC(pcBL_packet_header_t ph, const uint8_t* pl);
#ifdef __cplusplus
}
#endif
#endif /* BL_PACKET_H_ */
