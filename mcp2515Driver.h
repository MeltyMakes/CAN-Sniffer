/*!
 * @file    mcp2515Driver.h 
 * @author  meltyMakesCo
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
    /*!
     * @brief Default Constructor.
     */
    Mcp2515Driver(CarData *data);

    /*!
     * @brief Checks for CAN messages and stores the result if any was received. Call every loop().
     * 
     * @result Any relevant CAN data should be stored in the _data member of the class.
     */
    Errors loopReadMsgs();

private:
    MCP2515 _canNode = MCP2515(MCP2515_CS);
    CarData *_data;

    /*! 
    * @brief   Takes a given CAN message's contents and stores the data based on the message ID.
    * 
    * @param[in]    msgId       ID of the message corresponding to the data.
    * @param[in]    msgData     Pointer to a buffer containing the data from the message.
    * 
    * @result  Will set/update appropriate flags/variables for the message ID.
    *          On fail will just return without updating anything.
    */
    void processMsg(canid_t msgId, uint8_t *msgData);
};


#endif