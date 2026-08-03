#include "DisplayManager.h"
#include "Config.h"
#include "WireMap.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// --- Text-grid geometry over the raw pixel panel -------------------------
// Adafruit_GFX's built-in font is 6x8px per glyph at text size 1.
// At size 2 that's a 12x16px cell, giving a 20-col x 20-row grid on a
// 240x320 panel. Bump DISP_ROTATION if your enclosure needs a different
// orientation (0/1/2/3 = 0/90/180/270 degrees).
static const uint8_t  TEXT_SIZE     = 2;
static const uint16_t CHAR_W        = 6 * TEXT_SIZE;
static const uint16_t CHAR_H        = 8 * TEXT_SIZE;
static const uint16_t LINE_HEIGHT   = 32;                     // taller than CHAR_H for legibility
static const uint8_t  DISP_ROTATION = 0;
static const uint16_t SCREEN_W      = 240;
static const uint16_t SCREEN_H      = 320;
static const uint8_t  DISP_COLS     = SCREEN_W / CHAR_W;      // 20
static const uint8_t  DISP_ROWS     = SCREEN_H / LINE_HEIGHT; // 10

static Adafruit_ILI9341 tft(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

void DisplayManager::begin() {
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
    tft.begin();
    tft.setRotation(DISP_ROTATION);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextWrap(false);
    tft.setTextSize(TEXT_SIZE);

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);

    showIdle(UIMode::NORMAL);
}

void DisplayManager::clearLine(uint8_t row) {
    tft.fillRect(0, row * LINE_HEIGHT, SCREEN_W, LINE_HEIGHT, ILI9341_BLACK);
}

void DisplayManager::printLine(uint8_t row, const String& text, uint16_t color) {
    if (row >= DISP_ROWS) return; // guard against writing off-panel

    clearLine(row);
    tft.setCursor(0, row * LINE_HEIGHT + (LINE_HEIGHT - CHAR_H) / 2); // center glyph in the taller row
    tft.setTextColor(color, ILI9341_BLACK);
    tft.print(text.substring(0, DISP_COLS)); // clip, same intent as old LCD_COLS clip
}
void DisplayManager::setStatusColor(UIStatus status) {
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
            digitalWrite(PIN_LED_B, HIGH); // Purple
            break;
        case UIStatus::IDLE:
        default:
            break;
    }
}

void DisplayManager::showIdle(UIMode mode) {
    tft.fillScreen(ILI9341_BLACK);

    if (mode == UIMode::ADMIN) {
        printLine(0, "[ADMIN MODE]", ILI9341_CYAN);
        printLine(1, "Connect golden");
        printLine(2, "sample. press");
        printLine(3, "button to save");
        printLine(4, "harness as new");
        printLine(5, "reference sample");
        printLine(6, "or hold button");
        printLine(7, "for 5 secs");
        printLine(8, "to exit admin mode");

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
    tft.fillScreen(ILI9341_BLACK);

    if (report.allPass) {
        printLine(0, "RESULT: PASS", ILI9341_GREEN);
        printLine(1, "Harness PASSED", ILI9341_GREEN);
        printLine(DISP_ROWS - 1, "Press to continue", ILI9341_WHITE);
        setStatusColor(UIStatus::PASS);
        return;
    }

    printLine(0, "RESULT: FAIL (" + String(report.faultCount) + ")", ILI9341_RED);
    setStatusColor(UIStatus::FAIL);

    uint8_t faultRows   = DISP_ROWS - 1;
    bool    needSummary = report.faultCount > faultRows;
    uint8_t faultsToShow = needSummary ? (faultRows - 1) : report.faultCount;

    for (uint8_t i = 0; i < faultsToShow; i++) {
        printLine(i + 1, faultLine(report.faults[i]), ILI9341_RED);
    }

    if (needSummary) {
        uint8_t remaining = report.faultCount - faultsToShow;
        printLine(DISP_ROWS - 1, "+" + String(remaining) + " more see log", ILI9341_YELLOW);
    }
    printLine(DISP_ROWS - 1, "Press to continue", ILI9341_WHITE);
}

void DisplayManager::showGoldenSaved() {
    tft.fillScreen(ILI9341_BLACK);
    printLine(0, "[ADMIN MODE]");
    printLine(1, "Golden sample", ILI9341_BLUE);
    printLine(2, "saved OK", ILI9341_BLUE);
    printLine(3, "Press to return");
    setStatusColor(UIStatus::GOLDEN_SAVED);
}

void DisplayManager::showNoGoldenSample() {
    tft.fillScreen(ILI9341_BLACK);
    printLine(0, "NO REFERENCE", ILI9341_MAGENTA);
    printLine(1, "Hold 5 sec");
    printLine(2, "Enter ADMIN");
    printLine(3, "Save harness");
    setStatusColor(UIStatus::NO_GOLDEN);
}

void DisplayManager::showSDWriteError() {
    tft.fillScreen(ILI9341_BLACK);
    printLine(0, "SD WRITE ERROR", ILI9341_RED);
    printLine(1, "Result not");
    printLine(2, "saved");
    printLine(3, "Check SD card");
    setStatusColor(UIStatus::FAIL);
}

void DisplayManager::showSelfTest() {
    tft.fillScreen(ILI9341_BLACK);
    printLine(0, "SELF TEST");
    printLine(1, "Checking...");
    setStatusColor(UIStatus::IDLE);
}