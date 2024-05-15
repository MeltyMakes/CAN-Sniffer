//todo doxygen

#include "mcp2515Driver.h"

/* MCP2515 Definitions. */
#define MCP2515_BAUDRATE    CAN_500KBPS     /* Baudrate of the MCP2515 chip. */
#define MCP2515_CLOCK_HZ    MCP_8MHZ        /* Run Chip at 8MHz as the onboard source is 8MHz. */
#define MCP2515_MASK        MCP_STDEXT      /* Mask for Standard and Extended IDs. */
#define MCP2515_MODE        MCP_LISTENONLY  /* Listen on the bus, do not transmit any messages or ACK. */
#define MCP2515_INT         2               /* Interrupt pin connected to the MCP2515 chip. */
char str[128];

#define CAN_MSG_ENGINE_DATA 0x158
#define CAN_MSG_POWERTRAIN_DATA 0x17C
#define CAN_SIG_RPM_OFFSET 16         //bits
#define CAN_SIG_RPM_LEN 16         //bits

#define CAN_MSG_CAR_SPEED 0x309
#define CAN_SIG_CAR_SPEED_OFFSET 0         //bits
#define CAN_SIG_CAR_SPEED_LEN 16         //bits

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
        return;
    }
    this->data_ = data;

    /* Autowp library requires MCP to be reset before changing parameters. */
    canNode_.reset();
    
    /* Set MCP2515 parameters. */
    mcpRet = canNode_.setBitrate(MCP2515_BAUDRATE, MCP2515_CLOCK_HZ);
    if (mcpRet != MCP2515::ERROR_OK) {
      Serial.print("Error setBitrate: ");
      Serial.println(mcpRet);

      goto err;
    }

    //todo possibly set mask and filter registers in MCP

    /* Set listen mode. */
    mcpRet = canNode_.setListenOnlyMode();
    if (mcpRet != MCP2515::ERROR_OK) {
      Serial.print("Error SetListenOnlyMode: ");
      Serial.println(mcpRet);

      goto err;
    }

    Serial.println("MCP Setup Complete.");

    return;

err:
    /* Best effort. */
    canNode_.reset();
    canNode_.setSleepMode();
}


/*!
 * @brief Handles message checking.
 */
Errors Mcp2515Driver::readMsg() {
    Errors driverRet = ERROR_FAIL;
    int mcpRet = MCP2515::ERROR_OK;
    struct can_frame canMsg;

    /* Clear the CAN message container. */
    canMsg = {};

    /* Check for messages. */
    mcpRet = canNode_.readMessage(&canMsg);
    if(mcpRet == MCP2515::ERROR_NOMSG) {
        /* If no message, do nothing. */
        // Serial.println("No msgs."); //todo implement a better debug message verbosity system.
        return ERROR_OK;
    }
    else if (mcpRet != MCP2515::ERROR_OK) {
        /* If not successful, error out. */
        Serial.println("ReadFail.");

        return ERROR_FAIL;
    }
    
    /* Filter based on our message ID. */
    driverRet = processMsg(canMsg.can_id, canMsg.data);
    if (driverRet != ERROR_OK) {
      Serial.print("Error processMsg: ");
      Serial.println(mcpRet);

      return ERROR_FAIL;
    }

    /* If message read successfully. */
    return ERROR_OK;
}

Errors Mcp2515Driver::processMsg(canid_t msgId, unsigned char *msgData){
    Errors driverRet = ERROR_OK;

    /* Handle data based on ID. */
    switch (msgId)
    {
      case CAN_MSG_ENGINE_DATA:
          data_->speedKph = (msgData[0] << 8) + msgData[1];   // XMISSION_SPEED
          break;

      case CAN_MSG_POWERTRAIN_DATA:
          data_->rpm = (msgData[2] << 8) + msgData[3];    // ENGINE_RPM
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
          data_->gear = gear;
          break;

      default:
        /* No need to do anything on bad data. */
        return ERROR_OK;
    }


    sprintf(str, "Gear: %c,\t RPM2: %d,\t Speed %d", data_->gear, data_->rpm, data_->speedKph);

    Serial.print(str);
    Serial.println();

    return ERROR_OK;
}