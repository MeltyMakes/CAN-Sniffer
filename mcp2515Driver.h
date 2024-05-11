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

#define CS_PIN 10;



/* //todo doxygen */
class Mcp2515Driver {
public:    
    Mcp2515Driver(CarData *data);
    ~Mcp2515Driver();


    Errors readMsg();
    // Errors printMsg(Mcp2515DriverCanMessage msg);


private:
    MCP2515 *canNode = nullptr;
    CarData *data;
};


#endif