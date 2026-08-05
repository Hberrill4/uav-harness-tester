# UAV harness tester — User Manual
### UAV Wire Harness Continuity Tester

**Document version:** v0.2 (draft)
**Date:** 2026-08-DD
**Author:** Harrison Berrill

## 1. Introduction

- **Purpose:** Verifies continuity across a wire harness against a known-good "golden sample" for up to 64 wires. flags OPEN/SHORT/MISMATCH faults and their location. Saves test number, result of scan and when a new golden harness is submitted onto an SD card. It replaces manual time consuming point-to-point ohmmeter checks
- **Intended audience:** Assembly technicians, Drone operators or anyone using a wire loom.
- **Scope:** This Manual covers usage, troubleshooting and maintenance. The development of the product and code/firmware are covered separately in the repo.
- **Key features:**
  - Full matrix scan across [N]×[N] wire matrix
  - Golden-sample capture and comparison
  - Color-coded display
  - Open, Mismatch and Closed wire fault location
  - On-device result logging to SD card

---

## 2. Safety Information

- Electrical precautions (max voltage/current on test leads, ESD handling for the ESP32-S3 and mux ICs)
- Power source warnings (battery type, charging precautions if applicable)
- Do not test energized/live harnesses — [confirm and state clearly if this applies]
- Handling precautions for the umbilical cable and connectors (bend radius, strain relief)
- Environmental limits (operating temperature, humidity, ingress rating if any)

---

## 3. What's in the Box

- [ ] Tester unit
- [ ] Umbilical cable / connector to harness under test
- [ ] USB-C cable
- [ ] This guide

---

## 4. Product Overview

### 4.1 Physical Layout
- [ ] Insert labeled photo/diagram showing: display, buttons, status LED, connector(s), SD card slot, power switch/port
- Connector reference: J1 / J2 pinout — link to or summarize `docs/wiring-reference.md`

### 4.2 Controls
| User Type | Short Press | Long Press (5s hold) |
|---|---|---|
| Tester | Tests current wire harness | Switches to admin mode |
| Admin | Submits current harness as new golden sample | Switches to Tester mode |
| All | After scan its used to move off the test screen |

### 4.3 Indicators
| Indicator | State | Meaning |
|---|---|---|
| RGB LED | Green | Pass |
| RGB LED | Red | Fail |
| RGB LED | Blue | Golden sample mode |
| Display | — | Shows live scan status and fault report, color-coded per mode (see 4.4) |

*Note: GPIO48 (RGB LED) is shared with the onboard LED — cosmetic flicker during scans is expected, not a fault.*

### 4.4 Display Color Reference


| Mode / Result | Display Color | What it means |
|---|---|---|
| Idle / standby | white text] | Waiting for a scan to start |
| Golden Sample Capture — menu | Cyan | Matrix scan ready to be submitted |
| Golden Sample Capture — success | Blue | Sample stored; wire count shown |
| Golden Sample Capture — failure | Red | 0 (or too few) wires detected — reseat harness and retry |
| Test Scan — No reference | Magenta | No golden sample to refer off |
| Test Result — PASS | Green | Scan matches golden sample exactly |
| Test Result — OPEN | Red | Expected connection missing |
| Test Result — SHORT | Red | Unexpected connection detected |
| Test Result — MISMATCH | Red | Connection present but on the wrong pin/wire |

The Display will also show the location of the failure and its type. eg. Mismatch J11->J13

---

## 5. Getting Started

### 5.1 Powering On
1. Connect USB-C to port
2. Components will initialize and appear ready to scan

### 5.2 Connecting a Harness
1. Connect the harness that's being tested to the 2 adaptors which connect via D-Sub to the main test enclosure
2. Ensure connection is fully secure


## 6. Operating Instructions

### 6.1 Capturing a Golden Sample
1. Connect a **known-good, verified** harness to J1/J2.
2. Hold button for 5 seconds to enter Golden Sample Capture mode (Admin mode).
3. The display turns **cyan** prompts to press the button again to conduct the scan.
4. On success, the display turns **blue** and prompts to press the button again to return to admin screen.
5. The sample is written to the SD card as the reference for all future scans and this new scan change is logged as such. A new golden sample being submitted overwrites the previous one.
6. To exit the admin screen, follow the on screen instruction to hold button for 5 seconds.

### 6.2 Running a Continuity Test
1. Connect the harness under test to J1/J2.
2. In tester mode Short-press button to start a scan.
4. Results populate the 20-column grid, one line per fault — or a single **"PASS"** line in green if the harness matches the golden sample exactly.
5. See 4.4 for the full color reference and 6.3 for what each result code means.

