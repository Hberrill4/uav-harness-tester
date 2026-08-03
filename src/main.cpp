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
bool   resultHeld  = false;   // true while a pass/fail/save/error result is on screen

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
        resultHeld = false; // don't let a stale held result linger across a mode switch
        currentMode = (currentMode == UIMode::NORMAL) ? UIMode::ADMIN : UIMode::NORMAL;
        display_.showIdle(currentMode);
        return;
    }

    // --- Short press: meaning depends on whether a result is currently held ---
    if (button.shortPressEvent()) {

        if (resultHeld) {
            // This press just dismisses the held result and returns to idle.
            resultHeld = false;
            display_.showIdle(currentMode);
            return;
        }

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
                resultHeld = true; // error stays on screen until next press
                return;
            }

            display_.showResult(currentMode, report);
            resultHeld = true; // pass or fail - both now wait for the next press

        } else { // ADMIN mode: capture current wiring as the new reference

            TestResult golden = tester.runScan();

            if (!storage.saveGoldenSample(golden)) {
                display_.showSDWriteError();
                resultHeld = true; // consistent with the logResult failure above
                return;
            }

            display_.showGoldenSaved();
            resultHeld = true;
        }

        // No auto-return-to-idle here anymore -
        // the screen now waits for the next press to dismiss.
    }
}