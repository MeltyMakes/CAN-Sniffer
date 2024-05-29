# CAN Sniffer

*CAN Sniffer* is a MCP-2515 based Arduino firmware that extracts useful CAN data from an OBD-II port.

Currently only tested on 10th Generation Honda Civics (FC/FK).

## Requirements

### Hardware

The only external hardware required for this is a CAN controller. This firmware is built for a [MCP2515](https://www.microchip.com/en-us/product/MCP2515) controller.

Although you can use regular wires to connect to the OBD-II port of the car, it's recommended to purchase an OBD-II extension/splitter cable for easier access.

### Software

Firmware can be built using the [Arduino IDE](https://www.arduino.cc/en/main/software) and the following libraries:
- [Packet Serial](https://github.com/bakercp/PacketSerial) (Version 1.4.0)
- [autowp-mcp2515](https://github.com/autowp/arduino-mcp2515) (Version 1.03)

## Getting Started

### Building

Install the [required libraries](###Software) using the library manager. Afterwards connect the board and upload it.

### Wiring

TBD