### 6.3 Interpreting Results
| Result Code | Color | Meaning | Typical Cause |
|---|---|---|---|
| OPEN | [red] | Expected connection not detected | Broken wire, bad crimp, unseated pin |
| SHORT | [red] | Unexpected connection detected | Pinched insulation, solder bridge |
| MISMATCH | [red] | Connection present but wrong pin/wire | Miswire, wrong harness variant |
| PASS | [green] | Matches golden sample | — |

- How J1/J2 physical labels map to displayed fault locations
Both a Fail and a Pass will be logged along with its test number. In the event of a fail all information about the fail will be logged like the type and location.

### 6.4 Saving / Exporting Results
- [SD card file format and naming — see `docs/fault-report-format.md`]
- To retrieve logged data, remove the on board SD card from the ESP32 and plug it into a computer.
- [SD write error behavior — what the user sees if a write fails, and what to do]

---

## 7. Menu & Display Navigation

*Only needed if there's more than one screen/mode. A simple state diagram helps here.*

- [ ] Insert flow diagram: Idle → Golden Sample Capture → Test Scan → Results → (repeat)
- Description of each screen/state and how to move between them

---

## 8. Maintenance & Storage

- Cleaning (connector contacts, display)
- Storage conditions (temperature, humidity, connector caps)
- Battery care, if applicable (storage charge level, cycle life)
- Firmware update procedure (if field-updatable): [steps or link to repo instructions]

---

## 9. Troubleshooting

| Symptom | Possible Cause | Fix |
|---|---|---|
| Device won't power on | GPIO0 pulled low at boot (forces download mode, looks like "won't boot") | Check nothing is shorting GPIO0 low at power-up; confirm BOOT button isn't stuck |
| Device won't power on	| Onboard voltage regulator (5V/3.3V) failure |	Measure regulator output pins directly; if no output, regulator or its input caps may have failed
| Device won't power on	| Short circuit on board (solder bridge, pinched wire) drawing excess current	| Disconnect load/peripherals one at a time; check for hot components; inspect under magnification for bridges
| SD card error on first boot | `SPI.begin()` not called before SD access | Confirmed firmware fix applied (SPI now initialized before SD mount) — update firmware if still seen |
| Display shows garbled or no output | Wrong display driver assumed, or wiring fault | Confirm hardware is the ILI9341 SPI TFT, not an I2C character LCD; check SPI wiring |
| SD card or scan behaves erratically after golden sample capture | GPIO conflict (e.g. SD_CS sharing a pin with a sense-enable line) | Confirmed resolved in current pin map — verify you're on firmware with the de-conflicted `Config.h` |
| Golden Sample Capture always fails / 0 wires detected | Harness not seated, or wrong connector (J1 vs J2) | Reseat harness; confirm correct connector used |
| False SHORT readings | Dirty/oxidized contacts, pinched insulation | Clean contacts; inspect harness for damage; retest |
| RGB LED flickers during scan | Expected — GPIO48 is shared with the onboard RGB LED | No action needed, cosmetic only |
| Display text colors don't match this guide | Firmware color mapping changed since this guide was written | Check `DisplayManager` source against firmware version in the header of this guide |

*Keep this table growing as real-world issues are found during bring-up and use.*

---

## 10. Technical Specifications

| Spec | Value |
|---|---|
| Microcontroller | ESP32-S3-WROOM-1 (N8R8, Octal PSRAM) |
| Display | ILI9341 SPI TFT |
| Matrix size | [64×64 or actual configured size] |
| Connectors | J1 / J2 - type depending on harnesses being tested |
| Umbilical cable | D-Sub connector cable |
| Power | USB-C |
| Dimensions / weight | [ ] |
| Operating temperature | [ ] |
| Storage | microSD, 8GB |

---

## 11. Appendix

- **A. Full pinout / wiring reference** — [Workflow/wiring-referance.md](Workflow/wiring-referance.md)
- **B. Fault report file format** — [Workflow/fault-report-format.md](Workflow/fault-report-format.md)
- **C. Glossary**
  - *Golden Sample* — a verified-good harness scan used as the reference for pass/fail comparison
  - *OPEN / SHORT / MISMATCH* — as defined above
  - *ACTIVE_WIRE_MASK* — identifies which wires, channels, or signal lines are currently active. Each bit in the mask typically represents one wire: a bit value of 1 means that wire is active, while 0 means it is inactive. This allows multiple active wires to be represented efficiently in a single value

- **E. Support / contact**
  - [Repo link: github.com/Hberrill4/uav-harness-tester]

---

