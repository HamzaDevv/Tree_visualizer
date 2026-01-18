#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Numerical Integration Calculator
 *
 * Features:
 * - Trapezoidal Rule integration
 * - Simpson's Rule integration (more accurate)
 * - Supports multi-digit numbers and decimals
 * - Supports common math functions: sin, cos, tan, log, ln, sqrt, abs
 * - Supports constants: pi, e
 * - Input validation and error handling
 * - Configurable precision (number of subdivisions)
 * - Division by zero protection
 * - Unbalanced parentheses detection
 */

class ExpressionParser {
private:
  std::string postfix_;

  int precedence(char op) const {
    switch (op) {
    case '^':
      return 4;
    case '*':
    case '/':
      return 3;
    case '+':
    case '-':
      return 2;
    default:
      return 0;
    }
  }

  bool isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
  }

  bool isRightAssociative(char c) const { return c == '^'; }

public:
  // Convert infix expression to postfix (Reverse Polish Notation)
  std::string infixToPostfix(const std::string &expr) {
    std::stack<char> opStack;
    std::string result;
    int parenCount = 0;

    for (size_t i = 0; i < expr.length(); i++) {
      char c = expr[i];

      // Skip whitespace
      if (std::isspace(c))
        continue;

      // Handle numbers (including multi-digit and decimals)
      if (std::isdigit(c) ||
          (c == '.' && i + 1 < expr.length() && std::isdigit(expr[i + 1]))) {
        if (!result.empty() && result.back() != ' ')
          result += ' ';
        while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
          result += expr[i++];
        }
        result += ' ';
        i--; // Adjust for loop increment
      }
      // Handle variable 'x' or 'X'
      else if (c == 'x' || c == 'X') {
        if (!result.empty() && result.back() != ' ')
          result += ' ';
        result += "x ";
      }
      // Handle constants and functions (pi, e, sin, cos, etc.)
      else if (std::isalpha(c)) {
        std::string token;
        while (i < expr.length() && std::isalpha(expr[i])) {
          token += std::tolower(expr[i++]);
        }
        i--; // Adjust for loop increment

        if (token == "x") {
          if (!result.empty() && result.back() != ' ')
            result += ' ';
          result += "x ";
        } else if (token == "pi") {
          if (!result.empty() && result.back() != ' ')
            result += ' ';
          result += std::to_string(M_PI) + ' ';
        } else if (token == "e") {
          if (!result.empty() && result.back() != ' ')
            result += ' ';
          result += std::to_string(M_E) + ' ';
        } else {
          // It's a function - push to stack
          for (char ch : token)
            opStack.push(ch);
          opStack.push('#'); // Function marker
        }
      }
      // Handle opening parenthesis
      else if (c == '(') {
        opStack.push(c);
        parenCount++;
      }
      // Handle closing parenthesis
      else if (c == ')') {
        parenCount--;
        if (parenCount < 0) {
          throw std::invalid_argument(
              "Unbalanced parentheses: extra closing ')'");
        }

        while (!opStack.empty() && opStack.top() != '(') {
          if (opStack.top() == '#') {
            // Function - reconstruct and add
            opStack.pop();
            std::string func;
            while (!opStack.empty() && std::isalpha(opStack.top())) {
              func = opStack.top() + func;
              opStack.pop();
            }
            result += func + " ";
          } else {
            result += opStack.top();
            result += ' ';
            opStack.pop();
          }
        }

        if (opStack.empty()) {
          throw std::invalid_argument(
              "Unbalanced parentheses: missing opening '('");
        }
        opStack.pop(); // Remove '('

        // Check if function marker follows
        if (!opStack.empty() && opStack.top() == '#') {
          opStack.pop();
          std::string func;
          while (!opStack.empty() && std::isalpha(opStack.top())) {
            func = opStack.top() + func;
            opStack.pop();
          }
          result += func + " ";
        }
      }
      // Handle operators
      else if (isOperator(c)) {
        while (!opStack.empty() && opStack.top() != '(' &&
               (precedence(opStack.top()) > precedence(c) ||
                (precedence(opStack.top()) == precedence(c) &&
                 !isRightAssociative(c)))) {
          result += opStack.top();
          result += ' ';
          opStack.pop();
        }
        opStack.push(c);
      }
    }

    if (parenCount != 0) {
      throw std::invalid_argument(
          "Unbalanced parentheses: missing closing ')'");
    }

    // Pop remaining operators
    while (!opStack.empty()) {
      if (opStack.top() == '(' || opStack.top() == ')') {
        throw std::invalid_argument("Unbalanced parentheses in expression");
      }
      if (opStack.top() == '#') {
        opStack.pop();
        std::string func;
        while (!opStack.empty() && std::isalpha(opStack.top())) {
          func = opStack.top() + func;
          opStack.pop();
        }
        result += func + " ";
      } else {
        result += opStack.top();
        result += ' ';
        opStack.pop();
      }
    }

    postfix_ = result;
    return result;
  }

  // Evaluate postfix expression for given x value
  double evaluate(double x) const {
    std::stack<double> valStack;
    std::istringstream iss(postfix_);
    std::string token;

    while (iss >> token) {
      // Check if it's a number
      if (std::isdigit(token[0]) ||
          (token.length() > 1 && (token[0] == '-' || token[0] == '.') &&
           std::isdigit(token[1]))) {
        valStack.push(std::stod(token));
      }
      // Check if it's 'x'
      else if (token == "x") {
        valStack.push(x);
      }
      // Check if it's a function
      else if (token == "sin") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: sin requires an argument");
        double a = valStack.top();
        valStack.pop();
        valStack.push(std::sin(a));
      } else if (token == "cos") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: cos requires an argument");
        double a = valStack.top();
        valStack.pop();
        valStack.push(std::cos(a));
      } else if (token == "tan") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: tan requires an argument");
        double a = valStack.top();
        valStack.pop();
        valStack.push(std::tan(a));
      } else if (token == "log") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: log requires an argument");
        double a = valStack.top();
        valStack.pop();
        if (a <= 0)
          throw std::runtime_error(
              "Domain error: log requires positive argument");
        valStack.push(std::log10(a));
      } else if (token == "ln") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: ln requires an argument");
        double a = valStack.top();
        valStack.pop();
        if (a <= 0)
          throw std::runtime_error(
              "Domain error: ln requires positive argument");
        valStack.push(std::log(a));
      } else if (token == "sqrt") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: sqrt requires an argument");
        double a = valStack.top();
        valStack.pop();
        if (a < 0)
          throw std::runtime_error(
              "Domain error: sqrt requires non-negative argument");
        valStack.push(std::sqrt(a));
      } else if (token == "abs") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: abs requires an argument");
        double a = valStack.top();
        valStack.pop();
        valStack.push(std::abs(a));
      } else if (token == "exp") {
        if (valStack.empty())
          throw std::runtime_error(
              "Invalid expression: exp requires an argument");
        double a = valStack.top();
        valStack.pop();
        valStack.push(std::exp(a));
      }
      // Check if it's an operator
      else if (token.length() == 1 && isOperator(token[0])) {
        if (valStack.size() < 2) {
          throw std::runtime_error(
              "Invalid expression: not enough operands for operator '" + token +
              "'");
        }
        double b = valStack.top();
        valStack.pop();
        double a = valStack.top();
        valStack.pop();

        switch (token[0]) {
        case '+':
          valStack.push(a + b);
          break;
        case '-':
          valStack.push(a - b);
          break;
        case '*':
          valStack.push(a * b);
          break;
        case '/':
          if (std::abs(b) < 1e-15) {
            throw std::runtime_error("Division by zero");
          }
          valStack.push(a / b);
          break;
        case '^':
          valStack.push(std::pow(a, b));
          break;
        }
      }
    }

    if (valStack.empty()) {
      throw std::runtime_error("Invalid expression: no result computed");
    }

    return valStack.top();
  }

  const std::string &getPostfix() const { return postfix_; }
};

