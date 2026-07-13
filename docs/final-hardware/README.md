# Final Hardware

- **Microcontroller:** ESP32-S3 (same specs as above)
- **Multiplexer/Demultiplexer:** CD74HC4067
- **Input device:** single push button (1 pin)
- **Storage:** MicroSD SPI/SDIO breakout board
- **Display:** 2.8" ILI9341 SPI TFT
- **Connector:** 64-pin DIN 41612 universal PCB connector

## TFT ↔ ESP32 Wiring

| TFT Pin | ESP32 GPIO | Purpose |
|---|---|---|
| VCC | 3.3 V | Power |
| GND | GND | Ground |
| SCK | GPIO18 | SPI clock |
| MOSI | GPIO23 | Data to display |
| MISO | GPIO19 | Data from display |
| CS | GPIO5 | Select display |
| DC | GPIO2 | Type of byte |
| RESET | GPIO4 | Reset display |
| LED | 3.3 V | Backlight |
