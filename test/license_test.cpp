/**
 * @file license_test.cpp
 * @brief Test cases for CCSL license functionality
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/license.hpp>
#include "test_framework.hpp"

using namespace ccsl;
using namespace ccsl::test;

void testCodeContribution() {
    std::cout << "Testing CodeContribution...\n";
    
    // Test constructor
    CodeContribution contribution("Alice", "main.cpp", 10, 20);
    
    // Test getters
    Assert::areEqual(contribution.getContributor(), std::string("Alice"));
    Assert::areEqual(contribution.getFileId(), std::string("main.cpp"));
    
    auto lineRange = contribution.getLineRange();
    Assert::areEqual(lineRange.first, 10);
    Assert::areEqual(lineRange.second, 20);
    
    // Test invalid constructor arguments
    Assert::throws<std::invalid_argument>([]() {
        CodeContribution invalid("", "main.cpp", 10, 20);
    }, "Empty contributor should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        CodeContribution invalid("Alice", "", 10, 20);
    }, "Empty file ID should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        CodeContribution invalid("Alice", "main.cpp", 20, 10);
    }, "Start line > end line should throw exception");
    
    // Test metric evaluations
    Assert::areEqual(contribution.getMetricEvaluations().size(), size_t(0));
    
    MetricEvaluation eval;
    eval.type = MetricType::IMPACT;
    eval.value = 0.75;
    eval.rationale = "High impact code";
    
    contribution.addMetricEvaluation(eval);
    Assert::areEqual(contribution.getMetricEvaluations().size(), size_t(1));
    
    // Test value calculation
    Assert::areEqual(contribution.calculateValue(), 0.75);
    
    // Test adding another evaluation
    MetricEvaluation eval2;
    eval2.type = MetricType::SIMPLICITY;
    eval2.value = 0.85;
    eval2.rationale = "Very simple code";
    
    contribution.addMetricEvaluation(eval2);
    Assert::areEqual(contribution.getMetricEvaluations().size(), size_t(2));
    
    // Value should be average of all evaluations
    Assert::areEqual(contribution.calculateValue(), 0.8);
    
    // Test replacing an evaluation
    MetricEvaluation eval3;
    eval3.type = MetricType::IMPACT;  // Same type as eval1
    eval3.value = 0.95;
    eval3.rationale = "Extremely high impact";
    
    contribution.addMetricEvaluation(eval3);
    Assert::areEqual(contribution.getMetricEvaluations().size(), size_t(2));  // Still 2 evaluations
    
    // Value should be updated with the new evaluation
    Assert::areEqual(contribution.calculateValue(), 0.9);
}

void testPaymentManager() {
    std::cout << "Testing PaymentManager...\n";
    
    // Valid Bitcoin address (example only)
    std::string validAddress = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
    
    // Test constructor
    PaymentManager manager(validAddress);
    
    // Test invalid constructor arguments
    Assert::throws<std::invalid_argument>([]() {
        PaymentManager invalid("");
    }, "Empty wallet address should throw exception");
    
    // Test payment recording
    CodeContribution contribution("Bob", "utils.cpp", 5, 15);
    Assert::isTrue(manager.recordPayment(contribution, 0.001));
    
    // Test invalid payment amount
    Assert::isFalse(manager.recordPayment(contribution, 0.0));
    Assert::isFalse(manager.recordPayment(contribution, -0.001));
    
    // Test payment tracking
    Assert::areEqual(manager.getTotalPaymentsForContributor("Bob"), 0.001);
    Assert::areEqual(manager.getTotalPaymentsForContributor("Alice"), 0.0);
    
    // Test multiple payments
    Assert::isTrue(manager.recordPayment(contribution, 0.002));
    Assert::areEqual(manager.getTotalPaymentsForContributor("Bob"), 0.003);
    
    // Test payment report generation
    std::string report = manager.generatePaymentReport();
    Assert::isTrue(report.find("Bob: 0.00300000") != std::string::npos);
    Assert::isTrue(report.find("Total Payments: 0.00300000") != std::string::npos);
}

void testLicense() {
    std::cout << "Testing License...\n";
    
    // Test constructor
    License license("Test Project", "CCSL-1234-5678");
    
    // Test invalid constructor arguments
    Assert::throws<std::invalid_argument>([]() {
        License invalid("", "CCSL-1234-5678");
    }, "Empty project name should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        License invalid("Test Project", "");
    }, "Empty license key should throw exception");
    
    // Test license validation
    Assert::isTrue(license.validate());
    
    // Test contribution registration
    CodeContribution contribution("Carol", "api.cpp", 100, 200);
    Assert::isTrue(license.registerContribution(contribution));
    
    // Test overlapping contribution
    CodeContribution overlapping("Dave", "api.cpp", 150, 250);
    Assert::isFalse(license.registerContribution(overlapping));
    
    // Test non-overlapping contribution
    CodeContribution nonOverlapping("Eve", "api.cpp", 201, 300);
    Assert::isTrue(license.registerContribution(nonOverlapping));
    
    // Test getting contributions
    Assert::areEqual(license.getContributions().size(), size_t(2));
    
    // Test license info generation
    std::string info = license.getLicenseInfo();
    Assert::isTrue(info.find("Project: Test Project") != std::string::npos);
    Assert::isTrue(info.find("License Key: CCSL-1234-5678") != std::string::npos);
    Assert::isTrue(info.find("Contributor: Carol") != std::string::npos);
    Assert::isTrue(info.find("Contributor: Eve") != std::string::npos);
}

int main() {
    TestRunner runner;
    
    runner.addTest("CodeContribution", testCodeContribution);
    runner.addTest("PaymentManager", testPaymentManager);
    runner.addTest("License", testLicense);
    
    return runner.runAll();
}