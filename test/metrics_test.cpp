/**
 * @file metrics_test.cpp
 * @brief Test cases for CCSL metrics functionality
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/metrics.hpp>
#include <ccsl/utility.hpp>
#include "test_framework.hpp"
#include <iostream>
#include <memory>

using namespace ccsl;
using namespace ccsl::test;

// Helper function to create a sample code snippet for testing
std::string createSampleCode(bool highImpact, bool highSimplicity, bool highCleanness,
                             bool goodComments, bool highCreditability, bool highNovelty) {
    std::string code;
    
    // Header with documentation
    code += "/**\n";
    code += " * @file sample.cpp\n";
    code += " * @brief Sample code for testing metrics\n";
    if (highCreditability) {
        code += " * @author Test Author\n";
        code += " * @see https://example.com/reference\n";
        code += " * @version 1.0\n";
    }
    code += " */\n\n";
    
    // Include statements
    code += "#include <iostream>\n";
    code += "#include <vector>\n";
    code += "#include <algorithm>\n\n";
    
    // Class declaration
    code += "class SampleClass {\n";
    code += "public:\n";
    
    // Constructor
    if (goodComments) {
        code += "    // Constructor for the sample class\n";
        code += "    // Takes a name and value parameter\n";
    }
    
    if (highCleanness) {
        code += "    SampleClass(const std::string& name, int value)\n";
        code += "        : m_name(name),\n";
        code += "          m_value(value) {\n";
    } else {
        code += "    SampleClass(const std::string&name,int value):m_name(name),m_value(value){";
    }
    
    // Constructor body
    if (highImpact) {
        code += "        validateName();\n";
        code += "        validateValue();\n";
        code += "        initializeComponents();\n";
    } else {
        code += "        // No validation or initialization\n";
    }
    code += "    }\n\n";
    
    // Method
    if (goodComments) {
        code += "    /**\n";
        code += "     * Calculate a result based on the stored value\n";
        code += "     * @param factor The multiplication factor\n";
        code += "     * @return The calculated result\n";
        code += "     */\n";
    }
    
    code += "    int calculateResult(int factor) {\n";
    
    // Method implementation
    if (highImpact) {
        if (highNovelty) {
            code += "        // Using advanced pattern for calculation - O(1) complexity\n";
            code += "        auto valueMapper = [this](int f) { return this->m_value * f; };\n";
            code += "        return valueMapper(factor);\n";
        } else {
            code += "        return m_value * factor;\n";
        }
    } else {
        code += "        int result = 0;\n";
        code += "        for (int i = 0; i < factor; i++) {\n";
        code += "            result += m_value;\n";
        code += "        }\n";
        code += "        return result;\n";
    }
    code += "    }\n\n";
    
    // Private methods
    if (highNovelty) {
        code += "private:\n";
        code += "    // Factory method for creating component objects\n";
        code += "    void initializeComponents() {\n";
        code += "        m_components = std::make_unique<ComponentFactory>()\n";
        code += "                           ->createComponent(m_name)\n";
        code += "                           ->withValue(m_value)\n";
        code += "                           ->build();\n";
        code += "    }\n\n";
    }
    
    code += "    void validateName() {\n";
    if (highSimplicity) {
        code += "        if (m_name.empty()) {\n";
        code += "            throw std::invalid_argument(\"Name cannot be empty\");\n";
        code += "        }\n";
    } else {
        code += "        bool valid = false;\n";
        code += "        if (m_name.size() > 0) {\n";
        code += "            for (size_t i = 0; i < m_name.size(); ++i) {\n";
        code += "                if (std::isalpha(m_name[i]) || std::isdigit(m_name[i]) || m_name[i] == '_') {\n";
        code += "                    valid = true;\n";
        code += "                    break;\n";
        code += "                }\n";
        code += "            }\n";
        code += "        }\n";
        code += "        if (!valid) throw std::invalid_argument(\"Invalid name\");\n";
    }
    code += "    }\n\n";
    
    code += "    void validateValue() {\n";
    code += "        if (m_value < 0) {\n";
    code += "            throw std::invalid_argument(\"Value cannot be negative\");\n";
    code += "        }\n";
    code += "    }\n\n";
    
    // Member variables
    if (highCleanness) {
        code += "private:\n";
        code += "    std::string m_name;\n";
        code += "    int m_value;\n";
        if (highNovelty) {
            code += "    std::unique_ptr<Component> m_components;\n";
        }
    } else {
        code += "private: std::string m_name; int m_value;\n";
    }
    
    // End of class
    code += "};\n\n";
    
    // Main function
    if (highCreditability) {
        code += "// Tests for SampleClass\n";
        code += "void testSampleClass() {\n";
        code += "    // Test constructor\n";
        code += "    SampleClass sample(\"test\", 42);\n";
        code += "    \n";
        code += "    // Test calculateResult\n";
        code += "    assert(sample.calculateResult(2) == 84);\n";
        code += "    assert(sample.calculateResult(0) == 0);\n";
        code += "}\n\n";
    }
    
    code += "int main() {\n";
    code += "    std::cout << \"Sample code for testing metrics\" << std::endl;\n";
    if (highCreditability) {
        code += "    \n";
        code += "    // Run tests\n";
        code += "    testSampleClass();\n";
        code += "    std::cout << \"All tests passed\" << std::endl;\n";
    }
    code += "    \n";
    code += "    return 0;\n";
    code += "}\n";
    
    return code;
}

