#pragma once
#include <Arduino.h>
#include "FaultAnalyzer.h"

enum class UIMode { NORMAL, ADMIN };

enum class UIStatus {
    IDLE,
    PASS,
    FAIL,
    GOLDEN_SAVED,
    NO_GOLDEN
};

class DisplayManager {
public:
    void begin();
    void showIdle(UIMode mode);
    void showResult(UIMode mode, const FaultReport& report);
    void showGoldenSaved();
    void showNoGoldenSample();
    void setStatusColor(UIStatus status);

private:
    void printLine(uint8_t row, const String& text);
    String faultLine(const WireFault& f);
};
