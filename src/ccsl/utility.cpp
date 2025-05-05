/**
 * @file utility.cpp
 * @brief Implementation of utility functions for the CCSL system
 * @author Shyamal Chandra (C) 2025
 */

#include <ccsl/utility.hpp>
#include <random>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <ctime>
#include <algorithm>
#include <functional>
#include <cctype>

namespace ccsl {

std::string generateUUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    
    ss << dis2(gen);
    
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }
    
    return ss.str();
}

std::string calculateHash(const std::string& input) {
    if (input.empty()) {
        return "";
    }
    
    // Simple hash function for demonstration purposes
    // In a real implementation, use a cryptographic hash function
    std::hash<std::string> hasher;
    size_t hashValue = hasher(input);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << hashValue;
    return ss.str();
}

bool validateBitcoinAddress(const std::string& address) {
    // Simple validation for demonstration purposes
    // In a real implementation, use proper Bitcoin address validation
    
    // Check length (typical Bitcoin address is 25-34 characters)
    if (address.length() < 25 || address.length() > 34) {
        return false;
    }
    
    // Check that it starts with 1, 3, or bc1
    if (!(address[0] == '1' || address[0] == '3' || address.substr(0, 3) == "bc1")) {
        return false;
    }
    
    // Check that it only contains valid characters
    std::regex validChars("^[a-zA-Z0-9]+$");
    return std::regex_match(address, validChars);
}

std::unordered_map<std::string, std::string> parseCodeMetadata(const std::filesystem::path& filePath) {
    std::unordered_map<std::string, std::string> metadata;
    
    // Open the file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return metadata;
    }
    
    // Read the first few lines to look for metadata
    std::string line;
    int lineCount = 0;
    const int maxLinesToCheck = 20;
    
    while (std::getline(file, line) && lineCount < maxLinesToCheck) {
        lineCount++;
        
        // Look for metadata in comments
        std::smatch matches;
        std::regex metadataRegex(R"(\*\s*@(\w+)\s*:\s*([^*]+))");
        
        if (std::regex_search(line, matches, metadataRegex) && matches.size() > 2) {
            std::string key = matches[1].str();
            std::string value = matches[2].str();
            
            // Trim whitespace
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            metadata[key] = value;
        }
    }
    
    return metadata;
}

std::optional<std::string> readCodeFromFile(
    const std::filesystem::path& filePath,
    int startLine,
    int endLine
) {
    // Validate input
    if (startLine < 0 || endLine < startLine) {
        return std::nullopt;
    }
    
    // Open the file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return std::nullopt;
    }
    
    // Read the lines
    std::string line;
    std::stringstream codeStream;
    int currentLine = 0;
    
    while (std::getline(file, line)) {
        if (currentLine >= startLine && currentLine <= endLine) {
            codeStream << line << '\n';
        } else if (currentLine > endLine) {
            break;
        }
        
        currentLine++;
    }
    
    return codeStream.str();
}

double normalizeValue(double value, double min, double max) {
    if (min >= max) {
        throw std::invalid_argument("Min must be less than max");
    }
    
    // Clamp the value to the range
    value = std::max(min, std::min(max, value));
    
    // Normalize to [0, 1]
    return (value - min) / (max - min);
}

std::string formatBitcoinAmount(double amount) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(8) << amount;
    return ss.str();
}

std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp) {
    // Convert to time_t
    std::time_t time = std::chrono::system_clock::to_time_t(timestamp);
    
    // Format the time
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

} // namespace ccsl