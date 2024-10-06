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

    /* Read sensor data. */
    // In the future, the sniffer will be extendable with other sensors. Data will be read here.

    /* Report data through serial. */
    serialDriver->txSendEngineData(data.rpm, data.speedKph, data.gear);

}