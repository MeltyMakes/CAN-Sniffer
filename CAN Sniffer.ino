#include <mcp2515.h>
#include "mcp2515Driver.h"

char msgString[128];                        // Array to store serial string

Mcp2515Driver *mcpDriver;
// Mcp2515DriverCanMessage canMsg = {};
bool ret;
CarData data;

void setup() {
    // delay(500); // reduces Serial issues
    Serial.begin(115200);
    while (!Serial) {
      ;
    }
    
    Serial.println("Program started.");

    mcpDriver = new Mcp2515Driver(&data);
    ret = false;

    Serial.println("End setup.");
}

void loop() {
    mcpDriver->readMsg();
    // ret = false;
    // canMsg = {};

    // /* Loop through tasks. */

    // /* Check for messages. */
    // ret = mcpDriver->readMsg(&canMsg);

    // switch (canMsg.rxId)
    // {
    // case CAN_MSG_ENGINE_DATA:
    //   speed = (canMsg.data[0] << 8) + canMsg.data[1];   // XMISSION_SPEED
    //   rpm = (canMsg.data[2] << 8) + canMsg.data[3];     // ENGINE_RPM
      
    //   break;

    // case CAN_MSG_POWERTRAIN_DATA:
    //   rpm2 = (canMsg.data[2] << 8) + canMsg.data[3];    // ENGINE_RPM
      
    //   break;

    // case CAN_MSG_GEARBOX:
    //   gear = canMsg.data[0];
    //   gear = gear & 0b00111111;             // GEAR_SHIFTER
    //   switch (gear)
    //   {
    //   case 0x20:
    //     gear = 'L';                         // Low Gear
    //     break;
      
    //   case 0x10:
    //     gear = 'S';                         // Sport
    //     break;

    //   case 0x8:
    //     gear = 'D';                         // Drive
    //     break;

    //   case 0x4:
    //     gear = 'N';                         // Neutral
    //     break;

    //   case 0x2:
    //     gear = 'R';                         // Reverse
    //     break;

    //   case 0x1:
    //     gear = 'P';                         // Park
    //     break;
      
    //   default:
    //     break;
    //   }

    //   break;
    
    // default:
    //   break;
    // }


    // sprintf(msgString, "Gear: %c,\t RPM2: %d,\t Speed %d,\t %d,\t %d,\t %d", gear, rpm2, speed);

    // Serial.print(msgString);
    // Serial.println();
  
    // if (ret == false) {
    //   delay(500);
    //   Serial.println("F");
    //   return;
    // }

    // /* Report the message through serial. */
    // mcpManager.printMsg(canMsg);

}