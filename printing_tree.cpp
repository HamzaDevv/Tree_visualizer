#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

/**
 * 🌲 Binary Tree Visualizer - LeetCode Style
 *
 * Features:
 * - LeetCode-style input: [1,2,3,null,4,5]
 * - Beautiful ASCII art visualization
 * - All tree traversals (inorder, preorder, postorder, level-order)
 * - Tree operations (height, count, sum, min, max, mirror, etc.)
 * - Serialize/Deserialize
 * - Multiple input formats supported
 */

// ============================================================================
// TreeNode Definition (LeetCode Compatible)
// ============================================================================

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

// ============================================================================
// Tree Serializer/Deserializer (LeetCode Style)
// ============================================================================

class Codec {
public:
  // Serialize tree to LeetCode format: [1,2,3,null,null,4,5]
  static std::string serialize(TreeNode *root) {
    if (!root)
      return "[]";

    std::string result = "[";
    std::queue<TreeNode *> q;
    q.push(root);

    std::vector<std::string> values;

    while (!q.empty()) {
      TreeNode *node = q.front();
      q.pop();

      if (node) {
        values.push_back(std::to_string(node->val));
        q.push(node->left);
        q.push(node->right);
      } else {
        values.push_back("null");
      }
    }

    // Remove trailing nulls
    while (!values.empty() && values.back() == "null") {
      values.pop_back();
    }

    for (size_t i = 0; i < values.size(); i++) {
      result += values[i];
      if (i < values.size() - 1)
        result += ",";
    }

    return result + "]";
  }

  // Deserialize LeetCode format to tree: [1,2,3,null,null,4,5]
  static TreeNode *deserialize(const std::string &data) {
    if (data.empty() || data == "[]")
      return nullptr;

    // Parse the input
    std::string cleanData = data;
    // Remove brackets and spaces
    cleanData.erase(std::remove(cleanData.begin(), cleanData.end(), '['),
                    cleanData.end());
    cleanData.erase(std::remove(cleanData.begin(), cleanData.end(), ']'),
                    cleanData.end());
    cleanData.erase(std::remove(cleanData.begin(), cleanData.end(), ' '),
                    cleanData.end());

    if (cleanData.empty())
      return nullptr;

    std::vector<std::string> values;
    std::stringstream ss(cleanData);
    std::string token;

    while (std::getline(ss, token, ',')) {
      values.push_back(token);
    }

    if (values.empty() || values[0] == "null")
      return nullptr;

    TreeNode *root = new TreeNode(std::stoi(values[0]));
    std::queue<TreeNode *> q;
    q.push(root);

    size_t i = 1;
    while (!q.empty() && i < values.size()) {
      TreeNode *node = q.front();
      q.pop();

      // Left child
      if (i < values.size() && values[i] != "null") {
        node->left = new TreeNode(std::stoi(values[i]));
        q.push(node->left);
      }
      i++;

      // Right child
      if (i < values.size() && values[i] != "null") {
        node->right = new TreeNode(std::stoi(values[i]));
        q.push(node->right);
      }
      i++;
    }

    return root;
  }
};

// ============================================================================
// Tree Visualizer
// ============================================================================

class TreeVisualizer {
private:
  // Get the height of the tree
  static int getHeight(TreeNode *root) {
    if (!root)
      return 0;
    return 1 + std::max(getHeight(root->left), getHeight(root->right));
  }

  // Get maximum width of node values in tree
  static int getMaxWidth(TreeNode *root) {
    if (!root)
      return 1;
    int current = std::to_string(root->val).length();
    int leftMax = getMaxWidth(root->left);
    int rightMax = getMaxWidth(root->right);
    return std::max({static_cast<int>(current), leftMax, rightMax});
  }

  // Helper to center a string within a given width
  static std::string centerStr(const std::string &s, int width) {
    if (static_cast<int>(s.length()) >= width)
      return s;
    int padding = width - s.length();
    int leftPad = padding / 2;
    int rightPad = padding - leftPad;
    return std::string(leftPad, ' ') + s + std::string(rightPad, ' ');
  }

