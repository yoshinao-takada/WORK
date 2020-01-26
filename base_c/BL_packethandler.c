#include "base_c/BL_packethandler.h"
#if defined(__linux__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#else
#endif

#define CB_PAYLOAD      256
pBL_packetwriter_t BL_packetwriter_new()
{
    pBL_packetwriter_t p = (pBL_packetwriter_t)calloc(sizeof(BL_packetwriter_t), 1);
    p->reader = NULL;
    p->data = BL_packet_alloc(CB_PAYLOAD);
    BL_packet_header_set(&(p->data->header), 255, 0, BLPT_APPCMD, CB_PAYLOAD);
}

pBL_packetreader_t BL_packetreader_new()
{
    pBL_packetreader_t p = (pBL_packetreader_t)calloc(sizeof(BL_packetwriter_t), 1);
    p->data = BL_packet_alloc(CB_PAYLOAD);
    BL_packet_header_set(&(p->data->header), 255, 0, BLPT_APPCMD, CB_PAYLOAD);
    p->port.fd_port = -1;
    p->port.myaddress = 0;
    p->port.name_port = NULL;
}

void BL_packetwriter_delete(pBL_packetwriter_t* ppwriter)
{
    free((*ppwriter)->data);
    free(*ppwriter);
    *ppwriter = NULL;
}

void BL_packetreader_delete(pBL_packetreader_t* ppreader)
{
    free((*ppreader)->data);
    free(*ppreader);
    *ppreader = NULL;
}

int BL_packethandler_port_open(pBL_packethandler_port_t port, const char* filepath, uint8_t myaddress)
{
    int err = ESUCCESS;
    do {
        port->fd_port = open(filepath, O_RDWR | O_NOCTTY | O_NDELAY);
        if (port->fd_port == -1)
        {
            break;
        }
        port->name_port = strdup(filepath);
        port->myaddress = myaddress;
    } while (0);
    return err;
}

int BL_packethandler_port_close(pBL_packethandler_port_t port)
{
    int err = ESUCCESS;
    do {
        if (port->fd_port == -1)
        {
            err = ENOTCONN;
            break;
        }
        if (-1 == close(port->fd_port))
        {
            err = errno;
            break;
        }
        port->fd_port = -1;
        free(port->name_port);
        port->name_port = NULL;
        port->myaddress = 0;
    } while (0);
    return err;
}

void BL_packetreader_attach(pBL_packetreader_t reader, pBL_packetwriter_t writer)
{
    writer->reader = reader;
}

int BL_packetwriter_write(pBL_packetwriter_t writer)
{
    int err = ESUCCESS;
    do {
        size_t cb_payload = BL_packet_payload_size(&(writer->data->header));
        if (cb_payload != 0) 
        {
            cb_payload += sizeof(uint16_t); // add CRC length
        }
        size_t cb_write = sizeof(BL_packet_header_t) + cb_payload;
        ssize_t write_result = write(writer->reader->port.fd_port, (const void*)(writer->data), cb_write);
        if (write_result == -1)
        {
            err = errno;
            break;
        }
        if (cb_write > write_result )
        {
            err = ENOSPC;
            printf("%s,%d,(write_result/cb_write) = (%d/%d)\n",
                __FUNCTION__, __LINE__, write_result, cb_write);
        }
    } while (0);
    return err;
}
