#pragma once
#include <Arduino.h>
#include "ContinuityTester.h"
#include "FaultAnalyzer.h"

class StorageManager {
public:
    bool begin();

    // Appends one CSV row per test: test_number, timestamp, PASS/FAIL, then one
    // semicolon-separated fault descriptor per fault, e.g.
    // "OPEN:12;SHORT:5-9;MISWIRE:20-33"
    bool logResult(const FaultReport& report, time_t timestamp);

    // Appends a marker row noting a new golden sample was captured, so the
    // log shows exactly when the reference harness changed.
    bool logGoldenSampleEvent(time_t timestamp);

    // Persists the full 64x64 connectivity matrix (512 bytes) as the
    // reference to compare future scans against.
    bool saveGoldenSample(const TestResult& result); // was void
    bool loadGoldenSample(TestResult& outResult); // false if none saved yet

private:
    uint32_t _nextTestNumber = 1;
};