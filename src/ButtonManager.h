#pragma once
#include <Arduino.h>

// Reports two distinct, non-blocking events:
//   - shortPress()   : press+release under LONG_PRESS_MS      -> "do the action"
//   - longPressFired(): held past LONG_PRESS_MS (fires ONCE per hold) -> "toggle mode"
//
// Call update() every loop() iteration. Nothing here blocks or delays.
class ButtonManager {
public:
    void begin();
    void update();

    bool shortPressEvent();   // true for one update() cycle after a qualifying short press
    bool longPressEvent();    // true for one update() cycle when the 5s hold threshold is crossed
    bool isPressed() const { return _pressed; }

private:
    bool     _pressed        = false;
    bool     _lastReading    = false;
    uint32_t _lastEdgeMs     = 0;
    uint32_t _pressStartMs   = 0;
    bool     _longFiredThisPress = false;

    bool _shortPressEvent = false;
    bool _longPressEvent  = false;
};
