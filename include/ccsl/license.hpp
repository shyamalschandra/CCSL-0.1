/**
 * @file license.hpp
 * @brief Main header file for the CCSL (Chandra Credit Software License) system
 * @author Shyamal Chandra (C) 2025
 */

#ifndef CCSL_LICENSE_HPP
#define CCSL_LICENSE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <functional>

namespace ccsl {

/**
 * @brief Enumeration for different types of metrics used in code valuation
 */
enum class MetricType {
    IMPACT,        ///< Gravity effect towards a line in the program
    SIMPLICITY,    ///< Purity of syntactic, semantic, and pragmatic quality
    CLEANNESS,     ///< Proper formatting and notation
    COMMENT,       ///< Quality of non-opinionated statements
    CREDITABILITY, ///< Evidence that technique is compatible with requirements
    NOVELTY        ///< Creative approach to problem-solving
};

/**
 * @brief Structure for storing metric evaluation results
 */
struct MetricEvaluation {
    MetricType type;       ///< Type of metric
    double value;          ///< Normalized value between 0.0 and 1.0
    std::string rationale; ///< Explanation for the given value
};

/**
 * @brief Class for tracking code contributions and their valuations
 */
class CodeContribution {
public:
    /**
     * @brief Constructor for a code contribution
     * @param contributor Name of the contributor
     * @param fileId Identifier for the file
     * @param lineStart Starting line of the contribution
     * @param lineEnd Ending line of the contribution
     */
    CodeContribution(
        const std::string& contributor,
        const std::string& fileId,
        int lineStart,
        int lineEnd
    );
    
    /**
     * @brief Add a metric evaluation to this contribution
     * @param evaluation The metric evaluation to add
     */
    void addMetricEvaluation(const MetricEvaluation& evaluation);
    
    /**
     * @brief Calculate the total value of this contribution
     * @return Double value representing the contribution's worth
     */
    double calculateValue() const;
    
    /**
     * @brief Get the contributor's name
     * @return String containing the contributor's name
     */
    std::string getContributor() const { return m_contributor; }
    
    /**
     * @brief Get the file identifier
     * @return String containing the file identifier
     */
    std::string getFileId() const { return m_fileId; }
    
    /**
     * @brief Get the line range of this contribution
     * @return Pair containing the start and end lines
     */
    std::pair<int, int> getLineRange() const { return {m_lineStart, m_lineEnd}; }
    
    /**
     * @brief Get all metric evaluations for this contribution
     * @return Vector of metric evaluations
     */
    const std::vector<MetricEvaluation>& getMetricEvaluations() const { return m_evaluations; }
    
private:
    std::string m_contributor;              ///< Name of the contributor
    std::string m_fileId;                   ///< Identifier for the file
    int m_lineStart;                        ///< Starting line of the contribution
    int m_lineEnd;                          ///< Ending line of the contribution
    std::vector<MetricEvaluation> m_evaluations; ///< Metric evaluations for this contribution
};

/**
 * @brief Class for managing payments for code contributions
 */
class PaymentManager {
public:
    /**
     * @brief Constructor
     * @param walletAddress Bitcoin wallet address for payments
     */
    explicit PaymentManager(const std::string& walletAddress);
    
    /**
     * @brief Record a payment for a code contribution
     * @param contribution The code contribution being paid for
     * @param amount The amount in bitcoins
     * @return True if payment was successfully recorded
     */
    bool recordPayment(const CodeContribution& contribution, double amount);
    
    /**
     * @brief Get total payments for a specific contributor
     * @param contributor Name of the contributor
     * @return Total amount paid to the contributor
     */
    double getTotalPaymentsForContributor(const std::string& contributor) const;
    
    /**
     * @brief Generate a payment report
     * @return String containing the payment report
     */
    std::string generatePaymentReport() const;
    
private:
    std::string m_walletAddress; ///< Bitcoin wallet address for payments
    std::unordered_map<std::string, double> m_payments; ///< Map of contributor to total payments
};

/**
 * @brief Main class for the CCSL license system
 */
class License {
public:
    /**
     * @brief Constructor
     * @param projectName Name of the licensed project
     * @param licenseKey Unique license key
     */
    License(const std::string& projectName, const std::string& licenseKey);
    
    /**
     * @brief Register a code contribution
     * @param contribution The code contribution to register
     * @return True if contribution was successfully registered
     */
    bool registerContribution(const CodeContribution& contribution);
    
    /**
     * @brief Get all registered contributions
     * @return Vector of registered code contributions
     */
    const std::vector<CodeContribution>& getContributions() const { return m_contributions; }
    
    /**
     * @brief Get the payment manager
     * @return Reference to the payment manager
     */
    PaymentManager& getPaymentManager() { return m_paymentManager; }
    
    /**
     * @brief Validate the license
     * @return True if license is valid
     */
    bool validate() const;
    
    /**
     * @brief Get license information
     * @return String containing license information
     */
    std::string getLicenseInfo() const;
    
private:
    std::string m_projectName;              ///< Name of the licensed project
    std::string m_licenseKey;               ///< Unique license key
    std::vector<CodeContribution> m_contributions; ///< Registered code contributions
    PaymentManager m_paymentManager;        ///< Payment manager for this license
};

} // namespace ccsl

#endif // CCSL_LICENSE_HPP