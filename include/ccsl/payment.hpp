/**
 * @file payment.hpp
 * @brief Bitcoin micropayment integration for CCSL
 * @author Shyamal Chandra (C) 2025
 */

#ifndef CCSL_PAYMENT_HPP
#define CCSL_PAYMENT_HPP

#include <ccsl/license.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <future>

namespace ccsl {

/**
 * @brief Structure representing a payment transaction
 */
struct PaymentTransaction {
    std::string transactionId;        ///< Unique transaction ID
    std::string sourceWallet;         ///< Source wallet address
    std::string destinationWallet;    ///< Destination wallet address
    double amount;                    ///< Amount in bitcoins
    std::chrono::system_clock::time_point timestamp; ///< Transaction timestamp
    std::string contributionId;       ///< ID of the related code contribution
    bool verified;                    ///< Whether the transaction has been verified
};

/**
 * @brief Callback type for payment verification
 */
using PaymentVerificationCallback = std::function<void(const PaymentTransaction&, bool)>;

/**
 * @brief Class for managing Bitcoin micropayments
 */
class BitcoinPaymentManager {
public:
    /**
     * @brief Constructor
     * @param apiKey API key for the Bitcoin payment provider
     */
    explicit BitcoinPaymentManager(const std::string& apiKey);
    
    /**
     * @brief Initialize the payment manager
     * @return True if initialization was successful
     */
    bool initialize();
    
    /**
     * @brief Send a payment
     * @param sourceWallet Source wallet address
     * @param destinationWallet Destination wallet address
     * @param amount Amount in bitcoins
     * @param contributionId ID of the related code contribution
     * @param callback Callback function to be called when the payment is verified
     * @return A future that will contain the transaction ID when resolved
     */
    std::future<std::string> sendPayment(
        const std::string& sourceWallet,
        const std::string& destinationWallet,
        double amount,
        const std::string& contributionId,
        PaymentVerificationCallback callback
    );
    
    /**
     * @brief Verify a payment transaction
     * @param transactionId ID of the transaction to verify
     * @return True if the transaction is verified
     */
    bool verifyPayment(const std::string& transactionId);
    
    /**
     * @brief Get all transactions
     * @return Vector of all payment transactions
     */
    std::vector<PaymentTransaction> getTransactions() const;
    
    /**
     * @brief Get transactions for a specific contribution
     * @param contributionId ID of the contribution
     * @return Vector of related payment transactions
     */
    std::vector<PaymentTransaction> getTransactionsForContribution(const std::string& contributionId) const;
    
private:
    std::string m_apiKey;                            ///< API key for the Bitcoin payment provider
    std::vector<PaymentTransaction> m_transactions;  ///< All payment transactions
};

/**
 * @brief Class for managing payment subscriptions
 */
class PaymentSubscription {
public:
    /**
     * @brief Constructor
     * @param contributorId ID of the contributor
     * @param walletAddress Wallet address of the contributor
     * @param subscriptionPeriod Period in days between payments
     */
    PaymentSubscription(
        const std::string& contributorId,
        const std::string& walletAddress,
        int subscriptionPeriod
    );
    
    /**
     * @brief Process a subscription payment
     * @param paymentManager Reference to the payment manager
     * @param amount Amount to pay
     * @return True if payment was successful
     */
    bool processPayment(BitcoinPaymentManager& paymentManager, double amount);
    
    /**
     * @brief Check if payment is due
     * @return True if a payment is due
     */
    bool isPaymentDue() const;
    
    /**
     * @brief Get the contributor ID
     * @return String containing the contributor ID
     */
    std::string getContributorId() const { return m_contributorId; }
    
    /**
     * @brief Get the wallet address
     * @return String containing the wallet address
     */
    std::string getWalletAddress() const { return m_walletAddress; }
    
    /**
     * @brief Get the subscription period in days
     * @return Integer containing the subscription period
     */
    int getSubscriptionPeriod() const { return m_subscriptionPeriod; }
    
    /**
     * @brief Get the next payment date
     * @return Time point of the next payment
     */
    std::chrono::system_clock::time_point getNextPaymentDate() const { return m_nextPaymentDate; }
    
private:
    std::string m_contributorId;                    ///< ID of the contributor
    std::string m_walletAddress;                    ///< Wallet address of the contributor
    int m_subscriptionPeriod;                       ///< Period in days between payments
    std::chrono::system_clock::time_point m_nextPaymentDate; ///< Next payment date
};

/**
 * @brief Class for managing automatic recurring payments
 */
class RecurringPaymentManager {
public:
    /**
     * @brief Constructor
     * @param paymentManager Reference to the Bitcoin payment manager
     */
    explicit RecurringPaymentManager(BitcoinPaymentManager& paymentManager);
    
    /**
     * @brief Add a subscription
     * @param subscription The subscription to add
     */
    void addSubscription(const PaymentSubscription& subscription);
    
    /**
     * @brief Remove a subscription
     * @param contributorId ID of the contributor
     * @return True if the subscription was removed
     */
    bool removeSubscription(const std::string& contributorId);
    
    /**
     * @brief Process all due payments
     * @return Number of payments processed
     */
    int processDuePayments();
    
    /**
     * @brief Get all subscriptions
     * @return Vector of all subscriptions
     */
    const std::vector<PaymentSubscription>& getSubscriptions() const { return m_subscriptions; }
    
private:
    BitcoinPaymentManager& m_paymentManager;         ///< Reference to the Bitcoin payment manager
    std::vector<PaymentSubscription> m_subscriptions; ///< All subscriptions
};

} // namespace ccsl

#endif // CCSL_PAYMENT_HPP