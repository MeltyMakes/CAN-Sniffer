/*!
 * @file    mcp2515Driver.h 
 * @author  meltyMakes
 * @date    December, 2023
 * 
 * @brief   Driver to process outputs from an MCP2515 Shield.
 * 
 */

#ifndef MCP2515_DRIVER_H
#define MCP2515_DRIVER_H

#include <mcp_can.h>
#include "errors.h"

#define CS_PIN 10;

typedef struct Mcp2515DriverCanMessage
{
    unsigned long int rxId;          /* CAN Message ID. */
    unsigned char len;          /* Length of data. */
    unsigned char data[8];      /* MCP2515 only supports CAN 2.0, which has a data max of 8 bytes. */
} Mcp2515DriverCanMessage;


/* //todo doxygen */
class Mcp2515Driver {
public:
    Mcp2515Driver();
    ~Mcp2515Driver();

    Errors readMsg(Mcp2515DriverCanMessage *msg);
    Errors printMsg(Mcp2515DriverCanMessage msg);


private:
    MCP_CAN *canNode;
};


#endif