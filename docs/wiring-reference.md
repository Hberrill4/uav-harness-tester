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

Wires are indexed **0-63 internally**, but displayed and logged
**1-64** (matches how people usually label physical pins). If your harness
has two separate connectors (e.g. J1 and J2, 32 pins each), wire index `n`
maps to:

- Connector J1, pin `(n % 32) + 1` if `n < 32`
- Connector J2, pin `(n % 32) + 1` if `n >= 32`

Adjust this mapping in `docs/wire-map.md` (create one for your specific
harness) so fault reports like `SHORT W5<->W9` translate to real connector
pins for whoever is doing the physical repair.

## Mux addressing

Wire index → mux + channel:

```
muxIndex = wireIndex / 16   // which of the 4 muxes in the bank (0-3)
channel  = wireIndex % 16   // which of the 16 channels on that mux (0-15)
```

`MuxController` handles this internally — you never need to compute it by
hand, but it's worth understanding when debugging with a multimeter.