class NumericalIntegrator {
private:
  ExpressionParser parser_;
  std::string infix_;
  double lowerBound_;
  double upperBound_;
  int subdivisions_;

  // Evaluate function at point x
  double f(double x) const { return parser_.evaluate(x); }

public:
  NumericalIntegrator() : lowerBound_(0), upperBound_(1), subdivisions_(1000) {}

  void setExpression(const std::string &expr) {
    infix_ = expr;
    parser_.infixToPostfix(expr);
  }

  void setBounds(double lower, double upper) {
    if (lower > upper) {
      std::swap(lower, upper);
      std::cout << "Note: Bounds were swapped (lower > upper)\n";
    }
    lowerBound_ = lower;
    upperBound_ = upper;
  }

  void setSubdivisions(int n) {
    if (n < 1) {
      std::cout << "Warning: Subdivisions must be at least 1. Using 1.\n";
      n = 1;
    }
    subdivisions_ = n;
  }

  // Trapezoidal Rule Integration
  double trapezoidalRule() const {
    double h = (upperBound_ - lowerBound_) / subdivisions_;
    double sum = 0.5 * (f(lowerBound_) + f(upperBound_));

    for (int i = 1; i < subdivisions_; i++) {
      double x = lowerBound_ + i * h;
      sum += f(x);
    }

    return h * sum;
  }

