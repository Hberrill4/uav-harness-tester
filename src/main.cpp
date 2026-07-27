#include <Arduino.h>
#include <time.h>
#include "Config.h"
#include "ButtonManager.h"
#include "MuxController.h"
#include "ContinuityTester.h"
#include "FaultAnalyzer.h"
#include "DisplayManager.h"
#include "StorageManager.h"

ButtonManager    button;
MuxController    mux;
ContinuityTester tester;
DisplayManager   display_;
StorageManager   storage;

UIMode currentMode = UIMode::NORMAL;

void setup() {
    Serial.begin(115200);

    button.begin();
    mux.begin();
    tester.begin(&mux);
    display_.begin();

    if (!storage.begin()) {
        Serial.println("SD init failed - check wiring/CS pin");
    }

    // If you have RTC/NTP available, sync it here so log timestamps are
    // meaningful instead of seconds-since-boot.
}

void loop() {
    button.update();

    // --- Mode toggle: fires once, exactly when the hold crosses 5s ---
    if (button.longPressEvent()) {
        currentMode = (currentMode == UIMode::NORMAL) ? UIMode::ADMIN : UIMode::NORMAL;
        display_.showIdle(currentMode);
    }

    // --- Short press: meaning depends on current mode ---
    if (button.shortPressEvent()) {

        if (currentMode == UIMode::NORMAL) {

            TestResult actual = tester.runScan();

            // Compare against the saved golden sample.
            // Testing is disabled until a golden sample has been captured.
            TestResult expected;

            if (!storage.loadGoldenSample(expected)) {
                display_.showNoGoldenSample();
                delay(2500);
                display_.showIdle(currentMode);
                return;
            }

            FaultReport report = FaultAnalyzer::analyze(actual, expected);
            storage.logResult(report, time(nullptr));
            display_.showResult(currentMode, report);

        } else { // ADMIN mode: capture current wiring as the new reference

            TestResult golden = tester.runScan();
            storage.saveGoldenSample(golden);
            display_.showGoldenSaved();
        }

        // Let the user read the result before returning to idle.
        delay(2500);
        display_.showIdle(currentMode);
    }
}