  // Get all nodes at each level
  static std::vector<std::vector<TreeNode *>> getLevels(TreeNode *root) {
    std::vector<std::vector<TreeNode *>> levels;
    if (!root)
      return levels;

    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty()) {
      int size = q.size();
      std::vector<TreeNode *> level;

      for (int i = 0; i < size; i++) {
        TreeNode *node = q.front();
        q.pop();
        level.push_back(node);

        if (node) {
          q.push(node->left);
          q.push(node->right);
        }
      }

      // Check if all nulls
      bool allNull = true;
      for (auto n : level) {
        if (n) {
          allNull = false;
          break;
        }
      }
      if (allNull)
        break;

      levels.push_back(level);
    }

    return levels;
  }

public:
  // Print tree with beautiful ASCII art - handles multi-digit numbers
  static void print(TreeNode *root) {
    if (!root) {
      std::cout << "\n┌─────────────────┐\n";
      std::cout << "│   Empty Tree    │\n";
      std::cout << "└─────────────────┘\n";
      return;
    }

    int height = getHeight(root);
    int maxNodeWidth = std::max(3, getMaxWidth(root) + 2); // Min 3 for branches

    // Calculate width needed for bottom level
    int bottomLevelNodes = 1 << (height - 1); // 2^(height-1)
    int totalWidth = bottomLevelNodes * maxNodeWidth * 2;

    auto levels = getLevels(root);

    std::cout << "\n";
    std::string border(totalWidth + 4, '=');
    std::cout << border << "\n";

    for (size_t levelIdx = 0; levelIdx < levels.size(); levelIdx++) {
      const auto &level = levels[levelIdx];
      int nodesInLevel = level.size();
      int spacing = totalWidth / nodesInLevel;

      // Print node values
      std::string nodeLine = "";
      for (auto node : level) {
        std::string val = node ? std::to_string(node->val) : " ";
        nodeLine += centerStr(val, spacing);
      }
      std::cout << "  " << nodeLine << "\n";

      // Print branches (if not last level)
      if (levelIdx < levels.size() - 1) {
        int branchSpacing = spacing / 2;
        int branchWidth = std::max(1, branchSpacing / 2);

        // Draw multiple lines of branches for better visual
        for (int line = 0; line < std::max(1, branchWidth / 2); line++) {
          std::string branchLine = "";
          for (auto node : level) {
            std::string leftBranch = "";
            std::string rightBranch = "";

            if (node && node->left) {
              leftBranch = "/";
            } else {
              leftBranch = " ";
            }

            if (node && node->right) {
              rightBranch = "\\";
            } else {
              rightBranch = " ";
            }

            // Position branches
            int leftPad = spacing / 2 - line - 2;
            int midPad = line * 2 + 2;
            int rightPad = spacing - leftPad - midPad - 2;

            if (leftPad < 0)
              leftPad = 0;
            if (rightPad < 0)
              rightPad = 0;

            branchLine += std::string(leftPad, ' ') + leftBranch;
            branchLine += std::string(midPad, ' ') + rightBranch;
            branchLine += std::string(rightPad, ' ');
          }
          std::cout << "  " << branchLine << "\n";
        }
      }
    }

    std::cout << border << "\n";
  }

  // Alternative visualization using box drawing characters
  static void printBoxed(TreeNode *root) {
    if (!root) {
      std::cout << "\n[Empty Tree]\n";
      return;
    }

    int height = getHeight(root);
    int maxW = getMaxWidth(root);

    auto levels = getLevels(root);

    // Calculate spacing
    int bottomNodes = 1 << (height - 1);
    int cellWidth = maxW + 2;
    int totalWidth = bottomNodes * cellWidth * 2;

    std::cout << "\n";
    std::cout << std::string(totalWidth, '-') << "\n";

    for (size_t lvl = 0; lvl < levels.size(); lvl++) {
      int nodesAtLevel = levels[lvl].size();
      int spacing = totalWidth / nodesAtLevel;

      // Print values
      for (auto node : levels[lvl]) {
        std::string val = node ? std::to_string(node->val) : "·";
        std::cout << centerStr(val, spacing);
      }
      std::cout << "\n";

      // Print connectors
      if (lvl < levels.size() - 1) {
        for (auto node : levels[lvl]) {
          std::string conn = "";
          if (node) {
            if (node->left && node->right)
              conn = "|+|";
            else if (node->left)
              conn = "/  ";
            else if (node->right)
              conn = "  \\";
            else
              conn = "   ";
          } else {
            conn = "  ";
          }
          std::cout << centerStr(conn, spacing);
        }
        std::cout << "\n";
      }
    }

    std::cout << std::string(totalWidth, '-') << "\n";
  }

  // Print compact representation - better for large trees
  static void printCompact(TreeNode *root, const std::string &prefix = "",
                           bool isLeft = true) {
    if (!root)
      return;

    if (root->right) {
      printCompact(root->right, prefix + (isLeft ? "│   " : "    "), false);
    }

    std::cout << prefix << (isLeft ? "└── " : "┌── ") << root->val << "\n";

    if (root->left) {
      printCompact(root->left, prefix + (isLeft ? "    " : "│   "), true);
    }
  }
};