  // Simpson's Rule Integration (requires even number of subdivisions)
  double simpsonsRule() const {
    int n = subdivisions_;
    if (n % 2 != 0)
      n++; // Make even

    double h = (upperBound_ - lowerBound_) / n;
    double sum = f(lowerBound_) + f(upperBound_);

    for (int i = 1; i < n; i++) {
      double x = lowerBound_ + i * h;
      sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
    }

    return (h / 3) * sum;
  }

  // Midpoint Rule Integration
  double midpointRule() const {
    double h = (upperBound_ - lowerBound_) / subdivisions_;
    double sum = 0;

    for (int i = 0; i < subdivisions_; i++) {
      double midpoint = lowerBound_ + (i + 0.5) * h;
      sum += f(midpoint);
    }

    return h * sum;
  }

  // Compute all methods and show comparison
  void computeAll() const {
    std::cout << "\n========== Integration Results ==========\n";
    std::cout << "Function: f(x) = " << infix_ << "\n";
    std::cout << "Bounds: [" << lowerBound_ << ", " << upperBound_ << "]\n";
    std::cout << "Subdivisions: " << subdivisions_ << "\n";
    std::cout << "-----------------------------------------\n";

    try {
      double trap = trapezoidalRule();
      std::cout << "Trapezoidal Rule:  " << trap << "\n";
    } catch (const std::exception &e) {
      std::cout << "Trapezoidal Rule:  Error - " << e.what() << "\n";
    }

    try {
      double simp = simpsonsRule();
      std::cout << "Simpson's Rule:    " << simp << "\n";
    } catch (const std::exception &e) {
      std::cout << "Simpson's Rule:    Error - " << e.what() << "\n";
    }

    try {
      double mid = midpointRule();
      std::cout << "Midpoint Rule:     " << mid << "\n";
    } catch (const std::exception &e) {
      std::cout << "Midpoint Rule:     Error - " << e.what() << "\n";
    }

    std::cout << "=========================================\n";
  }

  // Test function evaluation at specific points
  void testFunction() const {
    std::cout << "\n--- Function Test Points ---\n";
    std::cout << "f(x) = " << infix_ << "\n";

    std::vector<double> testPoints = {0, 0.5, 1, 2, -1, M_PI, M_E};
    for (double x : testPoints) {
      try {
        double y = f(x);
        std::cout << "f(" << x << ") = " << y << "\n";
      } catch (const std::exception &e) {
        std::cout << "f(" << x << ") = Error: " << e.what() << "\n";
      }
    }
  }

  // Get current settings info
  void showInfo() const {
    std::cout << "\n--- Current Settings ---\n";
    std::cout << "Expression: " << infix_ << "\n";
    std::cout << "Postfix:    " << parser_.getPostfix() << "\n";
    std::cout << "Lower bound: " << lowerBound_ << "\n";
    std::cout << "Upper bound: " << upperBound_ << "\n";
    std::cout << "Subdivisions: " << subdivisions_ << "\n";
  }
};

