#pragma once
#include <Arduino.h>
#include "Config.h"

// Converts an internal 0-63 wire index into a physical connector+pin label,
// e.g. wireIndex 12 -> "J1-13", wireIndex 45 -> "J2-14".
// Pins are shown 1-indexed within their connector to match physical labeling.
inline String wireLabel(uint8_t wireIndex) {
    const char* connector = (wireIndex < PINS_PER_CONNECTOR) ? CONNECTOR_A_NAME : CONNECTOR_B_NAME;
    uint8_t pin = (wireIndex % PINS_PER_CONNECTOR) + 1;
    return String(connector) + "-" + String(pin);
}
