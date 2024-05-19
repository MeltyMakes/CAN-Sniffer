/*!
 * @file    serialDriver.h 
 * @author  meltyMakes
 * @date    December, 2023
 * 
 * @brief   Driver to handle sending serial data in a non-blocking way.
 * 
 */

#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include <PacketSerial.h>                   /* Depends on PacketSerial library: https://github.com/bakercp/PacketSerial */
#include "errors.h"

/* //todo doxygen */
class SerialDriver {
public:    
    SerialDriver(Stream *serialPort);

    Errors loopReadMsgs();

    Errors sendMessage(uint8_t id, const uint8_t* payload, size_t size);
private:
    PacketSerial _packetSerial;
    Stream *_serialP;

};

#endif