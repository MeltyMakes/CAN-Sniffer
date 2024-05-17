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

#include <mcp2515.h>                        /* Depends on autowp-mcp2515 library: https://github.com/autowp/arduino-mcp2515 */
#include "carData.h"
#include "errors.h"

#define MCP2515_CS          10              /* Chip Select connected to the MCP2515 chip. */


/* //todo doxygen */
class Mcp2515Driver {
public:    
    Mcp2515Driver(CarData *data);
    // ~Mcp2515Driver(); //todo implement


    Errors loopReadMsgs();

private:
    MCP2515 _canNode = MCP2515(MCP2515_CS);
    CarData *_data;


    Errors processMsg(canid_t msgId, unsigned char *msgData);
};


#endif