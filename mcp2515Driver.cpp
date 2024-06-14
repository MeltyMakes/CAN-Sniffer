/*!
 * @file    mcp2515Driver.cpp
 * @author  meltyHandsCo
 * @date    December, 2023
 * 
 * @brief   Driver to process outputs from an MCP2515 Shield.
 * 
 */

#include "mcp2515Driver.h"

/* MCP2515 Definitions. */
#define MCP2515_BAUDRATE    CAN_500KBPS     /* Baudrate of the MCP2515 chip. */
#define MCP2515_CLOCK_HZ    MCP_8MHZ        /* Run Chip at 8MHz as the onboard source is 8MHz. */
char str[128];

#define CAN_MSG_ENGINE_DATA 0x158
#define CAN_MSG_POWERTRAIN_DATA 0x17C
#define CAN_SIG_RPM_OFFSET 16               /* Bits. */
#define CAN_SIG_RPM_LEN 16                  /* Bits. */

#define CAN_MSG_CAR_SPEED 0x309
#define CAN_SIG_CAR_SPEED_OFFSET 0          /* Bits. */
#define CAN_SIG_CAR_SPEED_LEN 16            /* Bits. */

#define CAN_MSG_GEARBOX 0x191

#define CAN_MSG_SCM_FEEDBACK 0x326 // has r/l blinker

/*!
 * @brief Default Constructor.
 */
Mcp2515Driver::Mcp2515Driver(CarData *data) {
    int mcpRet = MCP2515::ERROR_OK;

    /* Validate CarData struct. */
    if (data == nullptr) {
        Serial.print("Error: Bad CarData pointer!");
        while (1);
    }
    this->_data = data;

    /* Autowp library requires MCP to be reset before changing parameters. */
    _canNode.reset();

    /* Set MCP2515 parameters. */
    mcpRet = _canNode.setBitrate(MCP2515_BAUDRATE, MCP2515_CLOCK_HZ);
    if (mcpRet != MCP2515::ERROR_OK) {
        Serial.print("Error setBitrate: ");
        Serial.println(mcpRet);

        goto err;
    }

    /* Set listen mode. */
    mcpRet = _canNode.setListenOnlyMode();
    if (mcpRet != MCP2515::ERROR_OK) {
        Serial.print("Error SetListenOnlyMode: ");
        Serial.println(mcpRet);

      goto err;
    }

    Serial.println("MCP Setup Complete.");

    return;

err:
    /* Best effort. */
    _canNode.reset();
    _canNode.setSleepMode();
    while (1);
}

/*!
 * @brief Checks for CAN messages and stores the result if any was received. Call every loop().
 * 
 * @result Any relevant CAN data should be stored in the _data member of the class.
 */
Errors Mcp2515Driver::loopReadMsgs() {
    Errors driverRet = ERROR_FAIL;
    int mcpRet = MCP2515::ERROR_OK;
    struct can_frame canMsg;

    /* Clear the CAN message container. */
    canMsg = {};

    /* Check for messages. */
    mcpRet = _canNode.readMessage(&canMsg);
    if(mcpRet == MCP2515::ERROR_NOMSG) {
        /* If no message, do nothing. */
        return ERROR_OK;
    }
    else if (mcpRet != MCP2515::ERROR_OK) {
        /* If not successful, error out. */
        Serial.println("ReadFail.");

        return ERROR_FAIL;
    }
    
    /* Filter based on our message ID. */
    processMsg(canMsg.can_id, canMsg.data);

    /* If message read successfully. */
    return ERROR_OK;
}

/*! 
 * @brief   Takes a given CAN message's contents and stores the data based on the message ID.
 * 
 * @param[in]   msgId       ID of the message corresponding to the data.
 * @param[in]   msgData     Pointer to a buffer containing the data from the message.
 * 
 * @result  Will set/update appropriate flags/variables for the message ID.
 *          On fail will just return without updating anything.
 */
void Mcp2515Driver::processMsg(canid_t msgId, uint8_t *msgData) {
    /* Handle data based on ID. */
    switch (msgId)
    {
    case CAN_MSG_ENGINE_DATA:
        _data->speedKph = (msgData[0] << 8) + msgData[1];   // XMISSION_SPEED
        break;

    case CAN_MSG_POWERTRAIN_DATA:
        _data->rpm = (msgData[2] << 8) + msgData[3];    // ENGINE_RPM
        break;

    case CAN_MSG_SCM_FEEDBACK:
        //todo R/L Blinker
        break;

    case CAN_MSG_GEARBOX:
        char gear = 'X';
        gear = msgData[0];
        gear = gear & 0b00111111;             // GEAR_SHIFTER
        switch (gear)
        {
        case 0x20:
            gear = 'L';                         // Low Gear
            break;

        case 0x10:
            gear = 'S';                         // Sport
            break;

        case 0x8:
            gear = 'D';                         // Drive
            break;

        case 0x4:
            gear = 'N';                         // Neutral
            break;

        case 0x2:
            gear = 'R';                         // Reverse
            break;

        case 0x1:
            gear = 'P';                         // Park
            break;

        default:
            break;
        }
        _data->gear = gear;
        break;

    default:
        /* No need to do anything on bad data. */
        return;
    }

    sprintf(str, "Gear: %c,\t RPM2: %d,\t Speed %d", _data->gear, _data->rpm, _data->speedKph);

    Serial.print(str);
    Serial.println();

    return;
}