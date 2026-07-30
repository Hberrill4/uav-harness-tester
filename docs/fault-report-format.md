# Fault report format

## Types

| Type | Meaning | Example |
|---|---|---|
| `OPEN` | No continuity anywhere on this wire | `OPEN:J1-12` — pin 12 on connector J1 is broken/disconnected |
| `SHORT` | Wire reaches its correct pin, but also touches another pin it shouldn't | `SHORT:J1-5-J2-9` — J1 pin 5 and J2 pin 9 are shorted together |
| `MISMATCH` | Wire reaches a pin, but not the one it's supposed to | `MISWIRE:J1-20-J2-1` — the wire meant for J1 pin 20 landed on J2 pin 1 instead |

## Where faults show up

- **LCD (live)**: first 3 faults with type + location, plus a "+N more, see
  log" line if there are more than that. This keeps each fault readable at
  a glance on the 2.8" TFT without needing to scroll; the full list is
  always available in the SD log regardless of what's shown live.
- **SD card log** (`test_log.csv`): every fault from every test, one row
  per test run:

  ```
  timestamp,result,faults
  1737654321,FAIL,OPEN:J1-12;SHORT:J1-5-J2-9;MISWIRE:J1-20-J2-1
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