// ============================================================================
// Tree Traversals
// ============================================================================

class TreeTraversals {
public:
  static std::vector<int> inorder(TreeNode *root) {
    std::vector<int> result;
    inorderHelper(root, result);
    return result;
  }

  static std::vector<int> preorder(TreeNode *root) {
    std::vector<int> result;
    preorderHelper(root, result);
    return result;
  }

  static std::vector<int> postorder(TreeNode *root) {
    std::vector<int> result;
    postorderHelper(root, result);
    return result;
  }

  static std::vector<std::vector<int>> levelOrder(TreeNode *root) {
    std::vector<std::vector<int>> result;
    if (!root)
      return result;

    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty()) {
      int size = q.size();
      std::vector<int> level;

      for (int i = 0; i < size; i++) {
        TreeNode *node = q.front();
        q.pop();
        level.push_back(node->val);

        if (node->left)
          q.push(node->left);
        if (node->right)
          q.push(node->right);
      }

      result.push_back(level);
    }

    return result;
  }

private:
  static void inorderHelper(TreeNode *root, std::vector<int> &result) {
    if (!root)
      return;
    inorderHelper(root->left, result);
    result.push_back(root->val);
    inorderHelper(root->right, result);
  }

  static void preorderHelper(TreeNode *root, std::vector<int> &result) {
    if (!root)
      return;
    result.push_back(root->val);
    preorderHelper(root->left, result);
    preorderHelper(root->right, result);
  }

  static void postorderHelper(TreeNode *root, std::vector<int> &result) {
    if (!root)
      return;
    postorderHelper(root->left, result);
    postorderHelper(root->right, result);
    result.push_back(root->val);
  }
};

// ============================================================================
// Tree Operations
// ============================================================================

class TreeOperations {
public:
  static int height(TreeNode *root) {
    if (!root)
      return 0;
    return 1 + std::max(height(root->left), height(root->right));
  }

  static int countNodes(TreeNode *root) {
    if (!root)
      return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
  }

  static int countLeaves(TreeNode *root) {
    if (!root)
      return 0;
    if (!root->left && !root->right)
      return 1;
    return countLeaves(root->left) + countLeaves(root->right);
  }

  static int sum(TreeNode *root) {
    if (!root)
      return 0;
    return root->val + sum(root->left) + sum(root->right);
  }

  static int minValue(TreeNode *root) {
    if (!root)
      return INT_MAX;
    return std::min({root->val, minValue(root->left), minValue(root->right)});
  }

  static int maxValue(TreeNode *root) {
    if (!root)
      return INT_MIN;
    return std::max({root->val, maxValue(root->left), maxValue(root->right)});
  }

  static int diameter(TreeNode *root) {
    int result = 0;
    diameterHelper(root, result);
    return result;
  }

  static bool isBalanced(TreeNode *root) { return balancedHeight(root) != -1; }

  static bool isBST(TreeNode *root) {
    return isBSTHelper(root, LONG_MIN, LONG_MAX);
  }

