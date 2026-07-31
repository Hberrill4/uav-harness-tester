# UAV Harness Tester — Prototype Build Guide
### (using the real `Config.h` — Waveshare ESP32-S3-DEV-KIT-N8R8)


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
- [ ] Flash `MuxController` + `ContinuityTester` in a reduced mode
- [ ] Bridge a single jumper wire between one drive channel and one sense
  channel and confirm the firmware reports continuity correctly, and
  reports open on all other channel combinations.

Once one drive/sense pair is verified, the remaining 6 muxes are just repetition of a known-good
pattern.

## Phase 6 — Scale to all 8 muxes

Remaining EN lines:

| Bank | Drive EN | Sense EN |
|---|---|---|
| 1 | 6 | 21 |
| 2 | 7 | 10 |
| 3 | 8 | 11 |
| 4 | 9 | 38 |

- [ ] Wire the remaining 3 drive + 3 sense chips, sharing S0–S3 within
  each bank (drive S0-S3: 13/12/14/15, sense S0-S3: 16/17/5/18), each chip
  with its own EN line from the table above.
- [ ] Run the full 64×64 matrix scan with a handful of test jumpers spread
  across different banks (not just adjacent channels — want to catch
  cross-talk or address-line issues that only show up between banks).
- [ ] Use `countPopulated()` to confirm the wire count matches what you
  physically jumpered before trusting a full golden-sample capture.

## Phase 7 — Connector integration (J1 / J2)

- [ ] Wire the two 32-pin connectors (`CONNECTOR_A_NAME` = J1,
  `CONNECTOR_B_NAME` = J2) to the mux channel pins per `WireMap.h`.
- [ ] Use IDC ribbon cable for this internal PCB-to-connector run (per the
  earlier hardware review) — it's easier to keep 64 conductors organized
  and labeled than loose wire.
- [ ] Double check J1 and J2 aren't mirror-labeled or swapped — this was
  flagged before as a risk when both banks look identical.

### Status LED (GPIO 2 / 0 / 3)

- [ ] Wire the discrete status LED: R=2, G=0, B=3. G and B sit on
  strapping pins, which is fine as outputs, but keep the series resistor
  value reasonably high (1kΩ+) so the LED circuit doesn't pull meaningful
  current into the pin during the reset/boot sampling window.
- [ ] If you ever get mystery boot failures (random resets into download
  mode, garbled early serial output) that disappear when the LED is
  unplugged, that's this circuit interacting with strapping — worth
  testing in isolation if it happens.
- [ ] Note: GPIO48 is also wired to this board's onboard addressable RGB
  LED. Since you're using GPIO48 for `PIN_SENSE_SIG`, expect that onboard
  LED to flicker during scans — cosmetic only, no functional impact.

## Phase 8 — Mechanical / enclosure

- [ ] Mount LCD, button, and connectors on a panel or enclosure face.
- [ ] Strain-relief the harness connectors — these will get plugged and
  unplugged a lot during testing, more than any other part of the box.

## Phase 9 — Full system test

- [ ] Connect a known-good harness, run golden-sample capture.
- [ ] Deliberately induce one open and one short on the test harness and
  confirm `FaultAnalyzer` flags both correctly.
- [ ] Re-run capture after a full power cycle to confirm nothing was
  relying on RAM state instead of SD-persisted data.

---

### Bench testing tips
- Multimeter continuity-check every wire *before* powering on, especially
  around the mux address lines — a solder bridge there silently corrupts
  channel selection instead of failing loudly.
- Test one bank fully before wiring the next; don't wire all 8 muxes and
  then start debugging.
- Keep a written map of which physical jumper you used for each test —
  when a bank fails, you want to know instantly whether it's a wiring
  mistake or a firmware bug.
