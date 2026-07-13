#include "StorageManager.h"
#include "Config.h"
#include <SD.h>
#include <SPI.h>

bool StorageManager::begin() {
    if (!SD.begin(PIN_SD_CS)) {
        return false;
    }
    if (!SD.exists(LOG_FILE_PATH)) {
        File f = SD.open(LOG_FILE_PATH, FILE_WRITE);
        if (f) {
            f.println("timestamp,result,faults");
            f.close();
        }
    }
    return true;
}

static String faultToken(const WireFault& f) {
    // 1-indexed wire numbers in the log to match physical labeling
    switch (f.type) {
        case FaultType::OPEN:
            return "OPEN:" + String(f.wireIndex + 1);
        case FaultType::SHORT:
            return "SHORT:" + String(f.wireIndex + 1) + "-" + String(f.partnerWire + 1);
        case FaultType::MISMATCH:
            return "MISWIRE:" + String(f.wireIndex + 1) + "-" + String(f.partnerWire + 1);
    }
    return "";
}

void StorageManager::logResult(const FaultReport& report, time_t timestamp) {
    File f = SD.open(LOG_FILE_PATH, FILE_APPEND);
    if (!f) return;

    f.print((uint32_t)timestamp);
    f.print(',');
    f.print(report.allPass ? "PASS" : "FAIL");
    f.print(',');

    for (uint8_t i = 0; i < report.faultCount; i++) {
        if (i > 0) f.print(';');
        f.print(faultToken(report.faults[i]));
    }
    f.println();
    f.close();
}

void StorageManager::saveGoldenSample(const TestResult& result) {
    SD.remove(GOLDEN_FILE_PATH); // overwrite any existing golden sample

    File f = SD.open(GOLDEN_FILE_PATH, FILE_WRITE);
    if (!f) return;
    f.write((const uint8_t*)result.connections, sizeof(result.connections));
    f.close();
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
