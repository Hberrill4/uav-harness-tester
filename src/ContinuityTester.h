#pragma once
#include <Arduino.h>
#include "Config.h"
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
    TestResult runScan();
    static TestResult identityExpectation();

private:
    MuxController* _mux;
};