  static TreeNode *mirror(TreeNode *root) {
    if (!root)
      return nullptr;
    TreeNode *newNode = new TreeNode(root->val);
    newNode->left = mirror(root->right);
    newNode->right = mirror(root->left);
    return newNode;
  }

  static TreeNode *invert(TreeNode *root) {
    if (!root)
      return nullptr;
    std::swap(root->left, root->right);
    invert(root->left);
    invert(root->right);
    return root;
  }

  static void deleteTree(TreeNode *root) {
    if (!root)
      return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
  }

private:
  static int diameterHelper(TreeNode *root, int &result) {
    if (!root)
      return 0;
    int left = diameterHelper(root->left, result);
    int right = diameterHelper(root->right, result);
    result = std::max(result, left + right);
    return 1 + std::max(left, right);
  }

  static int balancedHeight(TreeNode *root) {
    if (!root)
      return 0;
    int left = balancedHeight(root->left);
    int right = balancedHeight(root->right);
    if (left == -1 || right == -1 || std::abs(left - right) > 1)
      return -1;
    return 1 + std::max(left, right);
  }

  static bool isBSTHelper(TreeNode *root, long minVal, long maxVal) {
    if (!root)
      return true;
    if (root->val <= minVal || root->val >= maxVal)
      return false;
    return isBSTHelper(root->left, minVal, root->val) &&
           isBSTHelper(root->right, root->val, maxVal);
  }
};

// ============================================================================
// Helper Functions
// ============================================================================

void printVector(const std::vector<int> &v, const std::string &name) {
  std::cout << name << ": [";
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << v[i];
    if (i < v.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

void printLevelOrder(const std::vector<std::vector<int>> &levels) {
  std::cout << "Level Order: [";
  for (size_t i = 0; i < levels.size(); i++) {
    std::cout << "[";
    for (size_t j = 0; j < levels[i].size(); j++) {
      std::cout << levels[i][j];
      if (j < levels[i].size() - 1)
        std::cout << ", ";
    }
    std::cout << "]";
    if (i < levels.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]\n";
}

// ============================================================================
// Interactive Menu
// ============================================================================

class TreeApp {
private:
  TreeNode *root;

  void printWelcome() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════════════════════╗
║                    🌲 BINARY TREE VISUALIZER 🌲                               ║
║                        LeetCode Style Edition                                ║
╠══════════════════════════════════════════════════════════════════════════════╣
║  Enter trees using LeetCode format: [1,2,3,null,4,5]                         ║
╚══════════════════════════════════════════════════════════════════════════════╝
)";
  }

  void printHelp() {
    std::cout << R"(
┌────────────────────────── INPUT FORMAT ──────────────────────────────────────┐
│                                                                              │
│  LeetCode Format: [1,2,3,null,null,4,5]                                      │
│                                                                              │
│  This represents a level-order traversal where:                              │
│   - Numbers are node values                                                  │
│   - 'null' represents missing nodes                                          │
│                                                                              │
│  Example: [1,2,3,null,null,4,5]                                              │
│                                                                              │
│           1              Level 0                                             │
│          / \                                                                 │
│         2   3            Level 1                                             │
│            / \                                                               │
│           4   5          Level 2                                             │
│                                                                              │
│  More Examples:                                                              │
│   - [1]                  → Single node                                       │
│   - [1,2,3]              → Complete tree with 3 nodes                        │
│   - [1,null,2,null,3]    → Right-skewed tree                                 │
│   - [3,9,20,null,null,15,7] → Example from LeetCode                          │
│                                                                              │
└──────────────────────────────────────────────────────────────────────────────┘
)";
  }

  void printMenu() {
    std::cout << R"(
┌───────────────────── MENU ─────────────────────┐
│                                                │
│  📝 INPUT                                      │
│   1. Enter tree (LeetCode format)              │
│   2. Load example tree                         │
│                                                │
│  🎨 VISUALIZATION                              │
│   3. Print tree (ASCII art)                    │
│   4. Print tree (compact view)                 │
│   5. Show serialized form                      │
│                                                │
│  🔄 TRAVERSALS                                 │
│   6. Inorder traversal                         │
│   7. Preorder traversal                        │
│   8. Postorder traversal                       │
│   9. Level-order traversal                     │
│                                                │
│  📊 OPERATIONS                                 │
│  10. Show tree statistics                      │
│  11. Check if BST                              │
│  12. Check if balanced                         │
│  13. Invert/Mirror tree                        │
│                                                │
│  ❓ HELP                                       │
│  14. Show input format help                    │
│  15. Run demo tests                            │
│                                                │
│   0. Exit                                      │
└────────────────────────────────────────────────┘
)";
  }

