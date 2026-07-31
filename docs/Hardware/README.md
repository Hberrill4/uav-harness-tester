## Microcontrollers Considered

**ESP32** — 45 changeable GPIO, 4 SPI, 3 UART, 2 I2C, 240 MHz clock, 8 MB flash, 512 KB SRAM, 384 KB ROM, 3.3 V logic. ([ESP32-S3-DEV-KIT-N8R8 overview](https://www.waveshare.com))

**Arduino Mega** — 16 analog inputs, 54 digital I/O (15 PWM), 4 serial ports, 16 MHz clock, 256 KB flash (8 KB used by bootloader), 8 KB SRAM, 4 KB EEPROM, 3.3 V logic.

The ESP32 is faster with more RAM/flash (good for future/industrial expansion) but its 3.3 V logic complicates inclusion. The Arduino Mega tests matrices one at a time — slightly slower, but makes good use of the SD card.

Finally the ESP32 was preferred as it provided more internal RAM which would allow the 64x64 matrices to be accessed in this way and improve the speed of the system massively as well as ease of use.

## Other Component Considerations
Was tough to find connecting cables that accommodated for a 64 wire harness so a decision was made to split it in half and use 2 idc rainbow ribbon cables inside of the modular test pod as they are already protected in that way and to use 2 DSsub connectors as the floating umbilical cords as these can withstand the daily usage and strain.  
I also looked at the screen and lcd screen being able to share SPI bus ports as this would then require less GPIO pins on the Arduino.  
Another consideration was monitoring the noise and surges that would happen due to there being many MUX switches so ceramic capacitors would be needed to mitigate this.  
The MUX would have to have 4 control pins and 16 input/output pins while being able to operate on the esp32 to allow for switching between 64 wires.  
A rotary encoder was originally looked at to be able to scroll through options but a single push button was decided upon as this would be far more user friendly and be able to easily be mounted on the modular test pod and only require one user input.  
Headers would also be required to connect to the DSub cables off the PCB.

## Final Hardware

**Microcontroller:** ESP32-S3-DEV-KIT-NXRX - 45 changeable GPIO, 4 SPI, 3 UART, 2 I2C, 240 MHz clock, 8 MB flash, 512 KB SRAM, 384 KB ROM, 3.3 V logic.

**Display:** 2.8" ILI9341 SPI TFT — 169 KB internal display RAM, 3.3 V logic, 6 GPIO pins.

**Storage:** MicroSD SPI/SDIO breakout board — shares the SPI bus, needing only one extra pin.

**Multiplexer/Demultiplexer:** CD74HC4067 — 2–6 V operation, 16 channels, 4 control pins, enable pin

**DSub connector:** 37 Pin Dsub to Dsub males

**Test pod connectors:** High Speed Data Transfer Cord DB37 Data Cable Wire D-Sub 37Pin

**Bom is provided below:**  
[Bill of Materials](docs/project-files/BOM%20for%20project.csv)
