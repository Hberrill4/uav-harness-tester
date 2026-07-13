#pragma once
#include <Arduino.h>

// Wraps the two mux banks (drive + sense). Selecting "wire index" 0-63
// picks the right mux (index / 16) and the right channel on it (index % 16),
// and enables only that one mux via its EN line while disabling the other 3.
class MuxController {
public:
    void begin();

    // Drives PIN_DRIVE_SIG HIGH through the selected wire index (0-63)
    void selectDrive(uint8_t wireIndex);

    // Connects PIN_SENSE_SIG (an ESP32 input) to the selected wire index (0-63)
    void selectSense(uint8_t wireIndex);

    void disableAllDrive();
    void disableAllSense();

private:
    void setSelectLines(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t channel);
};
