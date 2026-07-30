#include <unity.h>
#include "FaultAnalyzer.h"
 
void setUp(void) {}
void tearDown(void) {}
 
// Builds a TestResult representing a perfect 1:1 harness: wire i connects
// only to sense channel i, nothing else.
static TestResult buildIdentity() {
    TestResult r;
    for (int i = 0; i < NUM_WIRES; i++) {
        r.connections[i] = (1ULL << i);
    }
    return r;
}
 
// --- Pass case -------------------------------------------------------
 
void test_identity_match_is_all_pass(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(0, report.faultCount);
    TEST_ASSERT_TRUE(report.allPass);
}
 
// --- OPEN --------------------------------------------------------------
 
void test_open_wire_detected(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    actual.connections[12] = 0; // wire 12: no continuity anywhere
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(1, report.faultCount);
    TEST_ASSERT_FALSE(report.allPass);
    TEST_ASSERT_EQUAL(12, report.faults[0].wireIndex);
    TEST_ASSERT_EQUAL((int)FaultType::OPEN, (int)report.faults[0].type);
}
 
// --- SHORT ---------------------------------------------------------------
 
void test_short_detected(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    // Wire 5 correctly reaches pin 5, but also touches pin 9
    actual.connections[5] |= (1ULL << 9);
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(1, report.faultCount);
    TEST_ASSERT_EQUAL(5, report.faults[0].wireIndex);
    TEST_ASSERT_EQUAL((int)FaultType::SHORT, (int)report.faults[0].type);
    TEST_ASSERT_EQUAL(9, report.faults[0].partnerWire);
    TEST_ASSERT_EQUAL((1ULL << 9), report.faults[0].extraMask);
}
 
// --- MISMATCH ------------------------------------------------------------
 
void test_mismatch_detected(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    // Wire 20 never reaches pin 20 (its correct pin), lands on pin 1 instead
    actual.connections[20] = (1ULL << 1);
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(1, report.faultCount);
    TEST_ASSERT_EQUAL(20, report.faults[0].wireIndex);
    TEST_ASSERT_EQUAL((int)FaultType::MISMATCH, (int)report.faults[0].type);
    TEST_ASSERT_EQUAL(1, report.faults[0].partnerWire);
}
 
// --- Unpopulated channels (partial harness support) -----------------------
 
void test_unused_channel_with_no_continuity_is_not_a_fault(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    // Simulate a 50-wire loom: wire 60 isn't part of this harness at all
    expected.connections[60] = 0;
    actual.connections[60]   = 0;
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(0, report.faultCount);
    TEST_ASSERT_TRUE(report.allPass);
}
 
void test_unused_channel_with_crosstalk_is_a_short(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    // Wire 60 isn't populated, but the scan shows unexpected continuity
    // to channel 30 -- crosstalk/short bleeding into an unused position
    expected.connections[60] = 0;
    actual.connections[60]   = (1ULL << 30);
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(1, report.faultCount);
    TEST_ASSERT_EQUAL(60, report.faults[0].wireIndex);
    TEST_ASSERT_EQUAL((int)FaultType::SHORT, (int)report.faults[0].type);
    TEST_ASSERT_EQUAL(30, report.faults[0].partnerWire);
}
 
// --- Multiple simultaneous faults ------------------------------------------
 
void test_multiple_faults_all_counted(void) {
    TestResult expected = buildIdentity();
    TestResult actual   = buildIdentity();
 
    actual.connections[3]  = 0;              // OPEN
    actual.connections[7] |= (1ULL << 8);     // SHORT
    actual.connections[40] = (1ULL << 2);     // MISMATCH
 
    FaultReport report = FaultAnalyzer::analyze(actual, expected);
 
    TEST_ASSERT_EQUAL(3, report.faultCount);
    TEST_ASSERT_FALSE(report.allPass);
}
 
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_identity_match_is_all_pass);
    RUN_TEST(test_open_wire_detected);
    RUN_TEST(test_short_detected);
    RUN_TEST(test_mismatch_detected);
    RUN_TEST(test_unused_channel_with_no_continuity_is_not_a_fault);
    RUN_TEST(test_unused_channel_with_crosstalk_is_a_short);
    RUN_TEST(test_multiple_faults_all_counted);
    return UNITY_END();
}