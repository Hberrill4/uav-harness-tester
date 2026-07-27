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
    while (padded.length() < LCD_COLS) padded += ' ';
    lcd.print(padded.substring(0, LCD_COLS));
}

void DisplayManager::setStatusColor(UIStatus status) {
    // Common-cathode assumption: HIGH = on. Invert if your LED is common-anode.
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);

    switch (status) {
        case UIStatus::PASS:         digitalWrite(PIN_LED_G, HIGH); break;
        case UIStatus::FAIL:         digitalWrite(PIN_LED_R, HIGH); break;
        case UIStatus::GOLDEN_SAVED: digitalWrite(PIN_LED_B, HIGH); break;
        case UIStatus::IDLE:         break;
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
        digitalWrite(PIN_LED_B, HIGH); // blue = admin
    } else {
        printLine(0, "UAV Harness Test");
        printLine(1, "Connect harness");
        printLine(2, "Press button");
        printLine(3, "to test 64 wires");
        digitalWrite(PIN_LED_R, LOW);
        digitalWrite(PIN_LED_G, LOW);
        digitalWrite(PIN_LED_B, LOW);
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
        printLine(1, "All 64 wires OK");
        setStatusColor(UIStatus::PASS);
        return;
    }

    printLine(0, "RESULT: FAIL (" + String(report.faultCount) + ")");
    setStatusColor(UIStatus::FAIL);

// LCD has one title row, so the remaining rows can show faults.
// If there are more faults than fit, reserve the last row for a
// "+N more" message.
uint8_t faultRows = LCD_ROWS - 1;

bool needSummary = report.faultCount > faultRows;

uint8_t faultsToShow =
    needSummary ? (faultRows - 1) : report.faultCount;

// Show the faults that fit
for (uint8_t i = 0; i < faultsToShow; i++) {
    printLine(i + 1, faultLine(report.faults[i]));
}

// Show how many more faults exist
if (needSummary) {
    uint8_t remaining = report.faultCount - faultsToShow;
    printLine(LCD_ROWS - 1,
              "+" + String(remaining) + " more see log");
}
}

void DisplayManager::showGoldenSaved() {
    lcd.clear();
    printLine(0, "[ADMIN MODE]");
    printLine(1, "Golden sample");
    printLine(2, "saved OK");
    setStatusColor(UIStatus::GOLDEN_SAVED);
}
