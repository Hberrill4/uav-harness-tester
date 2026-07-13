# Design PCB

## Component List

- ESP32-S3
- CD74HC4067 (×8)
- 2.8" ILI9341 SPI TFT
- Adafruit MicroSD SPI
- 64-pin DIN 41612 connector
- Voltage regulator
- Protection resistors
- Decoupling capacitors
- Push button

## PCB Requirements

- Test 64 harness wires
- Store golden sample in RAM/SD card
- Log all test results with timestamps
- Display results on the 2.8" TFT LCD
- Operate from a single push button
- Support administrator mode
- Interface with modular test pods
- Use floating interconnects through an umbilical cable
- 5 V input, 3.3 V logic, SPI communication, multiplexer switching
- Protection on ESP32 inputs

## Construction Plan

1. Draw schematic — measurement circuit, power flags, net labels, test points, decoupling capacitors
2. Run ERC and fix until clean
3. Assign footprints and check against datasheets
4. Import schematic into PCB editor
5. Arrange components — wider power traces, digital/analog separation
6. Route PCB traces
7. Add ground plane
8. Run DRC
9. Inspect in 3D
10. Review against schematic

**Make a prototype first.**
