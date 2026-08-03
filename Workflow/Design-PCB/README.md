# PCB Design

**Status:** 🟡 In progress — schematic capture, prototype validated on breadboard

This document covers the custom PCB design for the UAV wire harness
continuity tester: an ESP32-S3-based instrument that tests all 64 wires of
a UAV harness for continuity and fault conditions, logs results with
timestamps, and displays outcomes on an onboard TFT via a single-button
interface. See the [main README](../../README.md) for the full project
overview and [`Workflow/Hardware`](../Hardware/) for the hardware review
that shaped these decisions and [Bill of Materials](docs/project-files/BOM%20for%20project.csv) for the full list of hardware used.

---

## Architecture at a glance

- ESP32-S3 drives 8× CD74HC4067 16-channel multiplexers (4 "drive" / 4
  "sense") to scan a full 64×64 wire matrix
- Results are logged to microSD and shown on a 2.8" TFT
- A single push button handles all user input, including an
  administrator mode
- The harness under test connects via modular pods over a floating
  umbilical interconnect

## Bill of materials

| Component | Qty | Notes |
|---|---|---|
| ESP32-S3-WROOM-1 (N8R8, Octal PSRAM) | 1 | Corrected from an earlier WROOM-32 mislabel during design review — confirm GPIO33–37 stay clear of the Octal PSRAM bus |
| CD74HC4067 16-channel mux/demux | 8 | 4 drive-side, 4 sense-side; share address lines within each bank |
| ILI9341 2.8" SPI TFT | 1 | Requires a dedicated DC pin, not just CS/RST — missed in an earlier schematic revision |
| microSD card module (SPI) | 1 | Shares the SPI bus with the TFT; separate CS |
| DIN 41612 64-pin connector | 1 | Interfaces to the harness-under-test pods |
| Voltage regulator | 1 | 5V input → 3.3V logic rail |
| Protection resistors | TBD | Series current-limiting on ESP32-facing measurement lines — flagged in design review as a missing block, being added here |
| Decoupling capacitors | 1 per IC | 0.1 µF ceramic, placed close to each mux and the TFT |
| Push button | 1 | Single-button UI, debounced in firmware |

## Functional requirements

**Test coverage**
- Test all 64 harness wires for continuity and fault conditions
- Store a golden sample reference in RAM and persist it to SD card

**Data & UI**
- Log every test result with a timestamp
- Display results on the 2.8" TFT
- Operate entirely from a single push button, including an administrator mode

**Interconnect**
- Interface with modular test pods over a floating umbilical
- Internal PCB-to-connector runs use IDC ribbon cable; the external
  umbilical uses a high-density D-sub or keyed circular connector with
  28–30 AWG stranded conductors, chosen for flex durability over repeated
  connect/disconnect cycles

**Electrical**
- 5V input, 3.3V logic, SPI for TFT/SD, multiplexer channel switching
- Protection on all ESP32-facing measurement inputs

## Design workflow

- [x] Draft schematic — measurement circuit, power flags, net labels, test points, decoupling
- [x] Run ERC and resolve until clean
- [ ] Assign footprints and verify against datasheets
- [ ] Import schematic into PCB editor
- [ ] Arrange components — wide power traces, digital/analog separation
- [ ] Route traces
- [ ] Add ground plane
- [ ] Run DRC
- [ ] Inspect in 3D
- [ ] Final review against schematic

**A breadboard prototype was built and validated first** — see
[`../prototype/`](../prototype/) for the bring-up log — before committing
to PCB layout.

## Known issues carried from design review

- Two GPIO pin conflicts (SD chip-select vs. mux enable; SPI clock/data
  vs. the Octal PSRAM bus) were identified and resolved during firmware
  bring-up — see commit history in `firmware/Config.h`
- Protection/measurement circuit block was absent from the original
  schematic and is being added in this revision
- Mux bank labeling was ambiguous when both banks looked identical on the
  original diagram; corrected with distinct drive/sense labeling
