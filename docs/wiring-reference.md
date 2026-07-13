# Wiring reference

All pin assignments live in a single place: `include/Config.h`. Nothing else
in the codebase should hardcode a GPIO number — if you rewire something,
this is the only file you should need to touch.

## Assumptions baked into the current code

These need to be verified/adjusted against your actual schematic before
first power-on:

| Assumption | Where it matters | If wrong |
|---|---|---|
| Mux EN pins are **active-LOW** (74HC4067-style) | `MuxController::begin/selectDrive/selectSense` | Every wire will read "always open" or "always shorted" — invert the HIGH/LOW in those functions |
| Drive/sense mux banks share select lines across all 4 muxes in a bank | `Config.h` pin tables | If your breakout wires select lines per-mux instead of shared, you'll need 16 select pins instead of 4 |
| Sense pin uses internal pulldown, reads HIGH only when continuity exists | `MuxController::begin()` | If your sense circuit is wired differently (e.g. through a voltage divider or optocoupler), the polarity or threshold may need to change |
| 20x4 I2C LCD at address `0x27` | `DisplayManager` + `Config.h` | Wrong address = blank screen; run an I2C scanner sketch to confirm |
| SD card on hardware SPI, single CS pin | `Config.h` (`PIN_SD_CS`) | If using a different SPI bus or a card reader with extra pins, adjust `StorageManager::begin()` |

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
