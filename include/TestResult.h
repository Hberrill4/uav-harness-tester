#pragma once
#include <cstdint>
#include "Config.h"
 
// Result of one full connectivity scan.
// connections[i] is a bitmask: bit j is set if driven wire i shows
// continuity to sense channel j. A clean 1:1 harness has exactly one bit
// set per row (the diagonal); anything else is either an unpopulated
// channel (mask == 0, expected) or a fault (extra/missing bits).
struct TestResult {
    uint64_t connections[NUM_WIRES];
};