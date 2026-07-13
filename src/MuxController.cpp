#include "MuxController.h"
#include "Config.h"

// NOTE: 74HC4067 EN pin is active LOW on most breakout boards. Adjust the
// HIGH/LOW polarity below if your particular mux/breakout differs.

void MuxController::begin() {
    pinMode(PIN_DRIVE_S0, OUTPUT);
    pinMode(PIN_DRIVE_S1, OUTPUT);
    pinMode(PIN_DRIVE_S2, OUTPUT);
    pinMode(PIN_DRIVE_S3, OUTPUT);
    pinMode(PIN_DRIVE_SIG, OUTPUT);

    pinMode(PIN_SENSE_S0, OUTPUT);
    pinMode(PIN_SENSE_S1, OUTPUT);
    pinMode(PIN_SENSE_S2, OUTPUT);
    pinMode(PIN_SENSE_S3, OUTPUT);
    pinMode(PIN_SENSE_SIG, INPUT_PULLDOWN); // reads HIGH only if continuity exists

    for (uint8_t i = 0; i < MUXES_PER_BANK; i++) {
        pinMode(PIN_DRIVE_EN[i], OUTPUT);
        digitalWrite(PIN_DRIVE_EN[i], HIGH); // HIGH = disabled
        pinMode(PIN_SENSE_EN[i], OUTPUT);
        digitalWrite(PIN_SENSE_EN[i], HIGH);
    }

    digitalWrite(PIN_DRIVE_SIG, LOW);
}

void MuxController::setSelectLines(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t channel) {
    digitalWrite(s0, (channel >> 0) & 0x01);
    digitalWrite(s1, (channel >> 1) & 0x01);
    digitalWrite(s2, (channel >> 2) & 0x01);
    digitalWrite(s3, (channel >> 3) & 0x01);
}

void MuxController::selectDrive(uint8_t wireIndex) {
    uint8_t muxIndex = wireIndex / CHANNELS_PER_MUX;
    uint8_t channel  = wireIndex % CHANNELS_PER_MUX;

    setSelectLines(PIN_DRIVE_S0, PIN_DRIVE_S1, PIN_DRIVE_S2, PIN_DRIVE_S3, channel);

    for (uint8_t i = 0; i < MUXES_PER_BANK; i++) {
        digitalWrite(PIN_DRIVE_EN[i], (i == muxIndex) ? LOW : HIGH);
    }
}

void MuxController::selectSense(uint8_t wireIndex) {
    uint8_t muxIndex = wireIndex / CHANNELS_PER_MUX;
    uint8_t channel  = wireIndex % CHANNELS_PER_MUX;

    setSelectLines(PIN_SENSE_S0, PIN_SENSE_S1, PIN_SENSE_S2, PIN_SENSE_S3, channel);

    for (uint8_t i = 0; i < MUXES_PER_BANK; i++) {
        digitalWrite(PIN_SENSE_EN[i], (i == muxIndex) ? LOW : HIGH);
    }
}

void MuxController::disableAllDrive() {
    for (uint8_t i = 0; i < MUXES_PER_BANK; i++) digitalWrite(PIN_DRIVE_EN[i], HIGH);
}

void MuxController::disableAllSense() {
    for (uint8_t i = 0; i < MUXES_PER_BANK; i++) digitalWrite(PIN_SENSE_EN[i], HIGH);
}
