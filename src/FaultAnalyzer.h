#pragma once
#include <cstdint>
#include "TestResult.h"

enum class FaultType { OPEN, SHORT, MISMATCH };

struct WireFault {
    uint8_t   wireIndex;     // 0-63: the driven wire that has a problem
    FaultType type;
    uint8_t   partnerWire;   // for SHORT/MISMATCH: one offending wire it's unexpectedly tied to
    uint64_t  extraMask;     // for SHORT/MISMATCH: ALL unexpected wires it's tied to (bitmask)
};

struct FaultReport {
    WireFault faults[NUM_WIRES]; // worst case one fault per wire
    uint8_t   faultCount;
    bool      allPass;
};

class FaultAnalyzer {
public:
    // Compares a live scan against an expected connectivity matrix (either
    // a captured golden sample, or ContinuityTester::identityExpectation()).
    static FaultReport analyze(const TestResult& actual, const TestResult& expected);
};
