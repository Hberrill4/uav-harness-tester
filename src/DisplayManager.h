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
    void showSDWriteError();
    void showSelfTest();

private:
    // color defaults to white-on-black; callers can override to color-code
    // pass/fail/warning lines without changing every existing call site.
    void printLine(uint8_t row, const String& text, uint16_t color = 0xFFFF);
    void clearLine(uint8_t row);
    String faultLine(const WireFault& f);
};
