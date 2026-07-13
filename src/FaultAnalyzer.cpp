#include "FaultAnalyzer.h"

static inline uint8_t firstSetBit(uint64_t mask) {
    return (uint8_t)__builtin_ctzll(mask); // count trailing zeros -> index of lowest set bit
}

FaultReport FaultAnalyzer::analyze(const TestResult& actual, const TestResult& expected) {
    FaultReport report;
    report.faultCount = 0;
    report.allPass = true;

    for (uint8_t i = 0; i < NUM_WIRES; i++) {
        uint64_t expectedMask = expected.connections[i];
        uint64_t actualMask   = actual.connections[i];

        bool     hasExpectedConn = (actualMask & expectedMask) != 0;
        uint64_t extra           = actualMask & ~expectedMask; // continuity we did NOT expect

        WireFault f;
        f.wireIndex   = i;
        f.partnerWire = 0;
        f.extraMask   = extra;

        if (actualMask == 0) {
            // No continuity anywhere on this wire at all -> broken/open wire
            f.type = FaultType::OPEN;
            report.faults[report.faultCount++] = f;
            report.allPass = false;

        } else if (!hasExpectedConn && extra != 0) {
            // Continuity exists, but never to the wire it's supposed to reach ->
            // this wire is landed on the wrong pin (a swap/mismatch), not just shorted
            f.type        = FaultType::MISMATCH;
            f.partnerWire = firstSetBit(extra);
            report.faults[report.faultCount++] = f;
            report.allPass = false;

        } else if (hasExpectedConn && extra != 0) {
            // Correct connection is present, AND it's also touching something else -> short
            f.type        = FaultType::SHORT;
            f.partnerWire = firstSetBit(extra);
            report.faults[report.faultCount++] = f;
            report.allPass = false;
        }
        // else: actualMask == expectedMask (or a subset match with no extras) -> pass
    }

    return report;
}
