#include "StorageManager.h"
#include "Config.h"
#include "WireMap.h"
#include <SD.h>
#include <SPI.h>

bool StorageManager::begin() {
    if (!SD.begin(PIN_SD_CS, SPI)) {
        return false;
    }

    bool isNewFile = !SD.exists(LOG_FILE_PATH);

    if (isNewFile) {
        File f = SD.open(LOG_FILE_PATH, FILE_WRITE);
        if (f) {
            f.println("test_number,timestamp,result,faults");
            f.close();
        }
        _nextTestNumber = 1;
    } else {
        // Count existing data rows so numbering survives reboots without
        // needing a separate counter file that could drift out of sync.
        File f = SD.open(LOG_FILE_PATH, FILE_READ);
        uint32_t rowCount = 0;
        if (f) {
            bool sawHeader = false;
            while (f.available()) {
                String line = f.readStringUntil('\n');
                if (line.length() == 0) continue;
                if (!sawHeader) { sawHeader = true; continue; }
                rowCount++;
            }
            f.close();
        }
        _nextTestNumber = rowCount + 1;
    }

    return true;
}

static String faultToken(const WireFault& f) {
    switch (f.type) {
        case FaultType::OPEN:
            return "OPEN:" + wireLabel(f.wireIndex);
        case FaultType::SHORT:
            return "SHORT:" + wireLabel(f.wireIndex) + "-" + wireLabel(f.partnerWire);
        case FaultType::MISMATCH:
            return "MISWIRE:" + wireLabel(f.wireIndex) + "-" + wireLabel(f.partnerWire);
    }
    return "";
}

bool StorageManager::logResult(const FaultReport& report, time_t timestamp) {
    File f = SD.open(LOG_FILE_PATH, FILE_APPEND);
    if (!f) return false;

    f.print(_nextTestNumber);
    f.print(',');
    f.print((uint32_t)timestamp);
    f.print(',');
    f.print(report.allPass ? "PASS" : "FAIL");
    f.print(',');
    for (uint8_t i = 0; i < report.faultCount; i++) {
        if (i > 0) f.print(';');
        f.print(faultToken(report.faults[i]));
    }
    f.println();

    f.flush();
    f.close();

    _nextTestNumber++; // only consumed once a write is actually issued
    return true; // file opened and writes were issued; this SD lib can't confirm
                 // physical commit any more precisely than that
}

bool StorageManager::logGoldenSampleEvent(time_t timestamp) {
    File f = SD.open(LOG_FILE_PATH, FILE_APPEND);
    if (!f) return false;

    // Uses the same 4-column shape as a normal row, but with a distinct
    // "result" value so log readers/scripts can grep for it easily.
    // Does NOT consume a test_number — this isn't a test, it's an event marker.
    f.print("-,"); // no test number applies to this row
    f.print((uint32_t)timestamp);
    f.print(",GOLDEN_SAMPLE_UPDATED,");
    f.println();

    f.flush();
    f.close();
    return true;
}

bool StorageManager::saveGoldenSample(const TestResult& result) {
    SD.remove(GOLDEN_FILE_PATH);
    File f = SD.open(GOLDEN_FILE_PATH, FILE_WRITE);
    if (!f) return false;

    size_t written = f.write((const uint8_t*)result.connections, sizeof(result.connections));
    f.flush();
    f.close();

    return written == sizeof(result.connections); // this one we CAN verify precisely
}

bool StorageManager::loadGoldenSample(TestResult& outResult) {
    if (!SD.exists(GOLDEN_FILE_PATH)) return false;

    File f = SD.open(GOLDEN_FILE_PATH, FILE_READ);
    if (!f) return false;

    if (f.size() != sizeof(outResult.connections)) {
        f.close();
        return false; // corrupt or old-format file
    }

    f.read((uint8_t*)outResult.connections, sizeof(outResult.connections));
    f.close();
    return true;
}