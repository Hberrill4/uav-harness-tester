#pragma once
#include <Arduino.h>
#include "Config.h"
#include "MuxController.h"
#include "TestResult.h"

// Full connectivity matrix: connections[i] is a 64-bit mask where bit j is
// set if, while driving wire i, sense wire j reads continuity.
// A healthy straight-through harness has exactly one bit set per row (bit i).


class ContinuityTester {
public:
    void begin(MuxController* mux);
    TestResult runScan();
    static TestResult identityExpectation();

private:
    MuxController* _mux;
};