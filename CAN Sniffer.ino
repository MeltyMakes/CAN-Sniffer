#include <mcp_can.h>
#include <SPI.h>
#include "mcp2515Driver.h"

#define DEBUG_MODE 0

#define CAN_MSG_ENGINE_DATA 0x158
#define CAN_MSG_POWERTRAIN_DATA 0x17C
#define CAN_SIG_RPM_OFFSET 16         //bits
#define CAN_SIG_RPM_LEN 16         //bits

#define CAN_MSG_CAR_SPEED 0x309
#define CAN_SIG_CAR_SPEED_OFFSET 0         //bits
#define CAN_SIG_CAR_SPEED_LEN 16         //bits

#define CAN_MSG_GEARBOX 0x191

#define CAN_MSG_SCM_FEEDBACK 0x326 // has r/l blinker

int rpm = 0;
int rpm2 = 0;
int speed = 0;
char gear = 'O';
char msgString[128];                        // Array to store serial string

Mcp2515Driver *mcpDriver;
Mcp2515DriverCanMessage canMsg = {};
bool ret;

void setup() {
    // delay(500); // reduces Serial issues
    Serial.begin(115200);
    while (!Serial)
    {
      ;
    }
    
    Serial.print("Program started.");

    mcpDriver = new Mcp2515Driver();
    pinMode(2, INPUT);
    ret = false;
    canMsg = {};

    Serial.println("End setup.");
}

void loop() {
    // ret = false;
    // canMsg = {};

    // /* Loop through tasks. */

    // /* Check for messages. */
    ret = mcpDriver->readMsg(&canMsg);

    switch (canMsg.rxId)
    {
    case CAN_MSG_ENGINE_DATA:
      speed = (canMsg.data[0] << 8) + canMsg.data[1];   // XMISSION_SPEED
      rpm = (canMsg.data[2] << 8) + canMsg.data[3];     // ENGINE_RPM
      
      break;

    case CAN_MSG_POWERTRAIN_DATA:
      rpm2 = (canMsg.data[2] << 8) + canMsg.data[3];    // ENGINE_RPM
      
      break;

    case CAN_MSG_GEARBOX:
      gear = canMsg.data[0];
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

      break;
    
    default:
      break;
    }


    sprintf(msgString, "Gear: %c,\t RPM2: %d,\t Speed %d,\t %d,\t %d,\t %d", gear, rpm2, speed);

    Serial.print(msgString);
    Serial.println();
  
    // if (ret == false) {
    //   delay(500);
    //   Serial.println("F");
    //   return;
    // }

    // /* Report the message through serial. */
    // mcpManager.printMsg(canMsg);

}