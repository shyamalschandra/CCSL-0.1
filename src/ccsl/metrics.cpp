/**
 * @file metrics.cpp
 * @brief Implementation of the metrics evaluation system
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/metrics.hpp>
#include <ccsl/utility.hpp>
#include <algorithm>
#include <regex>
#include <cmath>

namespace ccsl {

// Impact Evaluator Implementation
MetricEvaluation ImpactEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::IMPACT;
    
    // Simple implementation: count function calls and control structures
    // as indicators of impact
    int functionCalls = 0;
    int controlStructures = 0;
    
    // Count function calls (simplified)
    std::regex functionRegex(R"(\b\w+\s*\()");
    auto functionBegin = std::sregex_iterator(code.begin(), code.end(), functionRegex);
    auto functionEnd = std::sregex_iterator();
    functionCalls = std::distance(functionBegin, functionEnd);
    
    // Count control structures (if, for, while, switch)
    std::regex controlRegex(R"(\b(if|for|while|switch)\s*\()");
    auto controlBegin = std::sregex_iterator(code.begin(), code.end(), controlRegex);
    auto controlEnd = std::sregex_iterator();
    controlStructures = std::distance(controlBegin, controlEnd);
    
    // Normalize score between 0 and 1
    // Higher impact is indicated by more function calls and control structures
    const int maxExpectedCount = 20; // arbitrary threshold
    double rawScore = (functionCalls + controlStructures) / static_cast<double>(maxExpectedCount);
    result.value = std::min(1.0, std::max(0.0, rawScore));
    
    result.rationale = "Impact score based on " + std::to_string(functionCalls) + 
                      " function calls and " + std::to_string(controlStructures) + 
                      " control structures.";
    
    return result;
}

std::string ImpactEvaluator::getDescription() const {
    return "Measures the gravity effect towards a particular line in the overall function of the program.";
}

// Simplicity Evaluator Implementation
MetricEvaluation SimplicityEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::SIMPLICITY;
    
    // Simple implementation: examine line length, nesting depth, and symbol density
    
    // Average line length
    std::istringstream stream(code);
    std::string line;
    int totalLines = 0;
    int totalLineLength = 0;
    int maxNestingDepth = 0;
    int currentNestingDepth = 0;
    
    while (std::getline(stream, line)) {
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        totalLines++;
        totalLineLength += line.length();
        
        // Calculate nesting depth
        for (char c : line) {
            if (c == '{') {
                currentNestingDepth++;
                maxNestingDepth = std::max(maxNestingDepth, currentNestingDepth);
            } else if (c == '}') {
                currentNestingDepth = std::max(0, currentNestingDepth - 1);
            }
        }
    }
    
    double avgLineLength = totalLines > 0 ? totalLineLength / static_cast<double>(totalLines) : 0;
    
    // Symbol density (rough calculation)
    int symbolCount = 0;
    std::string symbols = "+-*/=<>!&|^~%?:;[](){}";
    for (char c : code) {
        if (symbols.find(c) != std::string::npos) {
            symbolCount++;
        }
    }
    double symbolDensity = code.length() > 0 ? symbolCount / static_cast<double>(code.length()) : 0;
    
    // Calculate simplicity score (higher score = more simple)
    // Ideal: short lines, low nesting depth, moderate symbol density
    double lineScore = std::max(0.0, 1.0 - (avgLineLength - 40) / 40.0);
    double nestingScore = std::max(0.0, 1.0 - (maxNestingDepth / 5.0));
    double symbolScore = std::max(0.0, 1.0 - std::abs(symbolDensity - 0.1) / 0.1);
    
    result.value = (lineScore + nestingScore + symbolScore) / 3.0;
    result.value = std::min(1.0, std::max(0.0, result.value));
    
    result.rationale = "Simplicity score based on average line length (" + 
                      std::to_string(avgLineLength) + " chars), nesting depth (" + 
                      std::to_string(maxNestingDepth) + "), and symbol density.";
    
    return result;
}

std::string SimplicityEvaluator::getDescription() const {
    return "Measures purity of syntactic, semantic, and pragmatic quality to be easily digested by programmers.";
}

