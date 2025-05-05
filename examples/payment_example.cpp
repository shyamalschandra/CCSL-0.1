/**
 * @file payment_example.cpp
 * @brief Example of using the CCSL payment system
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/payment.hpp>
#include <ccsl/license.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace ccsl;

/**
 * Example program demonstrating the payment system in CCSL
 */
int main(int argc, char* argv[]) {
    std::cout << "===== CCSL Payment System Example =====" << std::endl;
    
    // Check command line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <api-key>" << std::endl;
        return 1;
    }
    
    std::string apiKey = argv[1];
    std::cout << "Using API key: " << apiKey.substr(0, 5) << "..." << std::endl;
    
    // Create Bitcoin payment manager
    BitcoinPaymentManager paymentManager(apiKey);
    
    // Initialize payment manager
    std::cout << "Initializing payment manager..." << std::endl;
    if (!paymentManager.initialize()) {
        std::cerr << "Failed to initialize payment manager" << std::endl;
        return 1;
    }
    std::cout << "Payment manager initialized successfully" << std::endl;
    
    // Define source and destination wallets
    std::string sourceWallet = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
    std::string destinationWallet = "3J98t1WpEZ73CNmQviecrnyiWrnqRhWNLy";
    double amount = 0.0001; // Small amount for example purposes
    std::string contributionId = "example-contribution-001";
    
    std::cout << "Setting up payment transaction:" << std::endl;
    std::cout << " - Source wallet: " << sourceWallet << std::endl;
    std::cout << " - Destination wallet: " << destinationWallet << std::endl;
    std::cout << " - Amount: " << std::fixed << std::setprecision(8) << amount << " BTC" << std::endl;
    std::cout << " - Contribution ID: " << contributionId << std::endl;
    
    // Set up payment callback
    auto callback = [](const PaymentTransaction& tx, bool success) {
        std::cout << "Payment callback triggered:" << std::endl;
        if (success) {
            std::cout << "✓ Payment successful: " << std::fixed << std::setprecision(8) 
                      << tx.amount << " BTC (Transaction ID: " << tx.transactionId << ")" << std::endl;
        } else {
            std::cerr << "✗ Payment failed for transaction: " << tx.transactionId << std::endl;
        }
    };
    
    // Send payment
    std::cout << "\nSending payment..." << std::endl;
    std::future<std::string> future = paymentManager.sendPayment(
        sourceWallet,
        destinationWallet,
        amount,
        contributionId,
        callback
    );
    
    // Wait for result with timeout
    std::cout << "Waiting for transaction confirmation..." << std::endl;
    
    // Try to get the future result with timeout
    std::string txId;
    auto futureStatus = future.wait_for(std::chrono::seconds(10));
    if (futureStatus == std::future_status::ready) {
        txId = future.get();
        std::cout << "Transaction ID received: " << txId << std::endl;
    } else {
        std::cout << "Waiting for transaction timed out, will continue in background" << std::endl;
        // In a real application, we would handle this differently
    }
    
    // Set up a recurring payment subscription
    std::cout << "\nSetting up recurring payment subscription" << std::endl;
    RecurringPaymentManager recurringManager(paymentManager);
    
    // Create a subscription with a 30-day period
    PaymentSubscription subscription(
        "alice-smith",
        destinationWallet,
        30 // 30-day subscription period
    );
    
    recurringManager.addSubscription(subscription);
    std::cout << "Subscription added for contributor 'alice-smith'" << std::endl;
    
    // Process due payments (for demonstration purposes)
    std::cout << "Processing due payments..." << std::endl;
    int processedPayments = recurringManager.processDuePayments();
    std::cout << "Processed " << processedPayments << " payments" << std::endl;
    
    // Show payment report
    std::cout << "\nGenerating payment report:" << std::endl;
    
    // Create a simple license and contribution for the report
    License license("Example Project", "CCSL-DEMO-2025");
    CodeContribution contribution("alice-smith", "main.cpp", 1, 100);
    
    // Add metric evaluations
    MetricEvaluation impact{MetricType::IMPACT, 0.85, "High impact code"};
    MetricEvaluation simplicity{MetricType::SIMPLICITY, 0.92, "Very clean implementation"};
    MetricEvaluation comments{MetricType::COMMENT, 0.78, "Well documented"};
    
    contribution.addMetricEvaluation(impact);
    contribution.addMetricEvaluation(simplicity);
    contribution.addMetricEvaluation(comments);
    
    // Register contribution and record payment
    license.registerContribution(contribution);
    license.getPaymentManager().recordPayment(contribution, amount);
    
    // Display payment report
    std::cout << license.getPaymentManager().generatePaymentReport() << std::endl;
    
    std::cout << "\nPayment example completed successfully" << std::endl;
    return 0;
}