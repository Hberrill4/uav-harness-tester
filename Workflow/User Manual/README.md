# [Product Name] — User Guide
### UAV Wire Harness Continuity Tester

**Document version:** v0.1 (draft)
**Date:** [YYYY-MM-DD]
**Author:** [Your name]
**Firmware version this guide applies to:** [e.g. v1.0]

> This is a **template**. Bracketed text (`[...]`) marks content to fill in. Section notes in *italics* explain what belongs there and why — delete the notes once the section is written.

---

## 1. Introduction

*Orients a new reader in under a minute: what the device is, who it's for, what it is not.*

- **Purpose:** One paragraph — what the tester does (verifies continuity across a wire harness against a known-good "golden sample," flags OPEN/SHORT/MISMATCH faults) and why it exists (replaces manual point-to-point ohmmeter checks).
- **Intended audience:** [Assembly technicians / QA / yourself as sole operator]
- **Scope:** What this guide covers vs. what it doesn't (e.g. firmware development and PCB design are covered separately in the repo's `docs/` folder, not here).
- **Key features** (bullet list, 4–6 items):
  - Full matrix scan across [N]×[N] wire matrix
  - Golden-sample capture and comparison
  - Color-coded fault display (OPEN / SHORT / MISMATCH)
  - Pass/fail/golden-sample status via RGB LED
  - On-device result logging to SD card

---

## 2. Safety Information

*This section exists to prevent hardware damage or injury — keep it near the front, not buried.*

- Electrical precautions (max voltage/current on test leads, ESD handling for the ESP32-S3 and mux ICs)
- Power source warnings (battery type, charging precautions if applicable)
- Do not test energized/live harnesses — [confirm and state clearly if this applies]
- Handling precautions for the umbilical cable and connectors (bend radius, strain relief)
- Environmental limits (operating temperature, humidity, ingress rating if any)

---

## 3. What's in the Box

- [ ] Tester unit
- [ ] Umbilical cable / connector to harness under test
- [ ] [Charging cable / power adapter, if applicable]
- [ ] [Quick start card]
- [ ] This guide

---

## 4. Product Overview

*A labeled diagram here is worth more than paragraphs — plan to insert one (photo or SVG) with callouts.*

### 4.1 Physical Layout
- [ ] Insert labeled photo/diagram showing: display, buttons, status LED, connector(s), SD card slot, power switch/port
- Connector reference: J1 / J2 pinout — link to or summarize `docs/wiring-reference.md`

### 4.2 Controls
| Control | Short Press | Long Press (5s hold) |
|---|---|---|
| [Button 1 name] | [action] | [action] |
| [Button 2 name] | [action] | [action] |

### 4.3 Indicators
| Indicator | State | Meaning |
|---|---|---|
| RGB LED | Green | [Pass] |
| RGB LED | Red | [Fail] |
| RGB LED | [Color] | [Golden sample mode] |
| Display | — | Shows live scan status and fault report, color-coded per mode (see 4.4) |

*Note: GPIO48 (RGB LED) is shared with the onboard LED — cosmetic flicker during scans is expected, not a fault.*

### 4.4 Display Color Reference

*The display uses the same green/red/yellow scheme throughout, so the operator only needs to learn it once. Confirm/adjust the exact hex values against `DisplayManager` — the mapping below reflects the current color-coded fault design.*

| Mode / Result | Display Color | What it means |
|---|---|---|
| Idle / standby | [neutral — e.g. white or gray text] | Waiting for a scan to start |
| Golden Sample Capture — in progress | Yellow | Matrix scan running; do not disconnect the harness |
| Golden Sample Capture — success | Green | Sample stored; wire count shown |
| Golden Sample Capture — failure | Red | 0 (or too few) wires detected — reseat harness and retry |
| Test Scan — in progress | Yellow | Matrix scan running; do not disconnect the harness |
| Test Result — PASS | Green | Scan matches golden sample exactly |
| Test Result — OPEN | Red | Expected connection missing |
| Test Result — SHORT | Red | Unexpected connection detected |
| Test Result — MISMATCH | Yellow | Connection present but on the wrong pin/wire |