// Cleanness Evaluator Implementation
MetricEvaluation CleanessEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::CLEANNESS;
    
    // Simple implementation: examine whitespace consistency, indentation, and bracket style
    
    // Check for consistent indentation
    std::istringstream stream(code);
    std::string line;
    int totalLines = 0;
    int indentedLines = 0;
    int emptyLines = 0;
    bool hasInconsistentIndentation = false;
    std::string prevIndent;
    
    while (std::getline(stream, line)) {
        totalLines++;
        
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            emptyLines++;
            continue;
        }
        
        // Extract indentation
        std::string indent;
        size_t firstNonWhitespace = line.find_first_not_of(" \t");
        if (firstNonWhitespace != std::string::npos) {
            indent = line.substr(0, firstNonWhitespace);
        }
        
        // Check for mixed spaces and tabs
        bool hasTabs = indent.find('\t') != std::string::npos;
        bool hasSpaces = indent.find(' ') != std::string::npos;
        if (hasTabs && hasSpaces) {
            hasInconsistentIndentation = true;
        }
        
        if (!indent.empty()) {
            indentedLines++;
            
            // Check if indentation is consistent with previous line
            if (!prevIndent.empty() && 
                (prevIndent.length() > 0 && indent.length() > 0) &&
                ((prevIndent[0] == ' ' && indent[0] == '\t') || 
                 (prevIndent[0] == '\t' && indent[0] == ' '))) {
                hasInconsistentIndentation = true;
            }
        }
        
        prevIndent = indent;
    }
    
    // Check for consistent bracket style
    int sameLine = 0;
    int nextLine = 0;
    
    std::regex sameBraceRegex(R"(\)\s*\{)");
    auto sameBraceBegin = std::sregex_iterator(code.begin(), code.end(), sameBraceRegex);
    auto sameBraceEnd = std::sregex_iterator();
    sameLine = std::distance(sameBraceBegin, sameBraceEnd);
    
    std::regex nextBraceRegex(R"(\)\s*$[\r\n]\s*\{)");
    auto nextBraceBegin = std::sregex_iterator(code.begin(), code.end(), nextBraceRegex);
    auto nextBraceEnd = std::sregex_iterator();
    nextLine = std::distance(nextBraceBegin, nextBraceEnd);
    
    bool consistentBraceStyle = (sameLine == 0 || nextLine == 0) && (sameLine + nextLine > 0);
    
    // Calculate cleanness score
    double indentScore = hasInconsistentIndentation ? 0.0 : 1.0;
    double braceScore = consistentBraceStyle ? 1.0 : 0.5;
    double whitespaceProportion = totalLines > 0 ? static_cast<double>(emptyLines) / totalLines : 0;
    double whitespaceScore = std::min(1.0, std::max(0.0, 1.0 - std::abs(whitespaceProportion - 0.2) / 0.2));
    
    result.value = (indentScore * 0.5) + (braceScore * 0.3) + (whitespaceScore * 0.2);
    result.value = std::min(1.0, std::max(0.0, result.value));
    
    result.rationale = "Cleanness score based on indentation consistency, brace style consistency, and whitespace usage.";
    
    return result;
}

std::string CleanessEvaluator::getDescription() const {
    return "Measures proper formatting and subsymbolic and symbolic notation.";
}

