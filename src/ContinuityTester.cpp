#include "ContinuityTester.h"
#include "Config.h"

void ContinuityTester::begin(MuxController* mux) {
    _mux = mux;
}

TestResult ContinuityTester::runScan() {
    TestResult result;

    for (uint8_t driveWire = 0; driveWire < NUM_WIRES; driveWire++) {
        _mux->selectDrive(driveWire);
        digitalWrite(PIN_DRIVE_SIG, HIGH);
        delayMicroseconds(50); // let the drive side settle

        uint64_t mask = 0;
        for (uint8_t senseWire = 0; senseWire < NUM_WIRES; senseWire++) {
            _mux->selectSense(senseWire);
            delayMicroseconds(30); // let the sense mux settle after switching
            if (digitalRead(PIN_SENSE_SIG) == HIGH) {
                mask |= (1ULL << senseWire);
            }
        }

        result.connections[driveWire] = mask;
        digitalWrite(PIN_DRIVE_SIG, LOW);
    }

    _mux->disableAllDrive();
    _mux->disableAllSense();
    return result;
}

TestResult ContinuityTester::identityExpectation() {
    TestResult expected;
    for (uint8_t i = 0; i < NUM_WIRES; i++) {
        expected.connections[i] = (1ULL << i);
    }
    return expected;
}