void testImpactEvaluator() {
    std::cout << "Testing ImpactEvaluator...\n";
    
    ImpactEvaluator evaluator;
    
    // Test high impact code
    std::string highImpactCode = createSampleCode(true, false, false, false, false, false);
    MetricEvaluation highImpactEval = evaluator.evaluate(highImpactCode);
    
    Assert::areEqual(highImpactEval.type, MetricType::IMPACT);
    Assert::isTrue(highImpactEval.value > 0.5, "High impact code should have high impact score");
    
    // Test low impact code
    std::string lowImpactCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation lowImpactEval = evaluator.evaluate(lowImpactCode);
    
    Assert::areEqual(lowImpactEval.type, MetricType::IMPACT);
    Assert::isTrue(highImpactEval.value > lowImpactEval.value, 
                  "High impact code should score higher than low impact code");
}

void testSimplicityEvaluator() {
    std::cout << "Testing SimplicityEvaluator...\n";
    
    SimplicityEvaluator evaluator;
    
    // Test simple code
    std::string simpleCode = createSampleCode(false, true, true, false, false, false);
    MetricEvaluation simpleEval = evaluator.evaluate(simpleCode);
    
    Assert::areEqual(simpleEval.type, MetricType::SIMPLICITY);
    Assert::isTrue(simpleEval.value > 0.5, "Simple code should have high simplicity score");
    
    // Test complex code
    std::string complexCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation complexEval = evaluator.evaluate(complexCode);
    
    Assert::areEqual(complexEval.type, MetricType::SIMPLICITY);
    Assert::isTrue(simpleEval.value > complexEval.value, 
                  "Simple code should score higher than complex code");
}

void testCleanessEvaluator() {
    std::cout << "Testing CleanessEvaluator...\n";
    
    CleanessEvaluator evaluator;
    
    // Test clean code
    std::string cleanCode = createSampleCode(false, false, true, false, false, false);
    MetricEvaluation cleanEval = evaluator.evaluate(cleanCode);
    
    Assert::areEqual(cleanEval.type, MetricType::CLEANNESS);
    Assert::isTrue(cleanEval.value > 0.5, "Clean code should have high cleanness score");
    
    // Test messy code
    std::string messyCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation messyEval = evaluator.evaluate(messyCode);
    
    Assert::areEqual(messyEval.type, MetricType::CLEANNESS);
    Assert::isTrue(cleanEval.value > messyEval.value, 
                  "Clean code should score higher than messy code");
}

void testCommentEvaluator() {
    std::cout << "Testing CommentEvaluator...\n";
    
    CommentEvaluator evaluator;
    
    // Test well-commented code
    std::string commentedCode = createSampleCode(false, false, false, true, false, false);
    MetricEvaluation commentedEval = evaluator.evaluate(commentedCode);
    
    Assert::areEqual(commentedEval.type, MetricType::COMMENT);
    Assert::isTrue(commentedEval.value > 0.5, "Well-commented code should have high comment score");
    
    // Test poorly-commented code
    std::string uncommentedCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation uncommentedEval = evaluator.evaluate(uncommentedCode);
    
    Assert::areEqual(uncommentedEval.type, MetricType::COMMENT);
    Assert::isTrue(commentedEval.value > uncommentedEval.value, 
                  "Well-commented code should score higher than poorly-commented code");
}

void testCreditabilityEvaluator() {
    std::cout << "Testing CreditabilityEvaluator...\n";
    
    CreditabilityEvaluator evaluator;
    
    // Test code with high creditability
    std::string creditableCode = createSampleCode(false, false, false, false, true, false);
    MetricEvaluation creditableEval = evaluator.evaluate(creditableCode);
    
    Assert::areEqual(creditableEval.type, MetricType::CREDITABILITY);
    Assert::isTrue(creditableEval.value > 0.5, "Creditable code should have high creditability score");
    
    // Test code with low creditability
    std::string uncreditableCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation uncreditableEval = evaluator.evaluate(uncreditableCode);
    
    Assert::areEqual(uncreditableEval.type, MetricType::CREDITABILITY);
    Assert::isTrue(creditableEval.value > uncreditableEval.value, 
                  "Creditable code should score higher than uncreditable code");
}

