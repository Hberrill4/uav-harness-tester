# Possible Hardware

## Microcontrollers Considered

**ESP32** — 45 changeable GPIO, 4 SPI, 3 UART, 2 I2C, 240 MHz clock, 8 MB flash, 512 KB SRAM, 384 KB ROM, 3.3 V logic. ([ESP32-S3-DEV-KIT-N8R8 overview](https://www.waveshare.com))

**Arduino Mega** — 16 analog inputs, 54 digital I/O (15 PWM), 4 serial ports, 16 MHz clock, 256 KB flash (8 KB used by bootloader), 8 KB SRAM, 4 KB EEPROM, 3.3 V logic.

## Other Components

**Multiplexer/Demultiplexer:** CD74HC4067 — 2–6 V operation, 16 channels, 4 control pins, enable pin (may be reducible to 3 control pins with an encoder).

**Input device options:** rotary encoder with push button, or a simple single-pin push button.

**Display:** 2.8" ILI9341 SPI TFT — 169 KB internal display RAM, 3.3 V logic, 6 GPIO pins.

**Storage:** MicroSD SPI/SDIO breakout board — shares the SPI bus, needing only one extra pin.

**Touch controller:** XPT2046 — 4-wire resistive touchscreen controller, paired with the ILI9341 for admin password entry.

## Notes

The ESP32 is faster with more RAM/flash (good for future/industrial expansion) but its 3.3 V logic complicates inclusion. The Arduino Mega tests matrices one at a time — slightly slower, but makes good use of the SD card.
