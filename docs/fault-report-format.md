# Fault report format

## Types

| Type | Meaning | Example |
|---|---|---|
| `OPEN` | No continuity anywhere on this wire | `OPEN:12` — wire 12 is broken/disconnected |
| `SHORT` | Wire reaches its correct pin, but also touches another pin it shouldn't | `SHORT:5-9` — wires 5 and 9 are shorted together |
| `MISMATCH` | Wire reaches a pin, but not the one it's supposed to | `MISWIRE:20-33` — wire 20 landed on pin 33's position |

## Where faults show up

- **LCD (live)**: first 3 faults with type + location, plus a "+N more, see
  log" line if there are more than that. This is a hardware space
  constraint (20x4 display), not a data limitation.
- **SD card log** (`test_log.csv`): every fault from every test, one row
  per test run:

  ```
  timestamp,result,faults
  1737654321,FAIL,OPEN:12;SHORT:5-9;MISWIRE:20-33
  1737654890,PASS,
  ```

## How faults are determined

Every test compares the live 64x64 connectivity scan against an "expected"
matrix:

- If a **golden sample** has been captured (admin mode), that becomes the
  expected matrix.
- Otherwise, the firmware assumes a straight-through harness (wire N should
  only ever reach pin N) — see `ContinuityTester::identityExpectation()`.

Capturing a golden sample is recommended for any harness that isn't a
simple 1:1 pass-through, since the identity assumption will otherwise flag
correct-but-nonstandard wiring as mismatches.
