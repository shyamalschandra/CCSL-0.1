/**
 * @file basic_example.cpp
 * @brief Basic example of using the CCSL system
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/license.hpp>
#include <ccsl/metrics.hpp>
#include <ccsl/payment.hpp>
#include <iostream>
#include <fstream>

using namespace ccsl;

// Example code to be evaluated
const char* sampleCode = R"(
/**
 * @brief Calculate factorial recursively
 * @param n The number to calculate factorial for
 * @return The factorial of n
 */
int factorial(int n) {
    // Base case: factorial of 0 is 1
    if (n == 0) {
        return 1;
    }
    
    // Recursive case: n! = n * (n-1)!
    return n * factorial(n - 1);
}
)";

int main() {
    try {
        std::cout << "CCSL Basic Example\n";
        std::cout << "=================\n\n";
        
        // Create a license
        std::cout << "Creating license...\n";
        License license("Example Project", "CCSL-EXAMPLE-2025");
        
        // Register a code contribution
        std::cout << "\nRegistering contribution...\n";
        CodeContribution contribution("Alice", "factorial.cpp", 1, 15);
        
        // Evaluate the code using all metrics
        std::cout << "Evaluating code metrics...\n";
        MetricsEvaluator evaluator;
        auto evaluations = evaluator.evaluateAll(sampleCode);
        
        // Add evaluations to the contribution
        for (const auto& eval : evaluations) {
            contribution.addMetricEvaluation(eval);
            std::cout << "Metric: " << static_cast<int>(eval.type) 
                      << ", Value: " << eval.value 
                      << ", Rationale: " << eval.rationale << "\n";
        }
        
        // Calculate overall value
        double value = contribution.calculateValue();
        std::cout << "\nOverall contribution value: " << value << "\n";
        
        // Register the contribution with the license
        if (license.registerContribution(contribution)) {
            std::cout << "Contribution registered successfully\n";
        }
        
        // Set up payment manager
        std::cout << "\nSetting up payment...\n";
        const std::string walletAddress = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
        BitcoinPaymentManager paymentManager("example-api-key");
        
        // Calculate payment amount based on license formula
        // Payment = Credit Score × Lines of Code × Base Rate
        const double baseRate = 0.00001; // BTC per line
        int linesOfCode = 15;  // From the contribution
        double payment = value * linesOfCode * baseRate;
        
        std::cout << "Calculated payment: " << formatBitcoinAmount(payment) << " BTC\n";
        
        // Set up payment callback
        PaymentVerificationCallback callback = [](const PaymentTransaction& tx, bool success) {
            if (success) {
                std::cout << "Payment verified: " << formatBitcoinAmount(tx.amount) 
                          << " BTC sent to " << tx.destinationWallet << "\n";
            } else {
                std::cout << "Payment verification failed\n";
            }
        };
        
        // Send payment
        std::cout << "Sending payment...\n";
        auto future = paymentManager.sendPayment(
            walletAddress,  // source (example only)
            walletAddress,  // destination (example only)
            payment,
            "example-contribution",
            callback
        );
        
        // Wait for transaction ID
        std::string transactionId = future.get();
        std::cout << "Transaction ID: " << transactionId << "\n";
        
        // Display license information
        std::cout << "\nLicense Information:\n";
        std::cout << license.getLicenseInfo() << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}