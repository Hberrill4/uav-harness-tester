#pragma once
#include <Arduino.h>
#include "MuxController.h"

// Full connectivity matrix: connections[i] is a 64-bit mask where bit j is
// set if, while driving wire i, sense wire j reads continuity.
// A healthy straight-through harness has exactly one bit set per row (bit i).
struct TestResult {
    uint64_t connections[NUM_WIRES];
};

class ContinuityTester {
public:
    void begin(MuxController* mux);

    // Drives each wire in turn and reads continuity against ALL 64 sense
    // lines (not just the matching one) so shorts and mismatches are visible,
    // not just opens. 64 x 64 = 4096 reads per full scan.
    TestResult runScan();

    // Default "expected" matrix when no golden sample has been captured yet:
    // assumes a straight-through harness (wire i should only reach pin i).
    static TestResult identityExpectation();
};