void testNoveltyEvaluator() {
    std::cout << "Testing NoveltyEvaluator...\n";
    
    NoveltyEvaluator evaluator;
    
    // Test novel code
    std::string novelCode = createSampleCode(false, false, false, false, false, true);
    MetricEvaluation novelEval = evaluator.evaluate(novelCode);
    
    Assert::areEqual(novelEval.type, MetricType::NOVELTY);
    Assert::isTrue(novelEval.value > 0.5, "Novel code should have high novelty score");
    
    // Test ordinary code
    std::string ordinaryCode = createSampleCode(false, false, false, false, false, false);
    MetricEvaluation ordinaryEval = evaluator.evaluate(ordinaryCode);
    
    Assert::areEqual(ordinaryEval.type, MetricType::NOVELTY);
    Assert::isTrue(novelEval.value > ordinaryEval.value, 
                  "Novel code should score higher than ordinary code");
}

void testMetricEvaluatorFactory() {
    std::cout << "Testing MetricEvaluatorFactory...\n";
    
    // Test creating individual evaluator
    auto impactEvaluator = MetricEvaluatorFactory::create(MetricType::IMPACT);
    Assert::isNotNull(impactEvaluator.get());
    Assert::areEqual(impactEvaluator->getType(), MetricType::IMPACT);
    
    // Test creating all evaluators
    auto allEvaluators = MetricEvaluatorFactory::createAll();
    Assert::areEqual(allEvaluators.size(), size_t(6)); // 6 types of evaluators
    
    // Check that all types are represented
    bool hasImpact = false, hasSimplicity = false, hasCleanness = false,
         hasComment = false, hasCreditability = false, hasNovelty = false;
    
    for (const auto& evaluator : allEvaluators) {
        switch (evaluator->getType()) {
            case MetricType::IMPACT: hasImpact = true; break;
            case MetricType::SIMPLICITY: hasSimplicity = true; break;
            case MetricType::CLEANNESS: hasCleanness = true; break;
            case MetricType::COMMENT: hasComment = true; break;
            case MetricType::CREDITABILITY: hasCreditability = true; break;
            case MetricType::NOVELTY: hasNovelty = true; break;
        }
    }
    
    Assert::isTrue(hasImpact && hasSimplicity && hasCleanness && 
                   hasComment && hasCreditability && hasNovelty,
                   "All metric types should be represented");
    
    // Test invalid metric type
    Assert::throws<std::invalid_argument>([]() {
        MetricEvaluatorFactory::create(static_cast<MetricType>(999));
    }, "Invalid metric type should throw exception");
}

void testMetricsEvaluator() {
    std::cout << "Testing MetricsEvaluator...\n";
    
    MetricsEvaluator evaluator;
    
    // Test high-quality code that scores well in all metrics
    std::string highQualityCode = createSampleCode(true, true, true, true, true, true);
    
    // Test evaluateAll
    auto evaluations = evaluator.evaluateAll(highQualityCode);
    Assert::areEqual(evaluations.size(), size_t(6)); // 6 types of metrics
    
    // Test calculateValue
    double value = evaluator.calculateValue(highQualityCode);
    Assert::isTrue(value > 0.5, "High-quality code should have high overall value");
    
    // Test low-quality code
    std::string lowQualityCode = createSampleCode(false, false, false, false, false, false);
    double lowValue = evaluator.calculateValue(lowQualityCode);
    
    Assert::isTrue(value > lowValue, 
                  "High-quality code should score higher than low-quality code");
}

int main() {
    TestRunner runner;
    
    runner.addTest("ImpactEvaluator", testImpactEvaluator);
    runner.addTest("SimplicityEvaluator", testSimplicityEvaluator);
    runner.addTest("CleanessEvaluator", testCleanessEvaluator);
    runner.addTest("CommentEvaluator", testCommentEvaluator);
    runner.addTest("CreditabilityEvaluator", testCreditabilityEvaluator);
    runner.addTest("NoveltyEvaluator", testNoveltyEvaluator);
    runner.addTest("MetricEvaluatorFactory", testMetricEvaluatorFactory);
    runner.addTest("MetricsEvaluator", testMetricsEvaluator);
    
    return runner.runAll();
}