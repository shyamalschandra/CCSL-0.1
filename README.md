# Chandra Credit Software License (CCSL)

![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)
![License](https://img.shields.io/badge/license-CCSL-green.svg)

## Overview

The Chandra Credit Software License (CCSL) is an innovative software licensing system that combines code quality metrics with cryptocurrency micropayments to create a fair and sustainable model for rewarding code contributions.

## Key Features

- **Code Quality Metrics**: Automatically evaluates code contributions using six distinct metrics (Impact, Simplicity, Cleanness, Comment, Creditability, and Novelty)
- **Bitcoin Integration**: Facilitates micropayments to contributors based on code quality and value
- **License Management**: Tracks contributions, metrics, and payments within a unified system
- **Recurring Payments**: Supports subscription-based payment models for ongoing contributions

## Requirements

- C++17 or higher compiler (GCC 8+, Clang 7+, MSVC 2019+)
- CMake 3.14 or higher
- Bitcoin API access (for payment functionality)

## Building the Project

### 1. Clone the repository

```bash
git clone https://github.com/shyamalchandra/CCSL.git
cd CCSL
```

### 2. Create a build directory

```bash
mkdir build
cd build
```

### 3. Configure with CMake

```bash
cmake ..
```

### 4. Build the library and examples

```bash
cmake --build .
```

### 5. Run the tests

```bash
ctest
```

### 6. Generate documentation (optional)

```bash
cmake --build . --target docs
```

## Usage Example

```cpp
#include <ccsl/license.hpp>
#include <ccsl/metrics.hpp>
#include <ccsl/payment.hpp>
#include <iostream>

using namespace ccsl;

int main() {
    // Create a license
    License license("My Project", "CCSL-1234-5678");
    
    // Register a code contribution
    CodeContribution contribution("Alice", "main.cpp", 1, 100);
    
    // Evaluate code quality
    MetricsEvaluator evaluator;
    std::string code = readCodeFromFile("main.cpp");
    auto evaluations = evaluator.evaluateAll(code);
    
    // Add evaluations to the contribution
    for (const auto& eval : evaluations) {
        contribution.addMetricEvaluation(eval);
    }
    
    // Calculate contribution value
    double value = contribution.calculateValue();
    std::cout << "Contribution value: " << value << std::endl;
    
    // Register the contribution with the license
    license.registerContribution(contribution);
    
    // Set up Bitcoin payment
    BitcoinPaymentManager paymentManager("your-api-key");
    
    // Calculate payment
    double paymentAmount = value * 0.001; // 0.001 BTC per credit point
    
    // Send payment
    paymentManager.sendPayment(
        "source-wallet-address",
        "destination-wallet-address",
        paymentAmount,
        contribution.getContributor(),
        [](const PaymentTransaction& tx, bool success) {
            if (success) {
                std::cout << "Payment successful" << std::endl;
            }
        }
    );
    
    return 0;
}
```

## Project Structure

- **include/**: Header files
- **src/**: Source files
- **examples/**: Example applications
- **test/**: Test files
- **doc/**: Documentation files
- **external/**: External dependencies

## Examples

The project includes several example applications demonstrating different aspects of the CCSL system:

- **basic_example**: Demonstrates the core functionality of the CCSL system
- **metrics_example**: Shows how to use the metrics evaluation system
- **payment_example**: Demonstrates the Bitcoin payment integration

## Documentation

Comprehensive documentation is available in both HTML and PDF formats. After building the documentation with the command shown above, you can find:

- HTML documentation in the `build/doc/html/` directory
- PDF documentation in the `build/doc/latex/` directory

## License

This project is licensed under the Chandra Credit Software License (CCSL). See the [LICENSE](license.md) file for details.

## Author

Shyamal Chandra (C) 2025

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.