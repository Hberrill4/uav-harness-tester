#pragma once
#include <Arduino.h>
// ---------------------------------------------------------------------------
// HARNESS GEOMETRY
// ---------------------------------------------------------------------------
static const uint8_t  NUM_WIRES        = 64;   // total wires under test
static const uint8_t  MUXES_PER_BANK   = 4;    // 4 muxes per bank
static const uint8_t  CHANNELS_PER_MUX = 16;   // 74HC4067-style 16:1 mux

// Two identical connectors, 32 pins each. Wire index 0-31 = connector A,
// 32-63 = connector B. Update the names/count here if your harness changes.
static const uint8_t  PINS_PER_CONNECTOR = 32;
static const char*    CONNECTOR_A_NAME   = "J1";
static const char*    CONNECTOR_B_NAME   = "J2";

// ---------------------------------------------------------------------------
// BUTTON
// ---------------------------------------------------------------------------
static const uint8_t  PIN_BUTTON        = 4;     // active LOW, INPUT_PULLUP
static const uint32_t DEBOUNCE_MS       = 30;
static const uint32_t LONG_PRESS_MS     = 5000;  // hold time to toggle admin mode

// ---------------------------------------------------------------------------
// MUX SELECT LINES (shared across all 4 muxes in a bank -> S0..S3)
// ---------------------------------------------------------------------------
// DRIVE bank = outputs a HIGH onto the selected wire
static const uint8_t PIN_DRIVE_S0   = 13;
static const uint8_t PIN_DRIVE_S1   = 12;
static const uint8_t PIN_DRIVE_S2   = 14;
static const uint8_t PIN_DRIVE_S3   = 27;
static const uint8_t PIN_DRIVE_SIG  = 26;   // shared SIG/COM, only one EN active at a time
static const uint8_t PIN_DRIVE_EN[MUXES_PER_BANK] = {25, 33, 32, 35};

// SENSE bank = reads back continuity on the far end of the same wire
static const uint8_t PIN_SENSE_S0   = 16;
static const uint8_t PIN_SENSE_S1   = 17;
static const uint8_t PIN_SENSE_S2   = 5;
static const uint8_t PIN_SENSE_S3   = 18;
static const uint8_t PIN_SENSE_SIG  = 19;
static const uint8_t PIN_SENSE_EN[MUXES_PER_BANK] = {21, 22, 23, 15};

// ---------------------------------------------------------------------------
// SD CARD (SPI)
// ---------------------------------------------------------------------------
static const uint8_t PIN_SD_CS = 15;  // adjust if it conflicts with EN pins above

// ---------------------------------------------------------------------------
// LCD (I2C)
// ---------------------------------------------------------------------------
static const uint8_t LCD_I2C_ADDR = 0x27;
static const uint8_t LCD_COLS     = 20;
static const uint8_t LCD_ROWS     = 4;

// ---------------------------------------------------------------------------
// STATUS RGB LED (or use LCD backlight color if your LCD supports RGB)
// ---------------------------------------------------------------------------
static const uint8_t PIN_LED_R = 2;
static const uint8_t PIN_LED_G = 0;
static const uint8_t PIN_LED_B = 15;

// ---------------------------------------------------------------------------
// FILES
// ---------------------------------------------------------------------------
static const char* LOG_FILE_PATH    = "/test_log.csv";
static const char* GOLDEN_FILE_PATH = "/golden_sample.bin";
