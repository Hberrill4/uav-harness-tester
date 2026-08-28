# Design Enclosure

# Harness Tester Enclosure — OnShape Build Guide

A note before you start: OnShape's toolbar occasionally shifts icon positions between updates. Everything below reflects the standard, current toolset — if an icon isn't exactly where described, hover over the toolbar for tooltips, or use the search (magnifying glass icon, top toolbar) and type the tool name.

---

## Before You Start: Measurements Checklist

Fill in the placeholders before you get deep into modeling — you'll need most of these by Section 3.

| Item | Value | Status |
|---|---|---|
| Main PCB length | 5905.5 mm | measured from kiCAD |
| Main PCB width | 10236.2 mm | measured from kiCAD |
| Main PCB mounting hole positions (X,Y from corner) | 236.2 mm | measured from kiCAD |
| Tallest component stack height (ESP32-S3 DevKit + header socket) | `[STACK_HEIGHT]` mm | **Measure physical stack** |
| Wall thickness | 2.5 mm | Suggested default — adjust if you want |
| Standoff height (PCB to floor) | 5.5 mm | Suggested default |
| D-sub flange hole spacing | 63.5 mm | Confirmed (HARTING) |
| D-sub flange hole diameter | 3.1 mm | Confirmed (HARTING) |
| D-sub shell cutout size | Use imported STEP bounding box | Don't hand-measure — see Section 5 |
| Screen mounting hole spacing | 57.2 × 74.9 mm | Confirmed (Adafruit) |
| Screen PCB thickness | 1.6 mm (bare) / 7.9 mm (full stack) | Confirmed (Adafruit/RS) |
| Screen active area | 69 x 51 mm | Measured physical screen |
| Button cutout diameter | 30 mm | Confirmed (Sanwa) |
| Wall thickness at button | 2.0–3.7 mm | Confirmed — must match your global wall thickness or get a local pad |

