/*!
 * @file    serialDriver.cpp
 * @author  meltyMakesCo
 * @date    May, 2023
 * 
 * @brief   Driver to handle sending and receiving serial data.
 * 
 */

#include "serialDriver.h"

/* Message Payload lengths. */
#define SERIAL_DRIVER_PAYLOAD_LEN_FAULT         1   /* No data sent. */
#define SERIAL_DRIVER_PAYLOAD_LEN_ENGINE_DATA   4   /* RPM (2 bytes), Speed, Gear. */

/* Serial Driver Message IDs. */
enum SdFnIds {
    SERIAL_DRIVER_ID_FAULT = 0x0,
    SERIAL_DRIVER_ID_ENGINE_DATA = 0x1,
    SERIAL_DRIVER_ID_MAX,
};


/* Rx Callback. */
void onPacketReceived(const uint8_t* buffer, size_t size);
/* Define Rx functions. */
void rxHandleUnimplemented(const uint8_t* payload, size_t size);

/* Define structure of Rx functions. */
typedef void SdRxFunc(const uint8_t* payload, size_t size);

/* Rx Function Lookup table entry. */
typedef struct SdRxFnEntry {
    SdFnIds id;         /* Function ID. */
    SdRxFunc *func;     /* Pointer to Rx Function to be called. */
} SdRxFnEntry;

/* Rx Function lookup table. */
const SdRxFnEntry rxFunctionLookupTable[SERIAL_DRIVER_ID_MAX] = {
    {SERIAL_DRIVER_ID_FAULT, rxHandleUnimplemented},        /* CAN Sniffer doesn't care if companion faults. */
    {SERIAL_DRIVER_ID_ENGINE_DATA, rxHandleUnimplemented},  /* CAN Sniffer shouldn't be recieving engine data. */
};


/*!
 * @brief Default Constructor.
 */
SerialDriver::SerialDriver(Stream *serialPort) {
    /* Validate serialPort object. */
    if (serialPort == nullptr) {
        Serial.println("Error: Bad Serial pointer!");
        while (1);
    }

    /* Initialize PacketSerial. */
    _packetSerial.setStream(serialPort);
    _packetSerial.setPacketHandler(&onPacketReceived);
}


/*!
 * @brief Checks for Serial messages and triggers the onPacketReceived callback. Call every loop().
 * 
 * @result onPacketReceived called.
 */
Errors SerialDriver::loopReadMsgs() {
    _packetSerial.update();
    
    if (_packetSerial.overflow()) {
        /* Indicate error to companion. Best effort. */
        txSendFault();
    }
}


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
Errors SerialDriver::sendMessage(uint8_t id, const uint8_t* payload, size_t size) {
    const uint8_t* message = nullptr;
    size_t msgSize = 0;

    /* Validate inputs. */
    if (payload == nullptr) {
        return ERROR_FAIL;
    }
    /* Every message should include a payload of at least 1 byte. */
    if (size < 1) {
        return ERROR_FAIL;
    }
    /* Check if the ID byte is valid. */
    if (id >= SERIAL_DRIVER_ID_MAX) {
        return ERROR_FAIL;
    }

    /* Prepend the ID to our payload data by storing in a message buffer. */
    msgSize = ++size;
    message = (const uint8_t*)malloc(msgSize * sizeof(uint8_t));
    memcpy(message, &id, sizeof(id));
    memcpy((message + 1), payload, size*sizeof(uint8_t));

    /* Send the message. */
    _packetSerial.send(message, msgSize);
    
    return ERROR_OK;
}

/*!
 * @brief   Send a message indicating a fault occured.
 *          Given the use case, results are best effort.
 * 
 * @result  If successful a serial message indicating a fault will be sent.
 */
Errors SerialDriver::txSendFault() {
    Errors ret;
    /* Send a fault. */
    ret = sendMessage(SERIAL_DRIVER_ID_FAULT, 0, SERIAL_DRIVER_PAYLOAD_LEN_FAULT);

    return ret;
}

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
Errors SerialDriver::txSendEngineData(int rpm, int speedKph, char gear) {
    Errors ret = ERROR_OK;
    uint8_t payload[SERIAL_DRIVER_PAYLOAD_LEN_ENGINE_DATA] = {};

    /* Cap input data. */
    rpm &= 0xFFFF;      /* RPM only supports 2 bytes. */

    /* Push data to payload. */
    payload[0] = (rpm >> 8) & 0xFF;
    payload[1] = (rpm) & 0xFF;
    payload[2] = (speedKph) & 0xFF;
    payload[3] = (uint8_t)gear;

    /* Send the data. */
    ret = sendMessage(SERIAL_DRIVER_ID_ENGINE_DATA, payload, SERIAL_DRIVER_PAYLOAD_LEN_ENGINE_DATA);

    return ret;
}

/*!
 * @brief   Callback triggered on message received.
 *
 *          Message expected to be sent in the following format:
 *          +------+------------------+
 *          |  ID  |  Payload Byte N  |
 *          +------+------------------+
 *          There must be at least one payload byte.
 * 
 * @param[in]   buffer    Byte array containing the data received.
 * @param[in]   size      Length of the buffer received.
 * 
 * @result  If received message was valid, calls a function from rxFunctionLookupTable based on the message ID.
 */
void onPacketReceived(const uint8_t* buffer, size_t size) {
    SdFnIds id = SERIAL_DRIVER_ID_MAX;
    const uint8_t* payload = nullptr;
    size_t payloadSize = 0;

    /* Validate inputs. */
    if (buffer == nullptr) {
        return;
    }
    /* Every message should include an ID and a payload of at least 1 byte. */
    if (size < 2) {
        return;
    }
    /* Check if the ID byte is valid. */
    id = (SdFnIds)buffer[0];
    if (id >= SERIAL_DRIVER_ID_MAX) {
        return;
    }

    /* ID is valid and payload should exist, extract payload from buffer. */
    payload = buffer + 1;
    payloadSize = --size;

    /* Look through our table for the corresponding function ID. */
    for (int i = 0; i < SERIAL_DRIVER_ID_MAX; i++) {
        /* Check if the IDs match. */
        if(rxFunctionLookupTable[i].id == id) {
            /* Call the function and return. */
            rxFunctionLookupTable[i].func(payload, payloadSize);
            return;
        }
    }

    return;
}



/* 
 * Serial Driver Rx Functions
 */

/*!
 * @brief Does nothing. Used for any actions that aren't implemented.
 *
 * @param[in]   payload   Data payload from the packet.
 * @param[in]   size      Length of the payload array.
 * 
 * @result  Does nothing.
 */
void rxHandleUnimplemented(const uint8_t* payload, size_t size) {
    return;
}
