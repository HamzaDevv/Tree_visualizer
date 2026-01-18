#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

/**
 * Robust Binary Tree Printer
 *
 * Features:
 * - Handles empty trees
 * - Handles single node trees
 * - Supports multi-digit numbers (positive and negative)
 * - Proper memory management with destructor
 * - Input validation
 * - Bounds checking to prevent buffer overflows
 * - Clean visual output with proper spacing
 */

class Node {
public:
  int data;
  Node *left;
  Node *right;

  explicit Node(int d) : data(d), left(nullptr), right(nullptr) {}
};

struct CoordinateBlock {
  Node *node;
  int x;
  int y;
};

class PrintingTree {
private:
  Node *root_;

  // Helper to calculate the maximum number width in the tree
  int getMaxWidth(Node *node) const {
    if (node == nullptr)
      return 1;

    int current = static_cast<int>(std::to_string(node->data).length());
    int leftMax = getMaxWidth(node->left);
    int rightMax = getMaxWidth(node->right);

    return std::max({current, leftMax, rightMax});
  }

  int heightOfTree(Node *node) const {
    if (node == nullptr)
      return 0;
    return 1 + std::max(heightOfTree(node->left), heightOfTree(node->right));
  }

  void inorder(Node *node, std::vector<CoordinateBlock> &coords, int start,
               int end, int term, int level) const {
    if (node == nullptr)
      return;

    int mid = start + (end - start) / 2;
    inorder(node->left, coords, start, mid - 1, term / 2, level + term + 1);
    coords.push_back({node, mid, level});
    inorder(node->right, coords, mid + 1, end, term / 2, level + term + 1);
  }

  void putDash(std::vector<std::vector<std::string>> &grid, Node *node, int x,
               int y, int height, int width) const {
    // Safety bounds check
    if (x < 0 || x >= width || y < 0 || y >= height)
      return;

    // Draw left branch
    if (node->left) {
      int p = x - 1;
      int q = y + 1;
      while (p >= 0 && q < height - 1) {
        // Check if next row has a number
        if (q + 1 < height && grid[q + 1][p] != " " && grid[q + 1][p] != "/" &&
            grid[q + 1][p] != "\\") {
          grid[q][p] = "/";
          break;
        }
        grid[q][p] = "/";
        p--;
        q++;
      }
    }

    // Draw right branch
    if (node->right) {
      int p = x + 1;
      int q = y + 1;
      while (p < width && q < height - 1) {
        // Check if next row has a number
        if (q + 1 < height && grid[q + 1][p] != " " && grid[q + 1][p] != "/" &&
            grid[q + 1][p] != "\\") {
          grid[q][p] = "\\";
          break;
        }
        grid[q][p] = "\\";
        p++;
        q++;
      }
    }
  }

  std::vector<Node *> levelOrder(Node *node) const {
    std::vector<Node *> result;
    if (node == nullptr)
      return result;

    std::queue<Node *> q;
    q.push(node);

    while (!q.empty()) {
      Node *front = q.front();
      q.pop();
      result.push_back(front);

      if (front->left)
        q.push(front->left);
      if (front->right)
        q.push(front->right);
    }
    return result;
  }

  int printingHeight(int h) const {
    if (h <= 0)
      return 0;
    if (h == 1)
      return 1;
    if (h == 2)
      return 2;

    int s = h;
    int term = static_cast<int>(std::pow(2, h - 2));
    while (term / 2 > 0) {
      s += term;
      term /= 2;
    }
    return s + 1;
  }

  void deleteTree(Node *node) {
    if (node == nullptr)
      return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }

  // Validate input from stream
  bool readInt(std::istream &in, int &value) {
    if (in >> value) {
      return true;
    }
    // Clear error state and ignore invalid input
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
  }

public:
  PrintingTree() : root_(nullptr) {}

  ~PrintingTree() {
    deleteTree(root_);
    root_ = nullptr;
  }

  // Prevent copying to avoid double-free
  PrintingTree(const PrintingTree &) = delete;
  PrintingTree &operator=(const PrintingTree &) = delete;

  // Build tree from preorder input
  Node *buildTree() { return buildTreeHelper(); }

  Node *buildTreeHelper() {
    int data;
    if (!readInt(std::cin, data)) {
      std::cerr
          << "Warning: Invalid input encountered, treating as -1 (null)\n";
      return nullptr;
    }

    if (data == -1) {
      return nullptr;
    }

    Node *node = new Node(data);
    node->left = buildTreeHelper();
    node->right = buildTreeHelper();

    return node;
  }

