#ifndef BL_PACKETHANDLER_H_
#define BL_PACKETHANDLER_H_
/*
Packet reader/writer for multi MCU network using RS-485 8bit/word communication.
The library does not support 9bit communication using address bit.
The library is an experimental implementation of master/slave communication able to handle
interrupt requests from slaves.
*/
#include "base_c/BL_packet.h"
#include "base_l/BL_fsm.h"
#include "base_c/BL_ringbuffer.h"
#include "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region data_type_definitions
    typedef struct 
    {
        int fd_port; /* file descriptor of a bound serialport */
        char* name_port; /* filename of a bound serialport */
        uint8_t myaddress; /* host address in an RS-485 network */
    } BL_packethandler_port_t, *pBL_packethandler_port_t;

    typedef struct 
    {
        BL_packethandler_port_t port; /* serialport I/F object */
        pBL_packet_t data; /* buffer for packet header and payload */
        BL_fsm_t fsm; /* statemachine controlling reading sequence */
    } BL_packetreader_t, *pBL_packetreader_t;
    
    typedef struct
    {
        pBL_packetreader_t reader; /* packet reader object */
        pBL_packet_t data; /* buffer for packet header and payload */
    } BL_packetwriter_t, *pBL_packetwriter_t;

    /*!
    \brief callback functions at packet arrivals.
    \param packet [in,out] incoming packet data; the handler function can modify it for other processings
    */
    typedef int (*BL_packethandler_t)(void* exparam, pBL_packetwriter_t writer);

    typedef struct {
        BL_packethandler_t handler_nopl[BLPT_END_NOPL];
        void* exparams[BLPT_END_NOPL];
        BL_packethandler_t handler_pl[BLPT_END_PL];
        void* exparams[BLPT_END_PL];
    } BL_packethandlers_t, *pBL_packethandlers_t;

    typedef const BL_packethandlers_t *pcBL_packethandlers_t;
#pragma endregion
#pragma region function_definitions
    pBL_packetwriter_t BL_packetwriter_new();
    pBL_packetreader_t BL_packetreader_new();
    void BL_packetwriter_delete(pBL_packetwriter_t* ppwriter);
    void BL_packetreader_delete(pBL_packetreader_t* ppreader);

    /*!
    \brief open a serialport for packet communication
    \param port [in,out] packet handler object
    \param filepath [in] serialport filename like "/dev/ttyS0"
    \param myaddress [in] network address
    \return unix errno compatible number
    */
    int BL_packethandler_port_open(pBL_packethandler_port_t port, const char* filepath, uint8_t myaddress);

    /*!
    \brief close a serialport for packet communication
    */
    int BL_packethandler_port_close(pBL_packethandler_port_t port);
    
    /*!
    \brief attach a serialport in a packet handler object to packet reader object.
    \param reader [in,out] packet reader object
    \param handler [in] packet handler object
    */
    void BL_packetreader_attach(pBL_packetreader_t reader, pBL_packetwriter_t writer);

    /*!
    \brief write a packet
    */
    int BL_packetwriter_write(pBL_packetwriter_t writer);

    /*!
    \brief read a packet
    */
    int BL_packetreader_read(pBL_packetreader_t reader);

    /*!
    \brief create a read thread
    \param handlers [in] packet handler functions at packet arrivals
    \return unix errno compatible number
    */
    int BL_packethandler_start(pBL_packetwriter_t writer, pcBL_packethandlers_t handlers);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif /* BL_PACKETHANDLER_H_ */