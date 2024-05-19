#include "mcp2515Driver.h"
#include "serialDriver.h"
char msgString[128];                        // Array to store serial string

Mcp2515Driver* mcpDriver;
SerialDriver*  serialDriver;
// Mcp2515DriverCanMessage canMsg = {};
bool ret;
CarData data;

const uint8_t test[3] = {0x1, 0x0, 0x2};

void setup() {
    Serial.begin(115200);
    while (!Serial) {
      ;
    }
    Serial.println("Program started.");

    /* Initialize drivers. */
    mcpDriver = new Mcp2515Driver(&data);
    Serial.println("MCP Setup Complete.");
    serialDriver = new SerialDriver(&Serial);
    Serial.println("SerialDriver Setup Complete.");

    // serialDriver->sendMessage(0xF,test,3);

    Serial.println("End setup.");
}

void loop() {
    /* Loop through tasks. */

    /* Check for messages. */
    // mcpDriver->loopReadMsgs();
    serialDriver->loopReadMsgs();

    /* Report the message through serial. */
    // mcpManager.printMsg(canMsg); //todo implement



}