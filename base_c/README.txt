BL_crc.h : CRC generator/error checker
BL_serial.h : synchronous/asynchrnonous serialport wrapper
BL_packet.h : packet utilities for serialport/RS-485 mini network
BL_mininet.h : serialport/RS-485 mini network
BL_sockettcp.h : socket client/server sample code
BL_socketudp.h : socket client/server sample code
BL_ringbuffer.h : ring buffer

BL_packet.h

1. paccket header
[RA][SA][PT][POPT][CRC0][CRC1]
RA: recipient address
SA: sender address
PT: packet type 
    bit 7==0: no-payload, bit6..0: subtype ID
    bit7==1, bit6==0: with payload and not to be continued. bit 5..0: subtype ID
    bit7==1, bit6==1: with payload and to be continued. bit 5..0: subtype ID
POPT: packet option (single byte parameter or payload size, depending on packet type)
    THe payload size specifies the byte count of data part excluding CRC in the payload.
CRC0,1: CCITT CRC16, little endian

2. payload
[data...][CRC0][CRC1]
data: arbitary byte sequence (1..255 bytes, specified in POPT)
CRC0,CRC1: CCITT CRC16, little endian

3. packet description
Table: list of packet type
Type grp        Type name       Description
no-payload      IFRES           Interface reset
                DEVIFRES        Device interface reset
                DEVRES          Device reset
                SYSRES          System reset
                TIMESYN         timing synchronization
                SRQ             service request
                ACK             acknowledge
                MSTBUSREL       master bus release
                MSTBUSOCC       master bus occupy
                DEVDESC         device description request
                DEVSTAT         device status request
with-payload    APPCMD          application command
                APPREPLY        application reply to command
                DEVDESCREPLY    device description reply
                DEVSTATREPLY    device status reply
                CONTINUE               


4. BL_packet test strategy
4.1. BL_packet_header_set()
    (1) setting no-payload type
        step1: BL_packet_header_set()
        step2: BL_packet_header_check_CRC()
        step3: modify RA
        step4: BL_packet_header_check_CRC() detects an error.
        step5: restore RA
        step6: modify SA
        step7: BL_packet_header_check_CRC() detects an error.
        step8: restore SA
        step9: BL_packet_header_check_CRC() detects no error
    (2) setting with-payload type
        step1: BL_packet_header_set()
        step2: BL_packet_header_check_CRC()
        step3: modify RA
        step4: BL_packet_header_check_CRC() detects an error.
        step5: restore RA
        step6: modify SA
        step7: BL_packet_header_check_CRC() detects an error.
        step8: restore SA
        step9: BL_packet_header_check_CRC() detects no error
4.2. BL_packet_payload_set_CRC()
        step1: define payload size
        step2: init a packet buffer with the payload size setting by
            BL_packet_alloc()
            BL_packet_header_set()
        step3: fill payload with random numbers
        step4: BL_packet_payload_set_CRC()
        step5: BL_packet_payload_check_CRC()
        step6: modify a bit in the payload
        step7: BL_packet_payload_check_CRC() detects an error
        step8: restore the modified bit
        step9: BL_packet_payload_check_CRC() detects no error
