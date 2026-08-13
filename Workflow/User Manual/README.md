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

### 2.1 ⚡ Electrical Precautions

- **Test lead voltage/current limits:** This tester is a **passive continuity tester only** — it applies a low-voltage sense signal (3.3 V logic level from the ESP32-S3) through the mux network to detect open/short/mismatch conditions. It does not source test voltage/current beyond this. Do not connect the DC37 test leads to any harness, wire bundle, or circuit that carries its own voltage source.

- **TVS diode rating (`ESDR0524SMUTAG`):** Each mux input is protected by an `ESDR0524SMUTAG` dual/quad-element TVS array with a **5 V reverse standoff voltage** and a **5.5–6 V breakdown voltage**. Under ESD transient conditions the clamping voltage can reach up to **15 V max** — this is a *transient protection* spec, not a sustained-voltage rating.

  > ⚠️ **Treat 5 V as the absolute ceiling** for any voltage present on a wire under test. This protection exists for ESD events, not for testing energized circuits — see [2.3](#23--do-not-test-energizedlive-harnesses).

- **ESD handling — ESP32-S3 and mux ICs:** The ESP32-S3-WROOM-1 module and CD74HC4067 muxes are CMOS devices vulnerable to static discharge.
  - Use an anti-static wrist strap or mat when handling the bare PCB, especially before full assembly.
  - Store the assembled tester in an anti-static bag when not in use.
  - Handle by PCB edges or connector shells — avoid touching mux input pins or the WROOM module pins directly.

<small>TVS diodes: onsemi `ESDR0524SMUTAG`, U-DFN2510, 4-element common anode.</small>

---

### 2.2 🔌 Power Source Warnings

- **Power input:** The tester is powered exclusively via **USB** through the ESP32-S3-DevKitC. There is no battery in the design — no charging precautions apply.
- Use only a USB cable and power source rated for the ESP32-S3's requirements (5 V, standard USB current limits). Do not use damaged USB cables or connectors.
- Disconnect USB power before making or breaking connections at the DC37 connectors or any exposed header.

---

### 2.3 🚫 Do Not Test Energized/Live Harnesses

> **This applies, without exception.**

The tester is designed exclusively for continuity/open/short/mismatch testing on **de-energized, unpowered wire harnesses**. Before every test session, confirm:

- [ ] The harness under test is fully disconnected from any UAV battery, ESC, flight controller power rail, or other voltage source.
- [ ] No capacitors in the harness (e.g., on ESC inputs) retain residual charge — allow bleed-down time or discharge safely first.
- [ ] Never connect the DC37 umbilical to a harness still mounted in a powered-on airframe.

Testing a live harness will exceed the TVS diode's 5 V standoff rating and can damage the mux ICs, the ESP32-S3, or the harness itself.

---

### 2.4 🔗 Umbilical Cable & Connector Handling

- **Cable:** RND `765-00044`, male–male 37-pin D-Sub, 1 m. No manufacturer bend-radius spec is published for this cable — as a conservative general D-Sub cable practice, avoid bends tighter than **~8× cable OD**, and never bend sharply right at the connector backshell.
- **Temperature range:** The cable/connector assembly is rated **10 °C to 60 °C** per the manufacturer datasheet — this becomes the practical environmental ceiling/floor for the whole tester (see [2.5](#25--environmental-limits)), since it's the most restrictive component.
- **Strain relief:** Support the cable at the D-Sub shell, not at the pins. Anchor a strain-relief boot or cable tie to the enclosure rather than the connector shell itself.
- Mate/unmate D-Sub connectors straight-on — never at an angle — to avoid bending pins.
- Inspect D-Sub pins for bend or recession before each test session.

<small>Cable: RND 765-00044, 37-pin D-Sub M–M, 1 m, grey jacket, RoHS.</small>

---

### 2.5 🌡️ Environmental Limits

- **Operating temperature:** **10 °C to 60 °C**, set by the D-Sub umbilical cable rating (the limiting component — the TVS diodes are rated to −55 °C to +125 °C, well beyond this).

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
| OPEN | red | Expected connection not detected | Broken wire, bad crimp, unseated pin |
| SHORT | red | Unexpected connection detected | Pinched insulation, solder bridge |
| MISMATCH | red | Connection present but wrong pin/wire | Miswire, wrong harness variant |
| PASS | green | Matches golden sample | — |

See [Workflow/Specific-wire-reference.md](main/Workflow/Specific-wire-reference.md) for interpretation of results displayed for this harness. This table will also be on the particular adaptor pod.
Both a Fail and a Pass will be logged along with its test number. In the event of a fail all information about the fail will be logged like the type and location.

### 6.4 Saving / Exporting Results
- When the SD card if full, damaged or fails to log an error message will appear until the error is corrected or esp32 is reset.
- To retrieve logged data, remove the on board SD card from the ESP32 and plug it into a computer.

---

## 7. Menu & Display Navigation

![General operating process diagram](https://raw.githubusercontent.com/Hberrill4/uav-harness-tester/main/Workflow/images/general%20operating%20process.jpg)

---

## 8. Maintenance & Storage

- Cleaning: Dry compressed air or a soft brush on the SD card slot and exposed connector pins (mux inputs, umbilical connector); isopropyl alcohol on a lint-free cloth for the ILI9341 display, avoid pressure on the glass.
- Storage conditions: Cool, dry, away from direct sun (ILI9341 contrast can degrade above ~60°C); keep the umbilical connector capped when detached to protect the conductors from oxidation/damage.
- Power: USB-C powered only, no onboard battery. No charge-cycle or storage-charge considerations — just avoid leaving the USB-C port exposed to dust/moisture when not connected.
- Firmware update procedure: Connect the ESP32-S3 via USB-C, pull the latest from github.com/Hberrill4/uav-harness-tester, then flash with pio run -t upload (or the Upload button in PlatformIO/VS Code).

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

- **A. Full pinout / wiring reference** — [Workflow/wiring-referance.md](Workflow/wiring-reference.md)
- **B. Fault report file format** — [Workflow/fault-report-format.md](Workflow/fault-report-format.md)
- **C. Glossary**
  - *Golden Sample* — a verified-good harness scan used as the reference for pass/fail comparison
  - *OPEN / SHORT / MISMATCH* — as defined above
  - *ACTIVE_WIRE_MASK* — identifies which wires, channels, or signal lines are currently active. Each bit in the mask typically represents one wire: a bit value of 1 means that wire is active, while 0 means it is inactive. This allows multiple active wires to be represented efficiently in a single value

- **E. Support / contact**
  - Repo link: github.com/Hberrill4/uav-harness-tester

---