*Because OPEN and SHORT share red, the fault **label text** (not just color) is what tells them apart on the results line — make sure the operator knows to read the code, not just the color, when a fail shows.*

---

## 5. Getting Started

### 5.1 Powering On
1. [Step-by-step power-on sequence]
2. [What the boot/splash screen looks like]
3. [Self-test behavior, if any]

### 5.2 Connecting a Harness
1. [How to align/seat the connector — orientation cues, keying]
2. [Confirm secure connection — what indicates a bad seat]

### 5.3 First-Time Setup
- [Any one-time configuration: timestamp source (RTC/NTP), SD card formatting, etc.]

---

## 6. Operating Instructions

*This is the core of the guide — write it as a numbered procedure a technician could follow without prior training.*

### 6.1 Capturing a Golden Sample
1. Connect a **known-good, verified** harness to J1/J2.
2. Hold [button name] for 5 seconds to enter Golden Sample Capture mode.
3. The display shows **"CAPTURING…"** in **yellow** while the full matrix scan runs — do not disconnect the harness during this time.
4. On success, the display turns **green**, shows the populated wire count (via `countPopulated()`), and the RGB LED flashes green to confirm.
5. On failure (e.g. zero or unexpectedly few wires detected), the display turns **red** with a **"CAPTURE FAILED — RETRY"** message. Reseat the harness and repeat from step 2.
6. The sample is written to the SD card as the reference for all future scans. [Confirm filename convention / whether a previous golden sample is overwritten or versioned.]

### 6.2 Running a Continuity Test
1. Connect the harness under test to J1/J2.
2. Short-press [button name] to start a scan.
3. The display shows **"SCANNING…"** in **yellow** for the duration of the matrix scan (a few seconds).
4. Results populate the 20-column grid, one line per fault — or a single **"ALL PASS"** line in green if the harness matches the golden sample exactly.
5. See 4.4 for the full color reference and 6.3 for what each result code means.

### 6.3 Interpreting Results
| Result Code | Color | Meaning | Typical Cause |
|---|---|---|---|
| OPEN | [color] | Expected connection not detected | Broken wire, bad crimp, unseated pin |
| SHORT | [color] | Unexpected connection detected | Pinched insulation, solder bridge |
| MISMATCH | [color] | Connection present but wrong pin/wire | Miswire, wrong harness variant |
| PASS | [color] | Matches golden sample | — |

- How J1/J2 physical labels map to displayed fault locations
- What to do with a failed result (retest? log it? flag the harness?)

### 6.4 Saving / Exporting Results
- [SD card file format and naming — see `docs/fault-report-format.md`]
- [How to retrieve files — remove SD card, or on-device transfer method]
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
| Device won't power on | [battery/power cause] | [fix] |
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
| Connectors | J1 / J2 — [type] |
| Umbilical cable | [connector type, AWG, stranded] |
| Power | [battery/voltage/current draw] |
| Dimensions / weight | [ ] |
| Operating temperature | [ ] |
| Storage | microSD, [capacity supported] |

---

## 11. Appendix

- **A. Full pinout / wiring reference** — link to `docs/wiring-reference.md`
- **B. Fault report file format** — link to `docs/fault-report-format.md`
- **C. Glossary**
  - *Golden Sample* — a verified-good harness scan used as the reference for pass/fail comparison
  - *OPEN / SHORT / MISMATCH* — [as defined above]
  - *ACTIVE_WIRE_MASK* — [brief plain-language definition, no need to expose internals unless this guide is for technical users]
- **D. Revision history**

| Version | Date | Changes |
|---|---|---|
| v0.1 | [date] | Initial draft |

- **E. Support / contact**
  - [Repo link: github.com/Hberrill4/uav-harness-tester]
  - [Contact info, if applicable]

---

*Template notes for you (delete before publishing): Sections 6 and 9 are the ones worth writing first and testing against a real user — everything else can stay skeletal until firmware and enclosure design settle. Once you've decided on the timestamp source and SD error behavior, section 6.4 and the troubleshooting table should get updated to match.*
