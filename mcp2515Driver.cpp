//todo doxygen

#include "mcp2515Driver.h"

/* MCP2515 Definitions. */
#define MCP2515_BITRATE CAN_500KBPS     /* Bitrate of the MCP2515 chip. */
#define MCP2515_SPEED   MCP_8MHZ        /* Run Chip at 8MHz. */
#define MCP2515_MASK    MCP_STDEXT      /* Mask for Standard and Extended IDs. */
#define MCP2515_MODE    MCP_LISTENONLY  /* Listen on the bus, do not transmit any messages or ACK. */
#define MCP2515_CS      10              /* Chip Select connected to the MCP2515 chip. */
#define MCP2515_INT     2               /* Interrupt pin connected to the MCP2515 chip. */
char str[128];
/*!
 * @brief Default Constructor.
 */
Mcp2515Driver::Mcp2515Driver() {
    Errors driverRet = ERROR_FAIL;
    INT8U mcpRet = CAN_FAIL;

    /* Create canNode object. */
    if (canNode != NULL) {
        Serial.print("Error: canNode already exists!");
        goto err;
    }
    canNode = new MCP_CAN(MCP2515_CS);

    /* Initialize MCP2515 with parameters. */
    mcpRet = canNode->begin(MCP2515_MASK, MCP2515_BITRATE, MCP_SLEEP);
    if (mcpRet != CAN_OK) {
      Serial.print("Error: ");
      Serial.println(mcpRet);

      goto err;
    }
    /* Set mode. */
    canNode->setMode(MCP2515_MODE);

err:
    /* Best effort. */
    delete canNode;
    canNode = NULL;
}


/*!
 * @brief Handles message checking.
 */
Errors Mcp2515Driver::readMsg(Mcp2515DriverCanMessage *canMsg) {
    Errors driverRet = ERROR_FAIL;
    INT8U mcpRet = CAN_FAIL;

    /* Clear the CAN message container. */
    canMsg = {};

    if(digitalRead(MCP2515_INT) != LOW) {
        /* If interrupt pin wasn't triggered, do nothing. */
        return ERROR_OK;
    }

    /* Read a message. */
    mcpRet = canNode->readMsgBuf(&canMsg->rxId, &canMsg->len, canMsg->data);
    
    sprintf(str, "RxID %d Len %d, Buf %d", canMsg->rxId, canMsg->len, canMsg->data[0]);
    Serial.print(str); //todo temp
    if (mcpRet != CAN_OK) {
        return ERROR_FAIL;
    }

    /* If message read successfully. */
    return ERROR_OK;  
}

// /*!
//  * @brief Print message.
//  */
// Errors Mcp2515Driver::printMsg(Mcp2515DriverCanMessage canMsg) {
//     Serial.print(canMsg.can_id, HEX);
//     Serial.print(","); 
//     Serial.print(canMsg.can_dlc, HEX);
//     Serial.print(",");
    
//     for (int i = 0; i<canMsg.can_dlc; i++)  {
//       Serial.print(canMsg.data[i],HEX);
//       Serial.print(",");
//     }

//     Serial.println();
// }