  // Build tree from a vector (useful for testing)
  Node *buildTreeFromVector(const std::vector<int> &values, size_t &index) {
    if (index >= values.size())
      return nullptr;

    int data = values[index++];
    if (data == -1)
      return nullptr;

    Node *node = new Node(data);
    node->left = buildTreeFromVector(values, index);
    node->right = buildTreeFromVector(values, index);

    return node;
  }

  void setRoot(Node *newRoot) {
    deleteTree(root_);
    root_ = newRoot;
  }

  Node *getRoot() const { return root_; }

  // Print tree with visual representation
  void printTree() const { printTreeFrom(root_); }

  void printTreeFrom(Node *root) const {
    // Handle empty tree
    if (root == nullptr) {
      std::cout << "\n[Empty Tree]\n";
      return;
    }

    int height = heightOfTree(root);

    // Handle single node
    if (height == 1) {
      std::cout << "\n[" << root->data << "]\n";
      return;
    }

    // Calculate dimensions
    int term = static_cast<int>(std::pow(2, height - 2));
    int gridWidth = static_cast<int>(std::pow(2, height));
    int gridHeight = printingHeight(height);

    // Get all coordinates via inorder traversal
    std::vector<CoordinateBlock> coords;
    inorder(root, coords, 0, gridWidth - 1, term, 0);

    // Create grid with strings (supports multi-digit numbers)
    std::vector<std::vector<std::string>> grid(
        gridHeight, std::vector<std::string>(gridWidth, " "));

    // Place all node values in grid
    for (const auto &coord : coords) {
      if (coord.x >= 0 && coord.x < gridWidth && coord.y >= 0 &&
          coord.y < gridHeight) {
        grid[coord.y][coord.x] = std::to_string(coord.node->data);
      }
    }

    // Get level order for connecting branches
    std::vector<Node *> levelNodes = levelOrder(root);

    // Draw connecting lines (branches)
    size_t nodeIndex = 0;
    for (int y = 0; y < gridHeight && nodeIndex < levelNodes.size(); y++) {
      for (int x = 0; x < gridWidth && nodeIndex < levelNodes.size(); x++) {
        // Check if this cell has a number
        if (grid[y][x] != " " && grid[y][x] != "/" && grid[y][x] != "\\") {
          putDash(grid, levelNodes[nodeIndex++], x, y, gridHeight, gridWidth);
        }
      }
    }

    // Print the grid
    std::cout << "\n";
    std::cout << std::string(gridWidth + 4, '=') << "\n";

    for (int y = 0; y < gridHeight; y++) {
      for (int x = 0; x < gridWidth; x++) {
        std::cout << grid[y][x];
      }
      std::cout << "\n";
    }

    std::cout << std::string(gridWidth + 4, '=') << "\n";
  }

  // Print tree statistics
  void printStats() const { printStatsFrom(root_); }

  void printStatsFrom(Node *root) const {
    if (root == nullptr) {
      std::cout << "Tree Statistics:\n";
      std::cout << "  - Height: 0\n";
      std::cout << "  - Node count: 0\n";
      return;
    }

    int height = heightOfTree(root);
    std::vector<Node *> nodes = levelOrder(root);

    // Calculate min and max values
    int minVal = nodes[0]->data;
    int maxVal = nodes[0]->data;
    for (const auto *n : nodes) {
      minVal = std::min(minVal, n->data);
      maxVal = std::max(maxVal, n->data);
    }

    std::cout << "\nTree Statistics:\n";
    std::cout << "  - Height: " << height << "\n";
    std::cout << "  - Node count: " << nodes.size() << "\n";
    std::cout << "  - Min value: " << minVal << "\n";
    std::cout << "  - Max value: " << maxVal << "\n";
  }