// Validate numeric input
bool readDouble(double &value) {
  if (std::cin >> value) {
    return true;
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return false;
}

bool readInt(int &value) {
  if (std::cin >> value) {
    return true;
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return false;
}

void printHelp() {
  std::cout << R"(
=== Numerical Integration Calculator - Help ===

SUPPORTED OPERATORS:
  +  Addition
  -  Subtraction
  *  Multiplication
  /  Division
  ^  Power (right-associative)

SUPPORTED FUNCTIONS:
  sin(x)   - Sine (radians)
  cos(x)   - Cosine (radians)
  tan(x)   - Tangent (radians)
  log(x)   - Base-10 logarithm
  ln(x)    - Natural logarithm
  sqrt(x)  - Square root
  abs(x)   - Absolute value
  exp(x)   - e^x

SUPPORTED CONSTANTS:
  pi - 3.14159...
  e  - 2.71828...

EXAMPLES:
  x^2           - Simple polynomial
  2*x + 3       - Linear function
  sin(x)        - Trigonometric
  x^2 + 2*x + 1 - Quadratic
  exp(-x^2)     - Gaussian-like
  1/x           - Reciprocal (avoid x=0!)
  sqrt(1-x^2)   - Semi-circle

INTEGRATION METHODS:
  Trapezoidal - Uses trapezoids to approximate area
  Simpson's   - Uses parabolas (more accurate for smooth functions)
  Midpoint    - Uses rectangles at midpoints

================================================
)";
}

void runTests() {
  std::cout << "\n=== Running Integration Tests ===\n";

  NumericalIntegrator integrator;

  // Test 1: Simple polynomial (known integral: x^2 from 0 to 1 = 1/3)
  std::cout << "\nTest 1: ∫₀¹ x² dx (exact: 0.333...)\n";
  integrator.setExpression("x^2");
  integrator.setBounds(0, 1);
  integrator.setSubdivisions(1000);
  integrator.computeAll();

  // Test 2: Sine function (known integral: sin(x) from 0 to π = 2)
  std::cout << "\nTest 2: ∫₀^π sin(x) dx (exact: 2.0)\n";
  integrator.setExpression("sin(x)");
  integrator.setBounds(0, M_PI);
  integrator.setSubdivisions(1000);
  integrator.computeAll();

  // Test 3: Exponential
  std::cout << "\nTest 3: ∫₀¹ e^x dx (exact: e-1 ≈ 1.718)\n";
  integrator.setExpression("exp(x)");
  integrator.setBounds(0, 1);
  integrator.setSubdivisions(1000);
  integrator.computeAll();

  // Test 4: Multi-digit numbers
  std::cout << "\nTest 4: ∫₀^10 (100 + 25*x) dx (exact: 2250)\n";
  integrator.setExpression("100 + 25*x");
  integrator.setBounds(0, 10);
  integrator.setSubdivisions(100);
  integrator.computeAll();

  std::cout << "\n=== Tests Complete ===\n";
}

void printWelcomeBanner() {
  std::cout << R"(
╔══════════════════════════════════════════════════════════════════════════════╗
║                    🧮 NUMERICAL INTEGRATION CALCULATOR 🧮                     ║
║                          Trapezoidal & Simpson's Rule                        ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  Calculate definite integrals: ∫[a,b] f(x) dx                                ║
╚══════════════════════════════════════════════════════════════════════════════╝
)";
}

