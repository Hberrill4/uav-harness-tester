#include "ButtonManager.h"
#include "Config.h"

void ButtonManager::begin() {
    pinMode(PIN_BUTTON, INPUT_PULLUP); // button pulls LOW when pressed
    _lastReading = digitalRead(PIN_BUTTON) == LOW;
}

void ButtonManager::update() {
    _shortPressEvent = false;
    _longPressEvent  = false;

    bool reading = (digitalRead(PIN_BUTTON) == LOW); // true = pressed
    uint32_t now = millis();

    // Debounce: only accept a change after it's been stable for DEBOUNCE_MS
    if (reading != _lastReading) {
        _lastEdgeMs = now;
        _lastReading = reading;
    }

    if ((now - _lastEdgeMs) > DEBOUNCE_MS) {
        if (reading && !_pressed) {
            // Fresh press begins
            _pressed = true;
            _pressStartMs = now;
            _longFiredThisPress = false;
        } else if (!reading && _pressed) {
            // Released
            _pressed = false;
            uint32_t heldFor = now - _pressStartMs;
            if (!_longFiredThisPress && heldFor < LONG_PRESS_MS) {
                _shortPressEvent = true; // qualifies as a short press/click
            }
            // if _longFiredThisPress, the mode toggle already happened on the way down;
            // releasing does nothing further.
        }
    }

    // While still held, check whether we've crossed the long-press threshold
    if (_pressed && !_longFiredThisPress) {
        if ((now - _pressStartMs) >= LONG_PRESS_MS) {
            _longFiredThisPress = true;
            _longPressEvent = true; // fires immediately at the 5s mark, no need to wait for release
        }
    }
}

bool ButtonManager::shortPressEvent() { return _shortPressEvent; }
bool ButtonManager::longPressEvent()  { return _longPressEvent; }
