/**
 * @file metrics_example.cpp
 * @brief Example of using the CCSL metrics evaluation system
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/metrics.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>

using namespace ccsl;

// Helper function to print metric evaluations
void printEvaluation(const MetricEvaluation& eval) {
    std::map<MetricType, std::string> metricNames = {
        {MetricType::IMPACT, "Impact"},
        {MetricType::SIMPLICITY, "Simplicity"},
        {MetricType::CLEANNESS, "Cleanness"},
        {MetricType::COMMENT, "Comment"},
        {MetricType::CREDITABILITY, "Creditability"},
        {MetricType::NOVELTY, "Novelty"}
    };
    
    std::cout << "Metric: " << std::left << std::setw(12) << metricNames[eval.type]
              << " Value: " << std::fixed << std::setprecision(2) << std::setw(5) << eval.value 
              << " - " << eval.rationale << std::endl;
}

int main() {
    std::cout << "===== CCSL Metrics Evaluation Example =====" << std::endl << std::endl;
    
    // Create example codes with different quality levels
    std::string goodCode = R"(
/**
 * Calculate factorial of a number using recursion.
 * @param n The number to calculate factorial for
 * @return The factorial value
 * @throws std::invalid_argument if n is negative
 */
int factorial(int n) {
    // Check for invalid input
    if (n < 0) {
        throw std::invalid_argument("Cannot calculate factorial of negative number");
    }
    
    // Base case
    if (n <= 1) {
        return 1;
    }
    
    // Recursive case
    return n * factorial(n - 1);
}
)";

    std::string poorCode = R"(
int f(int n) {
    if(n<0)throw std::invalid_argument("err");
    if(n<=1)return 1;
    return n*f(n-1);
}
)";

    std::string mediumCode = R"(
// Calculate factorial
int factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Negative input");
    }
    
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
)";

    // Create metrics evaluator
    MetricsEvaluator evaluator;
    
    // Evaluate good code
    std::cout << "Evaluating high-quality code:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << goodCode << std::endl;
    
    auto goodEvaluations = evaluator.evaluateAll(goodCode);
    for (const auto& eval : goodEvaluations) {
        printEvaluation(eval);
    }
    
    double goodValue = evaluator.calculateValue(goodCode);
    std::cout << "Overall value: " << std::fixed << std::setprecision(4) << goodValue << std::endl;
    std::cout << std::endl;
    
    // Evaluate medium code
    std::cout << "Evaluating medium-quality code:" << std::endl;
    std::cout << "------------------------------" << std::endl;
    std::cout << mediumCode << std::endl;
    
    auto mediumEvaluations = evaluator.evaluateAll(mediumCode);
    for (const auto& eval : mediumEvaluations) {
        printEvaluation(eval);
    }
    
    double mediumValue = evaluator.calculateValue(mediumCode);
    std::cout << "Overall value: " << std::fixed << std::setprecision(4) << mediumValue << std::endl;
    std::cout << std::endl;
    
    // Evaluate poor code
    std::cout << "Evaluating poor-quality code:" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << poorCode << std::endl;
    
    auto poorEvaluations = evaluator.evaluateAll(poorCode);
    for (const auto& eval : poorEvaluations) {
        printEvaluation(eval);
    }
    
    double poorValue = evaluator.calculateValue(poorCode);
    std::cout << "Overall value: " << std::fixed << std::setprecision(4) << poorValue << std::endl;
    std::cout << std::endl;
    
    // Compare the values
    std::cout << "Comparison of code quality:" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "High-quality code:   " << std::fixed << std::setprecision(4) << goodValue << std::endl;
    std::cout << "Medium-quality code: " << std::fixed << std::setprecision(4) << mediumValue << std::endl;
    std::cout << "Poor-quality code:   " << std::fixed << std::setprecision(4) << poorValue << std::endl;
    
    // Demonstrate per-metric comparison
    std::cout << std::endl << "Per-metric comparison:" << std::endl;
    std::cout << "---------------------" << std::endl;
    
    std::map<MetricType, std::string> metricNames = {
        {MetricType::IMPACT, "Impact"},
        {MetricType::SIMPLICITY, "Simplicity"},
        {MetricType::CLEANNESS, "Cleanness"},
        {MetricType::COMMENT, "Comment"},
        {MetricType::CREDITABILITY, "Creditability"},
        {MetricType::NOVELTY, "Novelty"}
    };
    
    // Create a map to store values by metric type
    std::map<MetricType, std::map<std::string, double>> metricValues;
    
    // Fill map for good code
    for (const auto& eval : goodEvaluations) {
        metricValues[eval.type]["High"] = eval.value;
    }
    
    // Fill map for medium code
    for (const auto& eval : mediumEvaluations) {
        metricValues[eval.type]["Medium"] = eval.value;
    }
    
    // Fill map for poor code
    for (const auto& eval : poorEvaluations) {
        metricValues[eval.type]["Poor"] = eval.value;
    }
    
    // Print table header
    std::cout << std::left << std::setw(14) << "Metric"
              << std::right << std::setw(10) << "High"
              << std::setw(10) << "Medium"
              << std::setw(10) << "Poor" << std::endl;
    std::cout << std::string(44, '-') << std::endl;
    
    // Print values by metric
    for (const auto& metric : metricNames) {
        std::cout << std::left << std::setw(14) << metric.second
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(10) << metricValues[metric.first]["High"]
                  << std::setw(10) << metricValues[metric.first]["Medium"]
                  << std::setw(10) << metricValues[metric.first]["Poor"] << std::endl;
    }
    
    std::cout << std::endl << "Metrics example completed successfully" << std::endl;
    
    return 0;
}