void printQuickReference() {
  std::cout << R"(
┌─────────────────────────── QUICK REFERENCE ──────────────────────────────────┐
│                                                                              │
│  📐 OPERATORS         │  📊 FUNCTIONS          │  🔢 CONSTANTS              │
│  ─────────────────────┼────────────────────────┼────────────────────────────│
│  +  Addition          │  sin(x)  Sine          │  pi = 3.14159...           │
│  -  Subtraction       │  cos(x)  Cosine        │  e  = 2.71828...           │
│  *  Multiplication    │  tan(x)  Tangent       │                            │
│  /  Division          │  log(x)  Log base 10   │  💡 Use 'x' as variable    │
│  ^  Power             │  ln(x)   Natural log   │                            │
│                       │  sqrt(x) Square root   │                            │
│                       │  abs(x)  Absolute val  │                            │
│                       │  exp(x)  e^x           │                            │
├──────────────────────────────────────────────────────────────────────────────┤
│  ✏️  HOW TO WRITE FUNCTIONS:                                                 │
│                                                                              │
│  • Simple:       x^2           (x squared)                                   │
│  • Polynomial:   x^3 - 2*x + 5 (cubic polynomial)                            │
│  • Trig:         sin(x)        (sine of x in radians)                        │
│  • Composite:    sin(x^2)      (sine of x-squared)                           │
│  • Combined:     x * sin(x)    (x times sine of x)                           │
│  • With const:   2*pi*x        (2π times x)                                  │
│  • Exponential:  exp(-x^2)     (Gaussian-like curve)                         │
│  • Logarithmic:  ln(x) / x     (log divided by x)                            │
│                                                                              │
│  ⚠️  IMPORTANT: Use * for multiplication! Write '2*x' not '2x'               │
└──────────────────────────────────────────────────────────────────────────────┘
)";
}

void printExamples() {
  std::cout << R"(
┌─────────────────────────── EXAMPLE PROBLEMS ─────────────────────────────────┐
│                                                                              │
│  1. Area under parabola:    f(x) = x^2         from 0 to 1    → Result: 0.33 │
│  2. Sine wave half-period:  f(x) = sin(x)      from 0 to pi   → Result: 2.00 │
│  3. Exponential growth:     f(x) = exp(x)      from 0 to 1    → Result: 1.72 │
│  4. Circle quarter area:    f(x) = sqrt(1-x^2) from 0 to 1    → Result: 0.79 │
│  5. Linear function:        f(x) = 3*x + 2     from 0 to 4    → Result: 32.0 │
│                                                                              │
└──────────────────────────────────────────────────────────────────────────────┘
)";
}

void runQuickStart(NumericalIntegrator &integrator) {
  std::cout
      << "\n🚀 QUICK START MODE - Let's compute an integral step by step!\n";
  std::cout
      << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

  // Step 1: Enter function
  std::cout << "\n📝 STEP 1: Enter your function f(x)\n";
  std::cout << "   Examples: x^2, sin(x), x^3 - 2*x + 1, exp(-x^2)\n";
  std::cout << "   👉 f(x) = ";

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::string expr;
  std::getline(std::cin, expr);

  try {
    integrator.setExpression(expr);
    std::cout << "   ✅ Function accepted!\n";
  } catch (const std::exception &e) {
    std::cout << "   ❌ Error: " << e.what() << "\n";
    std::cout << "   Using default: x^2\n";
    integrator.setExpression("x^2");
  }

  // Step 2: Lower bound
  std::cout << "\n📝 STEP 2: Enter the lower bound (a)\n";
  std::cout << "   Examples: 0, -1, -3.14\n";
  std::cout << "   👉 a = ";

  double lower;
  if (!readDouble(lower)) {
    std::cout << "   Using default: 0\n";
    lower = 0;
  }

  // Step 3: Upper bound
  std::cout << "\n📝 STEP 3: Enter the upper bound (b)\n";
  std::cout << "   Examples: 1, 10, 3.14\n";
  std::cout << "   👉 b = ";

  double upper;
  if (!readDouble(upper)) {
    std::cout << "   Using default: 1\n";
    upper = 1;
  }

  integrator.setBounds(lower, upper);

  // Step 4: Subdivisions
  std::cout << "\n📝 STEP 4: Enter precision (number of subdivisions)\n";
  std::cout << "   Higher = more accurate but slower. Recommended: 1000\n";
  std::cout << "   👉 n = ";

  int n;
  if (!readInt(n) || n < 1) {
    std::cout << "   Using default: 1000\n";
    n = 1000;
  }
  integrator.setSubdivisions(n);

  // Compute results
  std::cout << "\n🔄 Computing integral...\n";
  integrator.computeAll();
}

