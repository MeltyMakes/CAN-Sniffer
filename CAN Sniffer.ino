#include <mcp2515.h>
#include "mcp2515Driver.h"

char msgString[128];                        // Array to store serial string

Mcp2515Driver *mcpDriver;
// Mcp2515DriverCanMessage canMsg = {};
bool ret;
CarData data;

void setup() {
    Serial.begin(115200);
    while (!Serial) {
      ;
    }
    Serial.println("Program started.");

    /* Initialize drivers. */
    mcpDriver = new Mcp2515Driver(&data);

    Serial.println("End setup.");
}

void loop() {
    /* Loop through tasks. */

    /* Check for messages. */
    mcpDriver->readMsg();


    /* Report the message through serial. */
    // mcpManager.printMsg(canMsg); //todo implement

}