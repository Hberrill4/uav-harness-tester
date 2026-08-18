# PCB Design


This document covers the custom PCB design using the software KiCAD for the UAV wire harness continuity tester: an ESP32-S3-based instrument that tests all 64 wires of a UAV harness for continuity and fault conditions, logs results with timestamps, and displays outcomes on an onboard TFT via a single-button interface.

See the [main README](../../README.md) for the full project overview and [`Workflow/Hardware`](../Hardware/) for the core hardware review that shaped these decisions and [Test pod Bill of Materials](https://github.com/Hberrill4/uav-harness-tester/blob/main/Workflow/Project-files/Test%20pod%20BOM.pptx) for the full list of test pod hardware used and [Adaptor pod Bill of Materials](https://github.com/Hberrill4/uav-harness-tester/blob/main/Workflow/Project-files/Adaptor%20BOM.pptx) for full list of adaptor hardware. Each PCB contains components of their respective BOM.

---

## Architecture at a glance

- ESP32-S3 drives 8× CD74HC4067 16-channel multiplexers (4 "drive" / 4
  "sense") to scan a full 64×64 wire matrix
- Results are logged to microSD and shown on a 2.8" TFT
- A single push button handles all user input, including an
  administrator mode
- The harness under test connects via modular pods over a floating
  umbilical interconnect


## Functional requirements

**Test coverage**
- Test all harness wires (up to 64) for continuity and fault conditions
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
- USB-C input, 3.3V logic, SPI for TFT/SD, multiplexer channel switching
- Protection on all ESP32-facing measurement inputs

**A breadboard prototype was built and validated first** — see
[`../Assembly/`](../Assembly/) for the bring-up log — before committing
to PCB layout.

## Schematics
KiCAD was used to route all traces. Although a decision was made to mount the hardware components ourselves it was key to ensure the right components were selected in KiCAD to maintain the same dimensions and pin outs. Additional software like espressif was downloaded to ensure such accuracy could be maintained.

## Circuit protection

#### TVS Diode (4 Channel) — ESDR0524SMUTAG
The primary ESD/transient protection device on the board. Each package contains 4 low-capacitance channels, clamping voltage spikes on signal lines to a safe level whenever the external wiring harness is connected or disconnected — the most likely source of electrostatic discharge into the sense/mux lines. The low-capacitance rating matters here because it protects the matrix scan lines without materially loading them or slowing signal edges.

#### 10 µF Electrolytic Capacitor — MCESL16V106M4X5.2
Bulk reservoir capacitance. It provides a low-impedance path to absorb and dump the energy from any transient a TVS diode clamps, and smooths lower-frequency ripple on the rail — complementing the fast, small-value ceramics rather than replacing them. The 16V rating gives comfortable headroom above the logic/supply rail it's protecting.

#### 10 kΩ Resistor — RT0805FRE0710KL
Used as a pull-up resistor on the protected lines holding them at logic level 1 when nothing is actively driving them (e.g. before a harness is connected). This prevents floating inputs from producing false continuity/fault readings, working alongside the TVS clamping.

#### 0.1 µF Decoupling Capacitor — GCM155R71H104KE02J
Local high-frequency decoupling, placed as close as possible to IC power pins. It filters fast switching noise and supplies instantaneous current during transients, keeping the supply rail stable — standard practice paired with the bulk electrolytic above.



## Design workflow

-  Draft schematic — measurement circuit, power flags, net labels, test points, decoupling
-  Run ERC and resolve until clean
-  Assign footprints and verify against datasheets
-  Get schematic reviewed and approved
-  Import schematic into PCB editor
-  Arrange components — wide power traces, digital/analog separation
-  Route traces
-  Add ground plane
-  Run DRC
-  Inspect in 3D
-  Final review against schematic
  
Final Test pod PCB is shown here: 

**A breadboard prototype was built and validated first** — see
[`../Assembly/`](../Assembly/) for the bring-up log — before committing
to PCB layout.

## Known issues carried from design review

- Two GPIO pin conflicts (SD chip-select vs. mux enable; SPI clock/data
  vs. the Octal PSRAM bus) were identified and resolved during firmware
  bring-up — see commit history in `firmware/Config.h`
- Protection/measurement circuit block was absent from the original
  schematic and is being added in this revision
- Mux bank labeling was ambiguous when both banks looked identical on the
  original diagram; corrected with distinct drive/sense labeling
