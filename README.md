# UAV harness continuity tester

ESP32-based automated continuity tester for a 64-wire UAV wiring harness.
Detects open circuits, shorts, and wire mismatches, and reports the exact
wire location of each fault.

## How it works

One button drives everything:

- **Short press** — runs a test (normal mode) or captures a golden
  reference sample (admin mode)
- **Hold 5 seconds** — toggles between normal and admin mode (LCD turns
  blue in admin mode)

Every test scans all 64 wires against all 64 sense channels (not just a
1:1 diagonal check), which is what makes shorts and mismatches detectable,
not just opens. Results show on the LCD (pass = green, fail = red,
admin = blue) and get logged with a timestamp to the SD card.

See [docs/fault-report-format.md](docs/fault-report-format.md) for what a
fault report looks like and how faults are classified.

## Project structure

```
include/
  Config.h              All pin assignments, timing constants, wire count.
                         File to edit hardware changes.
src/
  main.cpp               Owns the state machine (normal/admin, test/capture)
  ButtonManager.*         Non-blocking debounce + short-press vs 5s-hold detection
  MuxController.*         Addresses any of the 64 wires across the drive/sense mux banks
  ContinuityTester.*      Runs the full 64x64 connectivity scan
  FaultAnalyzer.*         Classifies faults: OPEN / SHORT / MISMATCH, with location
  WireMap.h               Converts wire index -> connector+pin label (e.g. "J1-13")
  DisplayManager.*        LCD + RGB
  StorageManager.*        SD card logging + golden sample persistence
docs/
  wiring-reference.md     Hardware assumptions baked into the code, pin mapping conventions
  fault-report-format.md  What fault reports look like and how they're generated
platformio.ini            Build config (ESP32, Arduino framework)
```

## Getting started

1. Open this folder in VS Code with the [PlatformIO extension](https://platformio.org/install/ide?install=vscode).
2. Check [docs/wiring-reference.md](docs/wiring-reference.md) and update
   `include/Config.h` to match your actual mux/LCD/SD wiring.
3. Build and upload: PlatformIO sidebar → **Upload**, or `pio run --target upload`.
4. Open the serial monitor at 115200 baud to catch SD init errors on boot.

## Status

Firmware logic is complete and structured; it has **not yet been validated
on real hardware**. Before first power-on:

- [ ] Confirm mux EN pin polarity (active-low assumed) matches your mux breakout
- [ ] Confirm LCD I2C address and dimensions match your actual display
- [ ] Add an RTC or NTP sync so log timestamps are wall-clock time, not seconds-since-boot
- [ ] Tune the `delayMicroseconds()` settle times in `ContinuityTester` against your actual mux part
- [ ] Add a sanity check before saving a golden sample (reject an all-open capture)
- [ ] Decide if the LCD needs fault paging for reports with more than 3 faults
      ## Project Files
docs/BOM-for-project.xlsx
docs/Gant-Chart.xlsx
