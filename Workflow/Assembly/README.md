# UAV Harness Tester — Prototype Build Guide
### (using the real `Config.h` — Waveshare ESP32-S3-DEV-KIT-N16R8)


## Phase 1 — Power and ground

- [ ] Build a common 3.3V rail and ground bus on the breadboard.
- [ ] Add decoupling capacitors near the LCD and near each mux IC (0.1 µF
  ceramic)
- [ ] Confirmed rail voltage under load with a multimeter before connecting
  any ICs

## Phase 2 — Display bring-up (ILI9341)

Wire, using the corrected pin map:

| Signal | GPIO |
|---|---|
| SCK  | 19 (corrected) |
| MOSI | 20 (corrected) |
| MISO | 47 (verify 3.3V first) |
| CS   | 40 (`PIN_TFT_CS`) |
| DC   | 41 (`PIN_TFT_DC`) |
| RST  | 42 (`PIN_TFT_RST`) |

- [ ] Wire the table above, plus backlight/LED to 3.3V
- [ ] Flash just `DisplayManager`'s init + a test screen.
- [ ] **Expect:** a stable, correctly-oriented test image with no tearing,
  no random pixels, no color inversion.
- [ ] Confirmed this works cleanly before moving on as SPI wiring issues are
  much easier to isolate now than once the SD card and muxes are also on
  the bus.

## Phase 3 — microSD bring-up

- [ ] Wire SD card SPI: shares SCK (19), MOSI (20), MISO (47) with the
  LCD, but has its own CS on **GPIO 39** (`PIN_SD_CS`).
- [ ] Flash `StorageManager` alone and confirm you can write and read back
  a log file at `/test_log.csv`.
- [ ] **Expect:** GPIO39 reads HIGH immediately after reset which is the correct
  idle state for an SPI CS line, so nothing to fix there.
- [ ] Test with the card inserted and removed to know what the
  failure mode looks like 


## Phase 4 — Button input

- [ ] Wire the single push-button to **GPIO 4** (`PIN_BUTTON`)
- [ ] Flash `ButtonManager` alone and confirm short-press, long-press
  (≥5000ms per `LONG_PRESS_MS`), and no false triggers from switch bounce

## Phase 5 — One mux pair (proof of concept)

Drive bank (first chip only):

| Signal | GPIO |
|---|---|
| S0 | 13 |
| S1 | 12 |
| S2 | 14 |
| S3 | 15 |
| SIG | 1 |
| EN[0] | 6 |

Sense bank (first chip only):

| Signal | GPIO |
|---|---|
| S0 | 16 |
| S1 | 17 |
| S2 | 5 |
| S3 | 18 |
| SIG | 48 |
| EN[0] | 21 |

- [ ] Wired just these two chips: shared S0-S3 address lines within each
  bank, separate EN lines.
- [ ] Flash Microcontroller
- [ ] Connect wires between MUXs and scan as golden sample
- [ ] Return to test screen to then test same wire and confirm it passes and is displayed:[Pass evidence](https://github.com/Hberrill4/uav-harness-tester/blob/main/Workflow/images/IMG_8664.MOV)
- [ ] Disconnect one wire and switch two wires around to confirm open and crossed wire fault detection:[Fail evidence](https://github.com/Hberrill4/uav-harness-tester/blob/main/Workflow/images/IMG_8665.MOV)


