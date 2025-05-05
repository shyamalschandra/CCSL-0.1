/**
 * @file utility.hpp
 * @brief Utility functions for the CCSL system
 * @author Shyamal Chandra (C) 2025
 */

#ifndef CCSL_UTILITY_HPP
#define CCSL_UTILITY_HPP

#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace ccsl {

/**
 * @brief Generates a unique identifier
 * @return A string containing a UUID
 */
std::string generateUUID();

/**
 * @brief Calculates a hash value for a string
 * @param input The string to hash
 * @return A string containing the hash
 */
std::string calculateHash(const std::string& input);

/**
 * @brief Validates a Bitcoin wallet address
 * @param address The wallet address to validate
 * @return True if the address is valid
 */
bool validateBitcoinAddress(const std::string& address);

/**
 * @brief Parses a code file to extract metadata
 * @param filePath Path to the code file
 * @return A map of metadata keys to values
 */
std::unordered_map<std::string, std::string> parseCodeMetadata(const std::filesystem::path& filePath);

/**
 * @brief Reads code from a file
 * @param filePath Path to the code file
 * @param startLine Starting line (0-based)
 * @param endLine Ending line (0-based)
 * @return The code as a string, or empty if file couldn't be read
 */
std::optional<std::string> readCodeFromFile(
    const std::filesystem::path& filePath,
    int startLine,
    int endLine
);

/**
 * @brief Normalizes a value to be between 0.0 and 1.0
 * @param value The value to normalize
 * @param min Minimum possible value
 * @param max Maximum possible value
 * @return The normalized value
 */
double normalizeValue(double value, double min, double max);

/**
 * @brief Formats a Bitcoin amount with proper precision
 * @param amount The amount in bitcoins
 * @return A string containing the formatted amount
 */
std::string formatBitcoinAmount(double amount);

/**
 * @brief Formats a timestamp as a human-readable string
 * @param timestamp The timestamp to format
 * @return A string containing the formatted timestamp
 */
std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp);

} // namespace ccsl

#endif // CCSL_UTILITY_HPP