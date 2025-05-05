/**
 * @file metrics.hpp
 * @brief Metrics evaluation system for code valuation
 * @author Shyamal Chandra (C) 2025
 */

#ifndef CCSL_METRICS_HPP
#define CCSL_METRICS_HPP

#include <ccsl/license.hpp>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace ccsl {

/**
 * @brief Abstract base class for metric evaluators
 */
class MetricEvaluator {
public:
    virtual ~MetricEvaluator() = default;
    
    /**
     * @brief Evaluate a code fragment according to this metric
     * @param code The code fragment to evaluate
     * @return A metric evaluation result
     */
    virtual MetricEvaluation evaluate(const std::string& code) const = 0;
    
    /**
     * @brief Get the type of this metric evaluator
     * @return The metric type
     */
    virtual MetricType getType() const = 0;
    
    /**
     * @brief Get a human-readable description of this metric
     * @return String containing the description
     */
    virtual std::string getDescription() const = 0;
};

/**
 * @brief Impact metric evaluator
 * Measures the gravity effect towards a particular line in the overall function
 */
class ImpactEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::IMPACT; }
    std::string getDescription() const override;
};

/**
 * @brief Simplicity metric evaluator
 * Measures purity of syntactic, semantic, and pragmatic quality
 */
class SimplicityEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::SIMPLICITY; }
    std::string getDescription() const override;
};

/**
 * @brief Cleanness metric evaluator
 * Measures proper formatting and notation
 */
class CleanessEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::CLEANNESS; }
    std::string getDescription() const override;
};

/**
 * @brief Comment metric evaluator
 * Measures quality of non-opinionated statements
 */
class CommentEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::COMMENT; }
    std::string getDescription() const override;
};

/**
 * @brief Creditability metric evaluator
 * Measures evidence that technique is compatible with requirements
 */
class CreditabilityEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::CREDITABILITY; }
    std::string getDescription() const override;
};

/**
 * @brief Novelty metric evaluator
 * Measures creative approach to problem-solving
 */
class NoveltyEvaluator : public MetricEvaluator {
public:
    MetricEvaluation evaluate(const std::string& code) const override;
    MetricType getType() const override { return MetricType::NOVELTY; }
    std::string getDescription() const override;
};

/**
 * @brief Factory for creating metric evaluators
 */
class MetricEvaluatorFactory {
public:
    /**
     * @brief Create a metric evaluator of the specified type
     * @param type The type of metric evaluator to create
     * @return Unique pointer to the created evaluator
     */
    static std::unique_ptr<MetricEvaluator> create(MetricType type);
    
    /**
     * @brief Create all metric evaluators
     * @return Vector of unique pointers to all evaluators
     */
    static std::vector<std::unique_ptr<MetricEvaluator>> createAll();
};

/**
 * @brief Evaluates all metrics for a code contribution
 */
class MetricsEvaluator {
public:
    /**
     * @brief Constructor
     */
    MetricsEvaluator();
    
    /**
     * @brief Evaluate all metrics for a code fragment
     * @param code The code fragment to evaluate
     * @return Vector of metric evaluations
     */
    std::vector<MetricEvaluation> evaluateAll(const std::string& code) const;
    
    /**
     * @brief Calculate the overall value of a code fragment
     * @param code The code fragment to evaluate
     * @return Double value representing the code's worth
     */
    double calculateValue(const std::string& code) const;
    
private:
    std::vector<std::unique_ptr<MetricEvaluator>> m_evaluators; ///< All metric evaluators
};

} // namespace ccsl

#endif // CCSL_METRICS_HPP