// Comment Evaluator Implementation
MetricEvaluation CommentEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::COMMENT;
    
    // Simple implementation: examine comment density and quality
    
    // Count lines of code and lines of comments
    std::istringstream stream(code);
    std::string line;
    int totalLines = 0;
    int commentLines = 0;
    int codeLines = 0;
    bool inMultilineComment = false;
    
    // Extract comment text for quality analysis
    std::vector<std::string> comments;
    
    while (std::getline(stream, line)) {
        totalLines++;
        
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        // Check for multiline comments
        size_t startComment = line.find("/*");
        size_t endComment = line.find("*/");
        
        if (inMultilineComment) {
            commentLines++;
            comments.push_back(line);
            
            if (endComment != std::string::npos) {
                inMultilineComment = false;
            }
        } else if (startComment != std::string::npos) {
            commentLines++;
            comments.push_back(line.substr(startComment));
            
            if (endComment != std::string::npos && endComment > startComment) {
                inMultilineComment = false;
            } else {
                inMultilineComment = true;
            }
        } else {
            // Check for single-line comments
            size_t lineComment = line.find("//");
            if (lineComment != std::string::npos) {
                commentLines++;
                comments.push_back(line.substr(lineComment + 2));
            } else {
                codeLines++;
            }
        }
    }
    
    // Calculate comment density
    double commentDensity = totalLines > 0 ? static_cast<double>(commentLines) / totalLines : 0;
    
    // Simple quality analysis - longer comments are generally better
    double avgCommentLength = 0;
    int commentWords = 0;
    for (const auto& comment : comments) {
        // Count words (simplified)
        std::istringstream wordStream(comment);
        std::string word;
        while (wordStream >> word) {
            commentWords++;
        }
    }
    
    avgCommentLength = comments.size() > 0 ? static_cast<double>(commentWords) / comments.size() : 0;
    
    // Ideal comment density around 25-30%
    double densityScore = std::max(0.0, 1.0 - std::abs(commentDensity - 0.3) / 0.3);
    
    // Prefer comments with meaningful explanations (approximated by length)
    double lengthScore = std::min(1.0, avgCommentLength / 8.0);
    
    // Calculate overall comment score
    result.value = (densityScore * 0.6) + (lengthScore * 0.4);
    result.value = std::min(1.0, std::max(0.0, result.value));
    
    result.rationale = "Comment score based on density (" + std::to_string(commentDensity * 100) + 
                       "%) and average length (" + std::to_string(avgCommentLength) + " words).";
    
    return result;
}

std::string CommentEvaluator::getDescription() const {
    return "Measures quality of non-opinionated statements with no syntactic sugar.";
}

// Creditability Evaluator Implementation
MetricEvaluation CreditabilityEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::CREDITABILITY;
    
    // Simple implementation: look for evidence of testing, documentation, and references
    
    // Look for testing indicators
    std::regex testRegex(R"(\b(test|assert|expect|should|mock|stub|spy)\b)");
    auto testBegin = std::sregex_iterator(code.begin(), code.end(), testRegex);
    auto testEnd = std::sregex_iterator();
    int testIndicators = std::distance(testBegin, testEnd);
    
    // Look for documentation indicators
    std::regex docRegex(R"(\@(param|return|throws?|see|link|since|version|author|deprecated))");
    auto docBegin = std::sregex_iterator(code.begin(), code.end(), docRegex);
    auto docEnd = std::sregex_iterator();
    int docIndicators = std::distance(docBegin, docEnd);
    
    // Look for references to external resources
    std::regex refRegex(R"((http|https)://[^\s"'<>]+|(RFC|IEEE|ISO)[- ][0-9]+)");
    auto refBegin = std::sregex_iterator(code.begin(), code.end(), refRegex);
    auto refEnd = std::sregex_iterator();
    int refIndicators = std::distance(refBegin, refEnd);
    
    // Calculate creditability score
    double testScore = std::min(1.0, testIndicators / 5.0);
    double docScore = std::min(1.0, docIndicators / 10.0);
    double refScore = std::min(1.0, refIndicators / 2.0);
    
    result.value = (testScore * 0.4) + (docScore * 0.4) + (refScore * 0.2);
    result.value = std::min(1.0, std::max(0.0, result.value));
    
    result.rationale = "Creditability score based on evidence of testing (" + 
                       std::to_string(testIndicators) + "), documentation (" + 
                       std::to_string(docIndicators) + "), and references (" + 
                       std::to_string(refIndicators) + ").";
    
    return result;
}

std::string CreditabilityEvaluator::getDescription() const {
    return "Measures evidence that technique is compatible with architecture requirements.";
}

