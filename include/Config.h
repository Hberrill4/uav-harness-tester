#pragma once
#include <Arduino.h>

// ---------------------------------------------------------------------------
// HARNESS GEOMETRY
// ---------------------------------------------------------------------------
static const uint8_t  NUM_WIRES        = 64;
static const uint8_t  MUXES_PER_BANK   = 4;
static const uint8_t  CHANNELS_PER_MUX = 16;

static const uint8_t  PINS_PER_CONNECTOR = 32;
static const char*    CONNECTOR_A_NAME   = "J1";
static const char*    CONNECTOR_B_NAME   = "J2";

static const uint64_t ACTIVE_WIRE_MASK = 0xFFFFFFFFFFFFFFFFULL;

// ---------------------------------------------------------------------------
// BOARD: Waveshare ESP32-S3-DEV-KIT-N8R8 / NXRX / NxR8 (Octal PSRAM)
// Pin-header compatible with the official ESP32-S3-DevKitC-1.
// ---------------------------------------------------------------------------
// Confirmed restrictions for THIS board:
//   - GPIO22-25 don't exist on the S3 chip.
//   - GPIO26-32 = internal SPI flash bus            -> never use.
//   - GPIO35-37 = internal Octal PSRAM bus (N8R8)    -> never use.
//   - GPIO43/44 = UART0, wired to the onboard CH343 USB-UART bridge
//     (your flashing/Serial path)                   -> avoid for I/O.
//   - GPIO0/3/45/46 = strapping pins (boot mode, JTAG, flash voltage)
//                                                    -> avoid for I/O.
//   - GPIO19/20 = native USB D-/D+, also routed through the onboard CH334
//     USB switch on this board's single USB-C port   -> left unused below
//     to avoid any ambiguity; free for future expansion if you confirm
//     native USB isn't active.
//   - GPIO48 drives this board's onboard addressable RGB LED. Used below
//     for PIN_SENSE_SIG (a pure input) - the onboard LED will flicker
//     during scans as a side effect. Cosmetic only, no functional impact.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// BUTTON
// ---------------------------------------------------------------------------
static const uint8_t  PIN_BUTTON        = 4;
static const uint32_t DEBOUNCE_MS       = 30;
static const uint32_t LONG_PRESS_MS     = 5000;

// ---------------------------------------------------------------------------
// MUX SELECT LINES
// ---------------------------------------------------------------------------
static const uint8_t PIN_DRIVE_S0   = 13;
static const uint8_t PIN_DRIVE_S1   = 12;
static const uint8_t PIN_DRIVE_S2   = 14;
static const uint8_t PIN_DRIVE_S3   = 15;
static const uint8_t PIN_DRIVE_SIG  = 1;
static const uint8_t PIN_DRIVE_EN[MUXES_PER_BANK] = {6, 7, 8, 9};

static const uint8_t PIN_SENSE_S0   = 16;
static const uint8_t PIN_SENSE_S1   = 17;
static const uint8_t PIN_SENSE_S2   = 5;
static const uint8_t PIN_SENSE_S3   = 18;
static const uint8_t PIN_SENSE_SIG  = 48;   // moved off GPIO46 (strapping pin)
static const uint8_t PIN_SENSE_EN[MUXES_PER_BANK] = {21, 10, 11, 38};

// ---------------------------------------------------------------------------
// SHARED SPI BUS (SD card + TFT, each with its own CS)
// ---------------------------------------------------------------------------
static const uint8_t PIN_SPI_SCK   = 33;    // moved off GPIO19 (native USB)
static const uint8_t PIN_SPI_MOSI  = 34;    // moved off GPIO20 (native USB)
static const uint8_t PIN_SPI_MISO  = 47;

// ---------------------------------------------------------------------------
// SD CARD (SPI)
// ---------------------------------------------------------------------------
static const uint8_t PIN_SD_CS = 39;

// ---------------------------------------------------------------------------
// TFT DISPLAY (2.8" ILI9341, SPI)
// ---------------------------------------------------------------------------
static const uint8_t PIN_TFT_CS  = 40;
static const uint8_t PIN_TFT_DC  = 41;
static const uint8_t PIN_TFT_RST = 42;

// ---------------------------------------------------------------------------
// STATUS RGB LED (discrete LED, separate from the onboard addressable one)
// ---------------------------------------------------------------------------
static const uint8_t PIN_LED_R = 2;
static const uint8_t PIN_LED_G = 0;    // strapping pin; safe as an OUTPUT
static const uint8_t PIN_LED_B = 3;    // strapping pin; safe as an OUTPUT

// GPIO19, GPIO20, GPIO46 intentionally left unassigned - see notes above.

// ---------------------------------------------------------------------------
// FILES
// ---------------------------------------------------------------------------
static const char* LOG_FILE_PATH    = "/test_log.csv";
static const char* GOLDEN_FILE_PATH = "/golden_sample.bin";