  // Interactive menu
  void interactiveMode() {
    int choice;
    bool running = true;

    while (running) {
      std::cout << "\n=== Binary Tree Printer Menu ===\n";
      std::cout << "1. Build tree from preorder input\n";
      std::cout << "2. Print tree\n";
      std::cout << "3. Print tree statistics\n";
      std::cout << "4. Clear tree\n";
      std::cout << "5. Load example tree\n";
      std::cout << "0. Exit\n";
      std::cout << "Choice: ";

      if (!readInt(std::cin, choice)) {
        std::cout << "Invalid input. Please enter a number.\n";
        continue;
      }

      switch (choice) {
      case 1: {
        deleteTree(root_);
        std::cout << "Enter data in preorder form (-1 for null):\n";
        root_ = buildTree();
        std::cout << "Tree built successfully!\n";
        break;
      }
      case 2:
        printTree();
        break;
      case 3:
        printStats();
        break;
      case 4:
        deleteTree(root_);
        root_ = nullptr;
        std::cout << "Tree cleared.\n";
        break;
      case 5: {
        deleteTree(root_);
        // Example: balanced tree
        std::vector<int> example = {5, 3, 1,  -1, -1, 4,  -1, -1,
                                    7, 6, -1, -1, 8,  -1, -1};
        size_t idx = 0;
        root_ = buildTreeFromVector(example, idx);
        std::cout << "Example tree loaded.\n";
        printTree();
        break;
      }
      case 0:
        running = false;
        std::cout << "Goodbye!\n";
        break;
      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    }
  }
};

// Demonstration and testing functions
void runTests() {
  std::cout << "\n=== Running Robustness Tests ===\n";

  // Test 1: Empty tree
  {
    std::cout << "\nTest 1: Empty Tree\n";
    PrintingTree pt;
    pt.printTree(); // Should print "[Empty Tree]"
  }

  // Test 2: Single node
  {
    std::cout << "\nTest 2: Single Node\n";
    PrintingTree pt;
    std::vector<int> data = {42, -1, -1};
    size_t idx = 0;
    pt.setRoot(pt.buildTreeFromVector(data, idx));
    pt.printTree(); // Should print "[42]"
  }

  // Test 3: Left-skewed tree
  {
    std::cout << "\nTest 3: Left-Skewed Tree (3 levels)\n";
    PrintingTree pt;
    std::vector<int> data = {1, 2, 3, -1, -1, -1, -1};
    size_t idx = 0;
    pt.setRoot(pt.buildTreeFromVector(data, idx));
    pt.printTree();
  }

  // Test 4: Right-skewed tree
  {
    std::cout << "\nTest 4: Right-Skewed Tree (3 levels)\n";
    PrintingTree pt;
    std::vector<int> data = {1, -1, 2, -1, 3, -1, -1};
    size_t idx = 0;
    pt.setRoot(pt.buildTreeFromVector(data, idx));
    pt.printTree();
  }

  // Test 5: Balanced tree with multi-digit numbers
  {
    std::cout << "\nTest 5: Balanced Tree with Multi-Digit Numbers\n";
    PrintingTree pt;
    std::vector<int> data = {100, 50,  25, -1, -1,  75, -1, -1,
                             150, 125, -1, -1, 175, -1, -1};
    size_t idx = 0;
    pt.setRoot(pt.buildTreeFromVector(data, idx));
    pt.printTree();
    pt.printStats();
  }

  // Test 6: Negative numbers
  {
    std::cout << "\nTest 6: Tree with Negative Numbers\n";
    PrintingTree pt;
    std::vector<int> data = {0, -5, -10, -1, -1, -3, -1, -1,
                             5, 3,  -1,  -1, 10, -1, -1};
    size_t idx = 0;
    pt.setRoot(pt.buildTreeFromVector(data, idx));
    pt.printTree();
    pt.printStats();
  }

  std::cout << "\n=== All Tests Complete ===\n";
}

int main(int argc, char *argv[]) {
  // Check for command line arguments
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--test" || arg == "-t") {
      runTests();
      return 0;
    } else if (arg == "--help" || arg == "-h") {
      std::cout << "Binary Tree Printer - Robust Edition\n\n";
      std::cout << "Usage:\n";
      std::cout << "  " << argv[0] << "           - Interactive mode\n";
      std::cout << "  " << argv[0] << " --test    - Run robustness tests\n";
      std::cout << "  " << argv[0] << " --help    - Show this help\n";
      std::cout
          << "\nIn interactive mode, enter tree data in preorder format.\n";
      std::cout << "Use -1 to indicate null nodes.\n";
      std::cout << "\nExample: 5 3 1 -1 -1 4 -1 -1 7 6 -1 -1 8 -1 -1\n";
      std::cout << "Creates:      5\n";
      std::cout << "           /     \\\n";
      std::cout << "          3       7\n";
      std::cout << "         / \\     / \\\n";
      std::cout << "        1   4   6   8\n";
      return 0;
    }
  }

  // Default: Interactive mode
  PrintingTree printer;
  printer.interactiveMode();

  return 0;
}