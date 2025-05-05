/**
 * @file payment_test.cpp
 * @brief Test cases for CCSL payment functionality
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/payment.hpp>
#include <ccsl/utility.hpp>
#include "test_framework.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace ccsl;
using namespace ccsl::test;

void testBitcoinPaymentManager() {
    std::cout << "Testing BitcoinPaymentManager...\n";
    
    // Test constructor
    BitcoinPaymentManager manager("test-api-key");
    
    // Test invalid constructor arguments
    Assert::throws<std::invalid_argument>([]() {
        BitcoinPaymentManager invalid("");
    }, "Empty API key should throw exception");
    
    // Test initialization
    Assert::isTrue(manager.initialize());
    
    // Test sending payment
    std::string sourceWallet = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
    std::string destWallet = "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy";
    std::string contributionId = "test-contribution";
    double amount = 0.001;
    
    bool callbackCalled = false;
    PaymentVerificationCallback callback = [&callbackCalled](const PaymentTransaction& tx, bool success) {
        callbackCalled = true;
        Assert::isTrue(success);
        Assert::areEqual(tx.amount, 0.001);
    };
    
    // Send the payment
    std::future<std::string> future = manager.sendPayment(
        sourceWallet, destWallet, amount, contributionId, callback
    );
    
    // Wait for the future to complete (simulated transaction verification)
    std::string transactionId = future.get();
    
    // Check that we got a valid transaction ID
    Assert::isFalse(transactionId.empty());
    
    // Give some time for the callback to be executed
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Assert::isTrue(callbackCalled);
    
    // Test payment verification
    Assert::isTrue(manager.verifyPayment(transactionId));
    Assert::isFalse(manager.verifyPayment("invalid-id"));
    
    // Test getting transactions
    auto transactions = manager.getTransactions();
    Assert::areEqual(transactions.size(), size_t(1));
    
    // Test getting transactions for a contribution
    auto contributionTransactions = manager.getTransactionsForContribution(contributionId);
    Assert::areEqual(contributionTransactions.size(), size_t(1));
    
    // Test getting transactions for a non-existent contribution
    auto emptyTransactions = manager.getTransactionsForContribution("non-existent");
    Assert::areEqual(emptyTransactions.size(), size_t(0));
    
    // Test invalid payment parameters
    Assert::throws<std::invalid_argument>([&manager]() {
        manager.sendPayment("invalid-wallet", "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", 0.001, "test", {});
    }, "Invalid source wallet should throw exception");
    
    Assert::throws<std::invalid_argument>([&manager]() {
        manager.sendPayment("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", "invalid-wallet", 0.001, "test", {});
    }, "Invalid destination wallet should throw exception");
    
    Assert::throws<std::invalid_argument>([&manager]() {
        manager.sendPayment("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", 0, "test", {});
    }, "Zero amount should throw exception");
    
    Assert::throws<std::invalid_argument>([&manager]() {
        manager.sendPayment("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", -0.001, "test", {});
    }, "Negative amount should throw exception");
}

void testPaymentSubscription() {
    std::cout << "Testing PaymentSubscription...\n";
    
    std::string contributorId = "contributor-123";
    std::string walletAddress = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
    int subscriptionPeriod = 30; // days
    
    // Test constructor
    PaymentSubscription subscription(contributorId, walletAddress, subscriptionPeriod);
    
    // Test invalid constructor arguments
    Assert::throws<std::invalid_argument>([]() {
        PaymentSubscription invalid("", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", 30);
    }, "Empty contributor ID should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        PaymentSubscription invalid("contributor-123", "invalid-wallet", 30);
    }, "Invalid wallet address should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        PaymentSubscription invalid("contributor-123", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", 0);
    }, "Zero subscription period should throw exception");
    
    Assert::throws<std::invalid_argument>([]() {
        PaymentSubscription invalid("contributor-123", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", -30);
    }, "Negative subscription period should throw exception");
    
    // Test getters
    Assert::areEqual(subscription.getContributorId(), contributorId);
    Assert::areEqual(subscription.getWalletAddress(), walletAddress);
    Assert::areEqual(subscription.getSubscriptionPeriod(), subscriptionPeriod);
    
    // Test payment due status (should not be due right after creation)
    Assert::isFalse(subscription.isPaymentDue());
    
    // Test payment processing
    BitcoinPaymentManager manager("test-api-key");
    Assert::isTrue(subscription.processPayment(manager, 0.001));
}

void testRecurringPaymentManager() {
    std::cout << "Testing RecurringPaymentManager...\n";
    
    BitcoinPaymentManager paymentManager("test-api-key");
    
    // Test constructor
    RecurringPaymentManager manager(paymentManager);
    
    // Test adding subscriptions
    PaymentSubscription subscription1("contributor-1", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa", 30);
    PaymentSubscription subscription2("contributor-2", "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", 60);
    
    manager.addSubscription(subscription1);
    manager.addSubscription(subscription2);
    
    // Test getting subscriptions
    Assert::areEqual(manager.getSubscriptions().size(), size_t(2));
    
    // Test removing a subscription
    Assert::isTrue(manager.removeSubscription("contributor-1"));
    Assert::areEqual(manager.getSubscriptions().size(), size_t(1));
    
    // Test removing a non-existent subscription
    Assert::isFalse(manager.removeSubscription("non-existent"));
    
    // Test replacing a subscription
    PaymentSubscription subscription2Updated("contributor-2", "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy", 90);
    manager.addSubscription(subscription2Updated);
    Assert::areEqual(manager.getSubscriptions().size(), size_t(1));
    
    // Test processing due payments (none should be due)
    int processed = manager.processDuePayments();
    Assert::areEqual(processed, 0);
}

int main() {
    TestRunner runner;
    
    runner.addTest("BitcoinPaymentManager", testBitcoinPaymentManager);
    runner.addTest("PaymentSubscription", testPaymentSubscription);
    runner.addTest("RecurringPaymentManager", testRecurringPaymentManager);
    
    return runner.runAll();
}