public:
  TreeApp() : root(nullptr) {}

  ~TreeApp() { TreeOperations::deleteTree(root); }

  void run() {
    printWelcome();

    int choice;
    bool running = true;

    while (running) {
      printMenu();
      std::cout << "👉 Your choice: ";

      if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "❌ Invalid input. Please enter a number.\n";
        continue;
      }

      switch (choice) {
      case 1: {
        std::cout << "\n📝 Enter tree in LeetCode format\n";
        std::cout << "   Example: [1,2,3,null,4,5]\n";
        std::cout << "   👉 Input: ";
        std::cin.ignore();
        std::string input;
        std::getline(std::cin, input);

        try {
          TreeOperations::deleteTree(root);
          root = Codec::deserialize(input);
          std::cout << "   ✅ Tree created successfully!\n";
          TreeVisualizer::print(root);
        } catch (...) {
          std::cout << "   ❌ Error parsing input. Please check format.\n";
        }
        break;
      }

      case 2: {
        std::cout << "\n📋 Select example:\n";
        std::cout << "   1. [1,2,3,4,5,6,7] - Complete binary tree\n";
        std::cout << "   2. [3,9,20,null,null,15,7] - LeetCode example\n";
        std::cout << "   3. [1,2,null,3,null,4,null,5] - Left skewed\n";
        std::cout << "   4. [5,3,7,2,4,6,8] - BST\n";
        std::cout << "   👉 Choice: ";

        int ex;
        std::cin >> ex;

        TreeOperations::deleteTree(root);

        switch (ex) {
        case 1:
          root = Codec::deserialize("[1,2,3,4,5,6,7]");
          break;
        case 2:
          root = Codec::deserialize("[3,9,20,null,null,15,7]");
          break;
        case 3:
          root = Codec::deserialize("[1,2,null,3,null,4,null,5]");
          break;
        case 4:
          root = Codec::deserialize("[5,3,7,2,4,6,8]");
          break;
        default:
          std::cout << "   Invalid choice.\n";
          continue;
        }

        std::cout << "   ✅ Example loaded!\n";
        TreeVisualizer::print(root);
        break;
      }

      case 3:
        TreeVisualizer::print(root);
        break;

      case 4:
        if (!root) {
          std::cout << "\n❌ Tree is empty.\n";
        } else {
          std::cout << "\n🌲 Compact View:\n\n";
          TreeVisualizer::printCompact(root);
        }
        break;

      case 5:
        std::cout << "\n📦 Serialized: " << Codec::serialize(root) << "\n";
        break;

      case 6:
        printVector(TreeTraversals::inorder(root), "Inorder");
        break;

      case 7:
        printVector(TreeTraversals::preorder(root), "Preorder");
        break;

      case 8:
        printVector(TreeTraversals::postorder(root), "Postorder");
        break;

      case 9:
        printLevelOrder(TreeTraversals::levelOrder(root));
        break;

      case 10: {
        std::cout << "\n📊 Tree Statistics:\n";
        std::cout << "   Height:      " << TreeOperations::height(root) << "\n";
        std::cout << "   Node count:  " << TreeOperations::countNodes(root)
                  << "\n";
        std::cout << "   Leaf count:  " << TreeOperations::countLeaves(root)
                  << "\n";
        std::cout << "   Sum:         " << TreeOperations::sum(root) << "\n";
        if (root) {
          std::cout << "   Min value:   " << TreeOperations::minValue(root)
                    << "\n";
          std::cout << "   Max value:   " << TreeOperations::maxValue(root)
                    << "\n";
          std::cout << "   Diameter:    " << TreeOperations::diameter(root)
                    << "\n";
        }
        break;
      }

      case 11:
        std::cout << "\n"
                  << (TreeOperations::isBST(root)
                          ? "✅ This IS a valid Binary Search Tree"
                          : "❌ This is NOT a valid Binary Search Tree")
                  << "\n";
        break;

      case 12:
        std::cout << "\n"
                  << (TreeOperations::isBalanced(root)
                          ? "✅ This tree IS balanced"
                          : "❌ This tree is NOT balanced")
                  << "\n";
        break;

      case 13: {
        if (!root) {
          std::cout << "\n❌ Tree is empty.\n";
          break;
        }
        std::cout << "\n🔄 Original tree:\n";
        TreeVisualizer::print(root);

        TreeNode *mirrored = TreeOperations::mirror(root);
        std::cout << "\n🪞 Mirrored tree:\n";
        TreeVisualizer::print(mirrored);
        TreeOperations::deleteTree(mirrored);
        break;
      }

      case 14:
        printHelp();
        break;

      case 15:
        runDemoTests();
        break;

      case 0:
        std::cout << "\n👋 Goodbye!\n";
        running = false;
        break;

      default:
        std::cout << "❌ Invalid choice.\n";
      }
    }
  }

  void runDemoTests() {
    std::cout << "\n=== Running Demo Tests ===\n";

    // Test 1: Empty tree
    std::cout << "\n📌 Test 1: Empty tree\n";
    TreeNode *t1 = Codec::deserialize("[]");
    TreeVisualizer::print(t1);
    TreeOperations::deleteTree(t1);

    // Test 2: Single node
    std::cout << "\n📌 Test 2: Single node [42]\n";
    TreeNode *t2 = Codec::deserialize("[42]");
    TreeVisualizer::print(t2);
    TreeOperations::deleteTree(t2);

    // Test 3: Complete binary tree
    std::cout << "\n📌 Test 3: Complete tree [1,2,3,4,5,6,7]\n";
    TreeNode *t3 = Codec::deserialize("[1,2,3,4,5,6,7]");
    TreeVisualizer::print(t3);
    std::cout << "Serialized: " << Codec::serialize(t3) << "\n";
    TreeOperations::deleteTree(t3);

    // Test 4: LeetCode example
    std::cout << "\n📌 Test 4: LeetCode example [3,9,20,null,null,15,7]\n";
    TreeNode *t4 = Codec::deserialize("[3,9,20,null,null,15,7]");
    TreeVisualizer::print(t4);
    printVector(TreeTraversals::inorder(t4), "Inorder");
    TreeOperations::deleteTree(t4);

    // Test 5: BST check
    std::cout << "\n📌 Test 5: BST [5,3,7,2,4,6,8]\n";
    TreeNode *t5 = Codec::deserialize("[5,3,7,2,4,6,8]");
    TreeVisualizer::print(t5);
    std::cout << "Is BST: " << (TreeOperations::isBST(t5) ? "Yes ✅" : "No ❌")
              << "\n";
    std::cout << "Is Balanced: "
              << (TreeOperations::isBalanced(t5) ? "Yes ✅" : "No ❌") << "\n";
    TreeOperations::deleteTree(t5);

    std::cout << "\n=== All Tests Complete ===\n";
  }
};

// ============================================================================
// Main
// ============================================================================

int main(int argc, char *argv[]) {
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--test" || arg == "-t") {
      TreeApp app;
      app.runDemoTests();
      return 0;
    } else if (arg == "--help" || arg == "-h") {
      std::cout << R"(
🌲 Binary Tree Visualizer - LeetCode Style

Usage:
  ./printing_tree           Interactive mode
  ./printing_tree --test    Run demo tests
  ./printing_tree --help    Show this help

Input Format:
  Use LeetCode-style level order format: [1,2,3,null,4,5]
  - Numbers are node values
  - 'null' represents missing/empty nodes

Examples:
  [1,2,3]                 → Complete tree with 3 nodes
  [1,null,2,null,3]       → Right-skewed tree
  [3,9,20,null,null,15,7] → LeetCode example
)";
      return 0;
    }
  }

  TreeApp app;
  app.run();

  return 0;
}