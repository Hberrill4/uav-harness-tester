#include "StorageManager.h"
#include "Config.h"
#include "WireMap.h"
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
    return true; // file opened and writes were issued; this SD lib can't confirm
                 // physical commit any more precisely than that
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
