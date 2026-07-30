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

void setup()
{
    Serial.begin(115200);
    
Serial.println("1");
button.begin();

Serial.println("2");
mux.begin();

Serial.println("3");
tester.begin(&mux);

Serial.println("4");
display_.begin();

Serial.println("5");

    display_.showSelfTest();

    if (!storage.begin()) {

        display_.showSDWriteError();

        while (true) {
            delay(100);
        }
    }

    TestResult temp;

    if (!storage.loadGoldenSample(temp)) {
        display_.showNoGoldenSample();
    }
    else {
        display_.showIdle(currentMode);
    }
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

            // Testing is disabled until a golden sample exists - check first
            // so we don't waste a ~0.3s scan when there's nothing to compare against.
            TestResult expected;
            if (!storage.loadGoldenSample(expected)) {
                display_.showNoGoldenSample();
                delay(2500);
                display_.showIdle(currentMode);
                return;
            }

            TestResult actual = tester.runScan();
            FaultReport report = FaultAnalyzer::analyze(actual, expected);

            if (!storage.logResult(report, time(nullptr))) {
                display_.showSDWriteError();
                return; // error stays on screen until next press
            }

            display_.showResult(currentMode, report);

        } else { // ADMIN mode: capture current wiring as the new reference

            TestResult golden = tester.runScan();

            if (!storage.saveGoldenSample(golden)) {
                display_.showSDWriteError();
                return; // consistent with the logResult failure above
            }

            display_.showGoldenSaved();
        }

        // Reached only on success - let the operator read the result
        // before the display auto-returns to idle.
        delay(2500);
        display_.showIdle(currentMode);
    }
}
