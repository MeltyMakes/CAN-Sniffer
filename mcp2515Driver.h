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

#include <mcp2515.h>
#include "carData.h"
#include "errors.h"

#define MCP2515_CS          10              /* Chip Select connected to the MCP2515 chip. */


/* //todo doxygen */
class Mcp2515Driver {
public:    
    Mcp2515Driver(CarData *data);
    // ~Mcp2515Driver(); //todo implement


    Errors readMsg();

private:
    MCP2515 canNode_ = MCP2515(MCP2515_CS);
    CarData *data_;


    Errors processMsg(canid_t msgId, unsigned char *msgData);
};


#endif