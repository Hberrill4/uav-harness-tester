#include "DisplayManager.h"
#include "Config.h"
#include "WireMap.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

void DisplayManager::begin() {
    Wire.begin();
    lcd.init();
    lcd.backlight();

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);

    showIdle(UIMode::NORMAL);
}

void DisplayManager::printLine(uint8_t row, const String& text) {
    lcd.setCursor(0, row);
    String padded = text;
    while (padded.length() < LCD_COLS)
        padded += ' ';
    lcd.print(padded.substring(0, LCD_COLS));
}

void DisplayManager::setStatusColor(UIStatus status) {

    // Turn everything off first
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);

    switch (status) {

        case UIStatus::PASS:
            digitalWrite(PIN_LED_G, HIGH);
            break;

        case UIStatus::FAIL:
            digitalWrite(PIN_LED_R, HIGH);
            break;

        case UIStatus::GOLDEN_SAVED:
            digitalWrite(PIN_LED_B, HIGH);
            break;

        case UIStatus::NO_GOLDEN:
            digitalWrite(PIN_LED_R, HIGH);
            digitalWrite(PIN_LED_B, HIGH);     // Purple
            break;

        case UIStatus::IDLE:
        default:
            break;
    }
}

void DisplayManager::showIdle(UIMode mode) {

    lcd.clear();

    if (mode == UIMode::ADMIN) {

        printLine(0, "[ADMIN MODE]");
        printLine(1, "Connect golden");
        printLine(2, "sample, press");
        printLine(3, "button to save");

        digitalWrite(PIN_LED_R, LOW);
        digitalWrite(PIN_LED_G, LOW);
        digitalWrite(PIN_LED_B, HIGH);

    } else {

        printLine(0, "UAV Harness Test");
        printLine(1, "Connect harness");
        printLine(2, "Press button");
        printLine(3, "to begin test");

        setStatusColor(UIStatus::IDLE);
    }
}

String DisplayManager::faultLine(const WireFault& f) {

    switch (f.type) {

        case FaultType::OPEN:
            return "OPEN   " + wireLabel(f.wireIndex);

        case FaultType::SHORT:
            return "SHORT  " + wireLabel(f.wireIndex) + "<->" + wireLabel(f.partnerWire);

        case FaultType::MISMATCH:
            return "MISWIRE " + wireLabel(f.wireIndex) + "->" + wireLabel(f.partnerWire);
    }

    return "";
}

void DisplayManager::showResult(UIMode mode, const FaultReport& report) {

    lcd.clear();

    if (report.allPass) {

        printLine(0, "RESULT: PASS");
        printLine(1, "Harness PASSED");

        setStatusColor(UIStatus::PASS);
        return;
    }

    printLine(0, "RESULT: FAIL (" + String(report.faultCount) + ")");
    setStatusColor(UIStatus::FAIL);

    // One LCD row is used for the title.
    uint8_t faultRows = LCD_ROWS - 1;

    // If there are more faults than rows,
    // reserve the final row for the "+N more" message.
    bool needSummary = report.faultCount > faultRows;

    uint8_t faultsToShow =
        needSummary ? (faultRows - 1) : report.faultCount;

    for (uint8_t i = 0; i < faultsToShow; i++) {
        printLine(i + 1, faultLine(report.faults[i]));
    }

    if (needSummary) {

        uint8_t remaining = report.faultCount - faultsToShow;

        printLine(
            LCD_ROWS - 1,
            "+" + String(remaining) + " more see log"
        );
    }
}

void DisplayManager::showGoldenSaved() {

    lcd.clear();

    printLine(0, "[ADMIN MODE]");
    printLine(1, "Golden sample");
    printLine(2, "saved OK");

    setStatusColor(UIStatus::GOLDEN_SAVED);
}

void DisplayManager::showNoGoldenSample() {

    lcd.clear();

    printLine(0, "NO REFERENCE");
    printLine(1, "Hold 5 sec");
    printLine(2, "Enter ADMIN");
    printLine(3, "Save harness");

    setStatusColor(UIStatus::NO_GOLDEN);
}
void DisplayManager::showSDWriteError()
{
    lcd.clear();

    printLine(0, "SD WRITE ERROR");
    printLine(1, "Result not");
    printLine(2, "saved");
    printLine(3, "Check SD card");

    setStatusColor(UIStatus::FAIL);
}
void DisplayManager::showSelfTest()
{
    lcd.clear();

    printLine(0, "SELF TEST");
    printLine(1, "Checking...");
    printLine(2, "");
    printLine(3, "");

    setStatusColor(UIStatus::IDLE);
}