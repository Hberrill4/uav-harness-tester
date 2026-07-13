#pragma once
#include <Arduino.h>
#include "FaultAnalyzer.h"

enum class UIMode { NORMAL, ADMIN };
enum class UIStatus { IDLE, PASS, FAIL, GOLDEN_SAVED };

class DisplayManager {
public:
    void begin();
    void showIdle(UIMode mode);
    void showResult(UIMode mode, const FaultReport& report);
    void showGoldenSaved();
    void setStatusColor(UIStatus status); // drives the RGB LED: green/red/blue

private:
    void printLine(uint8_t row, const String& text);
    String faultLine(const WireFault& f); // e.g. "OPEN  W12" or "SHORT W5<->W9"
};
