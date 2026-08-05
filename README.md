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

See [Workflow/fault-report-format.md](Workflow/fault-report-format.md) for what a
fault report looks like and how faults are classified.

See [Workflow/User Manual](Workflow/User%20Manual) for full manual

## Firmware structure

### src/
- **[Config.h](src/Config.h)** — All pin assignments, timing constants, wire count. File to edit hardware changes.
- **[main.cpp](src/main.cpp)** — Owns the state machine (normal/admin, test/capture)
- **[ButtonManager.h](src/ButtonManager.h)** — Non-blocking debounce + short-press vs 5s-hold detection
- **[MuxController](src/MuxController.h)** — Addresses any of the 64 wires across the drive/sense mux banks
- **[ContinuityTester](src/ContinuityTester.h)** — Runs the full 64×64 connectivity scan
- **[FaultAnalyzer](src/FaultAnalyzer.h)** — Classifies faults: OPEN / SHORT / MISMATCH, with location
- **[WireMap.h](src/WireMap.h)** — Converts wire index → connector+pin label (e.g. "J1-13")
- **[DisplayManager](src/DisplayManager.h)** — LCD + RGB
- **[StorageManager](src/StorageManager.h)** — SD card logging + golden sample persistence

### Workflow/
- **[wiring-reference.md](Workflow/wiring-reference.md)** — Hardware assumptions baked into the code, pin mapping conventions
- **[fault-report-format.md](Workflow/fault-report-format.md)** — What fault reports look like and how they're generated
- **[Assembly](Workflow/Assembly/)** — The process behind the prototype and how everything was made
- **[Design-Architecture](Workflow/Design-Architecture/)** — General system layout and how components interact and connect
- **[Design-Enclosure](Workflow/Design-Enclosure/)** — Modular test pod and enclosure for bench-top testing
- **[Design-PCB](Workflow/Design-PCB/)** — PCB design process and final product
- **[Hardware](Workflow/Hardware/)** — Components considered and finalized design decisions
- **[Images](Workflow/Images/)** — Contains all images used throughout the documentation
- **[Project-files](Workflow/Project-files/)** — Contains all external project files
- **[User Manual](Workflow/User%20Manual/)** — Full user manual covering operation, maintenance, and troubleshooting
    
**platformio.ini**            Build config (ESP32, Arduino framework)



## Project Files

- [Bill of Materials](/Workflow/Project-files/BOM%20for%20project.csv)
- [Gant Chart](/Workflow/Project-files/Gant%20Chart.xlsx)
- [Test pod BOM](/Workflow/Project-files/Test%20pod%20BOM.pptx)
