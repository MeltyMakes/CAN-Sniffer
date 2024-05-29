#include "mcp2515Driver.h"
#include "serialDriver.h"

Mcp2515Driver* mcpDriver;
SerialDriver*  serialDriver;

CarData data;

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

    Serial.println("End setup.");
}

void loop() {
    /* Loop through tasks. */

    /* Check for messages. */
    mcpDriver->loopReadMsgs();
    serialDriver->loopReadMsgs();

    /* Report the message through serial. */
    //todo implement



}