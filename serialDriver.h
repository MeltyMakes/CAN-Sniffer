/*!
 * @file    serialDriver.h 
 * @author  meltyMakesCo
 * @date    May, 2023
 * 
 * @brief   Driver to handle sending and receiving serial data.
 * 
 */

#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include <PacketSerial.h>                   /* Depends on PacketSerial library: https://github.com/bakercp/PacketSerial */
#include "errors.h"

class SerialDriver {
public:    
    /*!
     * @brief Default Constructor.
     */
    SerialDriver(Stream *serialPort);

    /*!
     * @brief Checks for Serial messages and triggers the onPacketReceived callback. Call every loop().
     * 
     * @result onPacketReceived called.
     */
    Errors loopReadMsgs();

    /*!
    * @brief   Sends RPM, Speed, and Gear data over serial.
    *          
    *          Data is sent in the following format:
    *          +-----------+-----------+---------+--------+
    *          |  RPM msb  |  RPM lsb  |  Speed  |  Gear  |
    *          +-----------+-----------+---------+--------+
    * 
    * @param[in]   rpm        Engine RPM.
    * @param[in]   speedKph   Vehicle speed in kilometers per hour.
    * @param[in]   gear       Engine gear.
    * 
    * @return  ERROR_FAIL  Returned if the message was not sent.
    * @return  ERROR_OK    Returned if the message was sent.
    */
    Errors txSendEngineData(int rpm, int speedKph, char gear);

private:
    PacketSerial _packetSerial;

    /*!
     * @brief   Sends a serial message.
     *
     *          Message is sent in the following format:
     *          +------+------------------+
     *          |  ID  |  Payload Byte N  |
     *          +------+------------------+
     *          There must be at least one payload byte.
     * 
     * @param[in]   id        Identifier for the message to be sent.
     * @param[in]   payload   Byte array containing the data to be sent.
     * @param[in]   size      Length of the payload.
     * 
     * @return  ERROR_FAIL  Returned if the inputs were invalid.
     * @return  ERROR_OK    Returned if the message was sent.
     */
    Errors sendMessage(uint8_t id, const uint8_t* payload, size_t size);

    Errors txSendFault();

};

#endif