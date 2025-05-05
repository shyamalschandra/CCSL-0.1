/**
 * @file test_framework.hpp
 * @brief Simple test framework for CCSL
 * @author Shyamal Chandra (C) 2025
 */

#ifndef CCSL_TEST_FRAMEWORK_HPP
#define CCSL_TEST_FRAMEWORK_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>

namespace ccsl::test {

/**
 * @brief Class for running tests
 */
class TestRunner {
public:
    /**
     * @brief Add a test case to the test runner
     * @param name Name of the test
     * @param testFunc Function to run for the test
     */
    void addTest(const std::string& name, std::function<void()> testFunc) {
        m_tests.push_back({name, testFunc});
    }
    
    /**
     * @brief Run all tests
     * @return Number of failed tests
     */
    int runAll() {
        std::cout << "Running " << m_tests.size() << " tests...\n";
        std::cout << "========================================\n";
        
        int failedCount = 0;
        
        for (const auto& test : m_tests) {
            std::cout << "Running test: " << test.name << "\n";
            
            try {
                // Measure execution time
                auto start = std::chrono::steady_clock::now();
                test.func();
                auto end = std::chrono::steady_clock::now();
                
                std::chrono::duration<double, std::milli> elapsed = end - start;
                
                std::cout << "  PASSED (" << elapsed.count() << " ms)\n";
            } catch (const std::exception& e) {
                std::cerr << "  FAILED: " << e.what() << "\n";
                failedCount++;
            } catch (...) {
                std::cerr << "  FAILED: Unknown exception\n";
                failedCount++;
            }
        }
        
        std::cout << "========================================\n";
        std::cout << m_tests.size() - failedCount << " tests passed, " << failedCount << " tests failed\n";
        
        return failedCount;
    }
    
private:
    struct TestCase {
        std::string name;
        std::function<void()> func;
    };
    
    std::vector<TestCase> m_tests;
};

/**
 * @brief Assertion functions for testing
 */
class Assert {
public:
    /**
     * @brief Assert that a condition is true
     * @param condition The condition to check
     * @param message Message to display if assertion fails
     */
    static void isTrue(bool condition, const std::string& message = "Assertion failed") {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }
    
    /**
     * @brief Assert that a condition is false
     * @param condition The condition to check
     * @param message Message to display if assertion fails
     */
    static void isFalse(bool condition, const std::string& message = "Assertion failed") {
        isTrue(!condition, message);
    }
    
    /**
     * @brief Assert that two values are equal
     * @param actual The actual value
     * @param expected The expected value
     * @param message Message to display if assertion fails
     */
    template<typename T>
    static void areEqual(const T& actual, const T& expected, const std::string& message = "Values are not equal") {
        if (!(actual == expected)) {
            throw std::runtime_error(message);
        }
    }
    
    /**
     * @brief Assert that two values are not equal
     * @param actual The actual value
     * @param unexpected The unexpected value
     * @param message Message to display if assertion fails
     */
    template<typename T>
    static void areNotEqual(const T& actual, const T& unexpected, const std::string& message = "Values are equal") {
        if (actual == unexpected) {
            throw std::runtime_error(message);
        }
    }
    
    /**
     * @brief Assert that a value is null
     * @param value The value to check
     * @param message Message to display if assertion fails
     */
    template<typename T>
    static void isNull(const T* value, const std::string& message = "Value is not null") {
        if (value != nullptr) {
            throw std::runtime_error(message);
        }
    }
    
    /**
     * @brief Assert that a value is not null
     * @param value The value to check
     * @param message Message to display if assertion fails
     */
    template<typename T>
    static void isNotNull(const T* value, const std::string& message = "Value is null") {
        if (value == nullptr) {
            throw std::runtime_error(message);
        }
    }
    
    /**
     * @brief Assert that a function throws an exception of a specific type
     * @param func The function to check
     * @param message Message to display if assertion fails
     */
    template<typename ExceptionType>
    static void throws(std::function<void()> func, const std::string& message = "Function did not throw expected exception") {
        try {
            func();
            throw std::runtime_error(message);
        } catch (const ExceptionType&) {
            // Exception was thrown, test passes
        } catch (...) {
            throw std::runtime_error(message + " (wrong exception type)");
        }
    }
};

} // namespace ccsl::test

#endif // CCSL_TEST_FRAMEWORK_HPP