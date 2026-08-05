# Wiring reference

All pin assignments live in a single place: `include/Config.h`. Nothing else
in the codebase should hardcode a GPIO number — if you rewire something,
this is the only file you should need to touch.

## Pins assigned

### Mux Select Lines

| Connection | GPIO Pin | Notes |
|---|---|---|
| `PIN_DRIVE_S0` | 13 | |
| `PIN_DRIVE_S1` | 12 | |
| `PIN_DRIVE_S2` | 14 | |
| `PIN_DRIVE_S3` | 15 | |
| `PIN_DRIVE_SIG` | 1 | |
| `PIN_DRIVE_EN[0..3]` | 6, 7, 8, 9 | |
| `PIN_SENSE_S0` | 16 | |
| `PIN_SENSE_S1` | 17 | |
| `PIN_SENSE_S2` | 5 | |
| `PIN_SENSE_S3` | 18 | |
| `PIN_SENSE_SIG` | 48 | Moved off GPIO46 (strapping pin) |
| `PIN_SENSE_EN[0..3]` | 21, 10, 11, 38 | |

**SHARED SPI BUS (SD card + TFT, each with its own CS)**

PIN_SPI_SCK   = 19;
PIN_SPI_MOSI  = 20;
PIN_SPI_MISO  = 47;

**SD CARD (SPI)**

PIN_SD_CS = 39;

**TFT DISPLAY (2.8" ILI9341, SPI)**

PIN_TFT_CS  = 40;
PIN_TFT_DC  = 41;
PIN_TFT_RST = 42;

// STATUS RGB LED (discrete LED, separate from the onboard addressable one)
// ---------------------------------------------------------------------------
static const uint8_t PIN_LED_R = 2;
static const uint8_t PIN_LED_G = 0;    // strapping pin; safe as an OUTPUT
static const uint8_t PIN_LED_B = 3;    // strapping pin; safe as an OUTPUT

// GPIO46 intentionally left unassigned - see notes above.



## Wire numbering convention

Wires are indexed **0-63 internally**. The harness uses **two identical
32-pin connectors** (`J1` and `J2`), so the firmware converts internal
indices to physical locations automatically:

- Wire index `0-31` → connector **J1**, pin `(index % 32) + 1`
- Wire index `32-63` → connector **J2**, pin `(index % 32) + 1`

This mapping lives in `src/WireMap.h` (`wireLabel()`), and both the LCD and
the SD log use it — fault reports show real connector/pin labels like
`J1-13` or `SHORT J1-5<->J2-9`, not abstract wire numbers. If your
connector pin-out doesn't match a simple sequential 1-32 order (e.g. a
circular connector with a non-sequential pin layout), update `wireLabel()`
to do the correct index→pin translation for your specific part.

## Mux addressing

Wire index → mux + channel:

```
muxIndex = wireIndex / 16   // which of the 4 muxes in the bank (0-3)
channel  = wireIndex % 16   // which of the 16 channels on that mux (0-15)
```

`MuxController` handles this internally — you never need to compute it by
hand, but it's worth understanding when debugging with a multimeter.
