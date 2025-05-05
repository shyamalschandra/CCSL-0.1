/**
 * @file license.cpp
 * @brief Implementation of the CCSL core license functionality
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/license.hpp>
#include <ccsl/utility.hpp>
#include <iostream>
#include <algorithm>
#include <sstream>

namespace ccsl {

CodeContribution::CodeContribution(
    const std::string& contributor,
    const std::string& fileId,
    int lineStart,
    int lineEnd
) : m_contributor(contributor),
    m_fileId(fileId),
    m_lineStart(lineStart),
    m_lineEnd(lineEnd)
{
    // Validate input parameters
    if (lineStart > lineEnd) {
        throw std::invalid_argument("Start line must be less than or equal to end line");
    }
    
    if (contributor.empty()) {
        throw std::invalid_argument("Contributor name cannot be empty");
    }
    
    if (fileId.empty()) {
        throw std::invalid_argument("File ID cannot be empty");
    }
}

void CodeContribution::addMetricEvaluation(const MetricEvaluation& evaluation) {
    // Check if this metric type already exists, and replace it if so
    auto it = std::find_if(m_evaluations.begin(), m_evaluations.end(),
                          [&evaluation](const MetricEvaluation& e) {
                              return e.type == evaluation.type;
                          });
    
    if (it != m_evaluations.end()) {
        *it = evaluation;
    } else {
        m_evaluations.push_back(evaluation);
    }
}

double CodeContribution::calculateValue() const {
    if (m_evaluations.empty()) {
        return 0.0;
    }
    
    // Calculate the average of all metric values
    double sum = 0.0;
    for (const auto& eval : m_evaluations) {
        sum += eval.value;
    }
    
    return sum / m_evaluations.size();
}

PaymentManager::PaymentManager(const std::string& walletAddress)
    : m_walletAddress(walletAddress)
{
    // Validate the wallet address
    if (!validateBitcoinAddress(walletAddress)) {
        throw std::invalid_argument("Invalid Bitcoin wallet address");
    }
}

bool PaymentManager::recordPayment(const CodeContribution& contribution, double amount) {
    if (amount <= 0) {
        std::cerr << "Payment amount must be greater than zero" << std::endl;
        return false;
    }
    
    // Record the payment
    const std::string contributor = contribution.getContributor();
    m_payments[contributor] += amount;
    return true;
}

double PaymentManager::getTotalPaymentsForContributor(const std::string& contributor) const {
    auto it = m_payments.find(contributor);
    if (it != m_payments.end()) {
        return it->second;
    }
    return 0.0;
}

std::string PaymentManager::generatePaymentReport() const {
    std::stringstream report;
    report << "Payment Report\n";
    report << "==============\n\n";
    report << "Wallet Address: " << m_walletAddress << "\n\n";
    report << "Contributor Payments:\n";
    
    for (const auto& [contributor, amount] : m_payments) {
        report << contributor << ": " << formatBitcoinAmount(amount) << " BTC\n";
    }
    
    double total = 0.0;
    for (const auto& [_, amount] : m_payments) {
        total += amount;
    }
    
    report << "\nTotal Payments: " << formatBitcoinAmount(total) << " BTC\n";
    
    return report.str();
}

License::License(const std::string& projectName, const std::string& licenseKey)
    : m_projectName(projectName),
      m_licenseKey(licenseKey),
      m_paymentManager("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa")  // Default Bitcoin address
{
    // Validate input parameters
    if (projectName.empty()) {
        throw std::invalid_argument("Project name cannot be empty");
    }
    
    if (licenseKey.empty()) {
        throw std::invalid_argument("License key cannot be empty");
    }
}

bool License::registerContribution(const CodeContribution& contribution) {
    // Check if a contribution for the same file and line range already exists
    auto it = std::find_if(m_contributions.begin(), m_contributions.end(),
                          [&contribution](const CodeContribution& c) {
                              auto [start, end] = c.getLineRange();
                              auto [newStart, newEnd] = contribution.getLineRange();
                              
                              return c.getFileId() == contribution.getFileId() &&
                                     ((newStart >= start && newStart <= end) ||
                                      (newEnd >= start && newEnd <= end));
                          });
    
    if (it != m_contributions.end()) {
        std::cerr << "A contribution already exists for this file and line range" << std::endl;
        return false;
    }
    
    m_contributions.push_back(contribution);
    return true;
}

bool License::validate() const {
    // Simple validation logic for now
    if (m_projectName.empty() || m_licenseKey.empty()) {
        return false;
    }
    
    // Check if license key has the correct format (simplified for demonstration)
    if (m_licenseKey.length() < 8) {
        return false;
    }
    
    // In a real implementation, this would validate against a license server
    return true;
}

std::string License::getLicenseInfo() const {
    std::stringstream info;
    info << "CCSL License Information\n";
    info << "=======================\n\n";
    info << "Project: " << m_projectName << "\n";
    info << "License Key: " << m_licenseKey << "\n";
    info << "Validation Status: " << (validate() ? "Valid" : "Invalid") << "\n\n";
    
    info << "Registered Contributions:\n";
    for (const auto& contribution : m_contributions) {
        info << "  Contributor: " << contribution.getContributor() << "\n";
        info << "  File: " << contribution.getFileId() << "\n";
        info << "  Lines: " << contribution.getLineRange().first << "-" 
             << contribution.getLineRange().second << "\n";
        info << "  Value: " << contribution.calculateValue() << "\n\n";
    }
    
    return info.str();
}

} // namespace ccsl