int main(int argc, char *argv[]) {
  // Command line arguments
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--test" || arg == "-t") {
      runTests();
      return 0;
    } else if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    }
  }

  NumericalIntegrator integrator;
  bool running = true;
  int choice;

  // Show welcome banner and quick reference
  printWelcomeBanner();
  printQuickReference();

  std::cout << "\n💡 TIP: Choose option [9] for a guided quick-start!\n";

  while (running) {
    std::cout << R"(
┌────────────────── MENU ──────────────────┐
│  1. 📝 Enter function f(x)               │
│  2. 📏 Set integration bounds [a, b]     │
│  3. 🔢 Set subdivisions (precision)      │
│  4. 📊 Compute integral (all methods)    │
│  5. 🧪 Test function at sample points    │
│  6. ⚙️  Show current settings             │
│  7. ❓ Show help & syntax reference      │
│  8. 🧪 Run built-in tests                │
│  9. 🚀 Quick Start (guided mode)         │
│  10. 📋 Show example problems            │
│  0. 🚪 Exit                              │
└──────────────────────────────────────────┘
)";
    std::cout << "👉 Your choice: ";

    if (!readInt(choice)) {
      std::cout << "❌ Invalid input. Please enter a number (0-10).\n";
      continue;
    }

    switch (choice) {
    case 1: {
      std::cout << "\n📝 Enter function f(x)\n";
      std::cout << "   Syntax: Use * for multiply, ^ for power, parentheses "
                   "for grouping\n";
      std::cout << "   Examples: x^2, sin(x), 2*x + 3, x^2 * exp(-x)\n";
      std::cout << "   👉 f(x) = ";

      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string expr;
      std::getline(std::cin, expr);
      try {
        integrator.setExpression(expr);
        std::cout << "\n   ✅ Function set successfully!\n";
        integrator.showInfo();
      } catch (const std::exception &e) {
        std::cout << "\n   ❌ Error parsing expression: " << e.what() << "\n";
        std::cout << "   💡 Tip: Make sure to use * for multiplication (e.g., "
                     "2*x not 2x)\n";
      }
      break;
    }
    case 2: {
      double lower, upper;
      std::cout << "\n📏 Set Integration Bounds\n";
      std::cout << "   We'll compute ∫[a,b] f(x) dx\n";
      std::cout << "   👉 Enter lower bound (a): ";
      if (!readDouble(lower)) {
        std::cout << "   ❌ Invalid number.\n";
        break;
      }
      std::cout << "   👉 Enter upper bound (b): ";
      if (!readDouble(upper)) {
        std::cout << "   ❌ Invalid number.\n";
        break;
      }
      integrator.setBounds(lower, upper);
      std::cout << "   ✅ Bounds set to [" << lower << ", " << upper << "]\n";
      break;
    }
    case 3: {
      int n;
      std::cout << "\n🔢 Set Number of Subdivisions\n";
      std::cout << "   More subdivisions = higher accuracy but slower\n";
      std::cout
          << "   Recommended: 100 (fast), 1000 (balanced), 10000 (precise)\n";
      std::cout << "   👉 Enter subdivisions: ";
      if (!readInt(n)) {
        std::cout << "   ❌ Invalid number.\n";
        break;
      }
      integrator.setSubdivisions(n);
      std::cout << "   ✅ Subdivisions set to " << n << "\n";
      break;
    }
    case 4:
      integrator.computeAll();
      break;
    case 5:
      integrator.testFunction();
      break;
    case 6:
      integrator.showInfo();
      break;
    case 7:
      printQuickReference();
      printHelp();
      break;
    case 8:
      runTests();
      break;
    case 9:
      runQuickStart(integrator);
      break;
    case 10:
      printExamples();
      break;
    case 0:
      std::cout
          << "\n👋 Goodbye! Thanks for using the Integration Calculator!\n";
      running = false;
      break;
    default:
      std::cout
          << "❌ Invalid choice. Please enter a number between 0 and 10.\n";
    }
  }

  return 0;
}
