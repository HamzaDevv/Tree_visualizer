# 🧮 Numerical Integration Calculator

A robust C++ program for calculating definite integrals using numerical methods.

## Features

- **Multiple Integration Methods**
  - Trapezoidal Rule
  - Simpson's Rule  
  - Midpoint Rule

- **Expression Parser**
  - Supports multi-digit numbers and decimals
  - Supports variables (`x`)
  - Math functions: `sin`, `cos`, `tan`, `log`, `ln`, `sqrt`, `abs`, `exp`
  - Constants: `pi`, `e`
  - Operators: `+`, `-`, `*`, `/`, `^`

- **User-Friendly Interface**
  - Interactive menu with emoji icons
  - Guided quick-start mode
  - Built-in help and examples
  - Input validation with helpful error messages

## Compilation

```bash
g++ -std=c++17 -Wall -o integration INTEGRATION.cpp
```

## Usage

```bash
./integration           # Interactive mode
./integration --test    # Run built-in tests
./integration --help    # Show help
```

## Example Functions

| Expression | Description |
|------------|-------------|
| `x^2` | x squared |
| `x^3 - 2*x + 5` | Cubic polynomial |
| `sin(x)` | Sine of x (radians) |
| `exp(-x^2)` | Gaussian-like curve |
| `sqrt(1-x^2)` | Semi-circle |

## Sample Session

```
🚀 QUICK START MODE

📝 STEP 1: Enter your function f(x)
   👉 f(x) = x^2
   ✅ Function accepted!

📝 STEP 2: Enter the lower bound (a)
   👉 a = 0

📝 STEP 3: Enter the upper bound (b)
   👉 b = 1

📝 STEP 4: Enter precision
   👉 n = 1000

========== Integration Results ==========
Function: f(x) = x^2
Bounds: [0, 1]
-----------------------------------------
Trapezoidal Rule:  0.333333
Simpson's Rule:    0.333333
Midpoint Rule:     0.333333
=========================================
```

## License

MIT License