// Novelty Evaluator Implementation
MetricEvaluation NoveltyEvaluator::evaluate(const std::string& code) const {
    MetricEvaluation result;
    result.type = MetricType::NOVELTY;
    
    // Simple implementation: look for unusual patterns, advanced features, and creativity indicators
    
    // Look for advanced language features
    std::regex advancedRegex(R"(\b(template|constexpr|decltype|concept|requires|noexcept|auto|lambda|fold|structured\s+binding)\b)");
    auto advancedBegin = std::sregex_iterator(code.begin(), code.end(), advancedRegex);
    auto advancedEnd = std::sregex_iterator();
    int advancedFeatures = std::distance(advancedBegin, advancedEnd);
    
    // Look for design pattern indicators
    std::regex patternRegex(R"(\b(Factory|Builder|Singleton|Adapter|Bridge|Composite|Decorator|Facade|Proxy|Observer|Strategy|Command|State|Visitor|Interpreter|Iterator|Mediator|Memento|Prototype)\b)");
    auto patternBegin = std::sregex_iterator(code.begin(), code.end(), patternRegex);
    auto patternEnd = std::sregex_iterator();
    int patternIndicators = std::distance(patternBegin, patternEnd);
    
    // Look for algorithm complexity comments
    std::regex complexityRegex(R"(O\([^\)]*\))");
    auto complexityBegin = std::sregex_iterator(code.begin(), code.end(), complexityRegex);
    auto complexityEnd = std::sregex_iterator();
    int complexityIndicators = std::distance(complexityBegin, complexityEnd);
    
    // Calculate novelty score
    double advancedScore = std::min(1.0, advancedFeatures / 3.0);
    double patternScore = std::min(1.0, patternIndicators / 2.0);
    double complexityScore = std::min(1.0, complexityIndicators / 1.0);
    
    result.value = (advancedScore * 0.4) + (patternScore * 0.4) + (complexityScore * 0.2);
    result.value = std::min(1.0, std::max(0.0, result.value));
    
    result.rationale = "Novelty score based on advanced language features (" + 
                       std::to_string(advancedFeatures) + "), design patterns (" + 
                       std::to_string(patternIndicators) + "), and algorithm analysis (" + 
                       std::to_string(complexityIndicators) + ").";
    
    return result;
}

std::string NoveltyEvaluator::getDescription() const {
    return "Measures creative and exotic approach to problem-solving.";
}

// MetricEvaluatorFactory Implementation
std::unique_ptr<MetricEvaluator> MetricEvaluatorFactory::create(MetricType type) {
    switch (type) {
        case MetricType::IMPACT:
            return std::make_unique<ImpactEvaluator>();
        case MetricType::SIMPLICITY:
            return std::make_unique<SimplicityEvaluator>();
        case MetricType::CLEANNESS:
            return std::make_unique<CleanessEvaluator>();
        case MetricType::COMMENT:
            return std::make_unique<CommentEvaluator>();
        case MetricType::CREDITABILITY:
            return std::make_unique<CreditabilityEvaluator>();
        case MetricType::NOVELTY:
            return std::make_unique<NoveltyEvaluator>();
        default:
            throw std::invalid_argument("Unknown metric type");
    }
}

std::vector<std::unique_ptr<MetricEvaluator>> MetricEvaluatorFactory::createAll() {
    std::vector<std::unique_ptr<MetricEvaluator>> evaluators;
    evaluators.push_back(std::make_unique<ImpactEvaluator>());
    evaluators.push_back(std::make_unique<SimplicityEvaluator>());
    evaluators.push_back(std::make_unique<CleanessEvaluator>());
    evaluators.push_back(std::make_unique<CommentEvaluator>());
    evaluators.push_back(std::make_unique<CreditabilityEvaluator>());
    evaluators.push_back(std::make_unique<NoveltyEvaluator>());
    return evaluators;
}

// MetricsEvaluator Implementation
MetricsEvaluator::MetricsEvaluator() {
    m_evaluators = MetricEvaluatorFactory::createAll();
}

std::vector<MetricEvaluation> MetricsEvaluator::evaluateAll(const std::string& code) const {
    std::vector<MetricEvaluation> results;
    
    for (const auto& evaluator : m_evaluators) {
        results.push_back(evaluator->evaluate(code));
    }
    
    return results;
}

double MetricsEvaluator::calculateValue(const std::string& code) const {
    auto evaluations = evaluateAll(code);
    
    if (evaluations.empty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (const auto& eval : evaluations) {
        sum += eval.value;
    }
    
    return sum / evaluations.size();
}

} // namespace ccsl