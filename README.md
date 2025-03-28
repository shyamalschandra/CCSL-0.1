# Code Checker

The `code-checker.c` program is designed to evaluate the quality and value of a given piece of code using a token-based cascading transformers approach. This program simulates the process of analyzing code metrics and calculating a price in Bitcoin based on the defined metrics in the CCSL-0.1a license.

## Overview

The program performs the following steps:

1. **Tokenization**: The input code is tokenized to extract relevant metrics.
2. **Metric Calculation**: Various metrics are calculated based on the tokenized code.
3. **Composite Credit Score Calculation**: A composite score is computed using the calculated metrics.
4. **Bitcoin Payment Calculation**: The final price of the code is determined in Bitcoin.

## Process Diagram

```
┌──────────────────────┐
│      Input Code      │
└─────────┬────────────┘
          │
          ▼
┌──────────────────────┐
│    Tokenization      │
└─────────┬────────────┘
          │
          ▼
┌──────────────────────┐
│  Metric Calculation  │
└─────────┬────────────┘
          │
          ▼
┌────────────────────────────┐
│ Composite Credit Score Calc│
└─────────┬──────────────────┘
          │
          ▼
┌────────────────────────────┐
│ Bitcoin Payment Calculation│
└────────────────────────────┘
```

## Metrics

The program evaluates the following metrics:

- **Impact**: Measures the dependency and usage of the code.
- **Simplicity**: Assesses the complexity and maintainability.
- **Cleanness**: Evaluates formatting and structural coherence.
- **Comment Quality**: Analyzes the relevance and clarity of comments.
- **Creditability**: Checks the evidence and technical soundness.
- **Novelty**: Considers the innovation and uniqueness of the solution.

## Usage

To compile and run the program, use the following commands:

```bash
gcc -o code-checker code-checker.c -lm
./code-checker
```

## Example Output

The program outputs the composite credit score and the corresponding Bitcoin payment:

```
Composite Credit Score: 0.1234
Bitcoin Payment: 0.00001234 BTC
```

## License

This project is licensed under the CCSL-0.1a license. For more details, refer to the `LICENSE.md` file. 