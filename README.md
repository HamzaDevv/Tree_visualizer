# 🌲 Binary Tree Visualizer - LeetCode Style

A robust C++ library for visualizing binary trees using the same input format as LeetCode.

## ✨ Features

### 📝 LeetCode-Style Input
```cpp
[1,2,3,null,4,5]   // Just like LeetCode!
```

### 🎨 Beautiful ASCII Visualization
```
         5        
      /// \\\     
     3       7    
    / \     / \   
   2   4   6   8  
```

### 🔄 All Tree Traversals
- Inorder, Preorder, Postorder
- Level-order with levels separated

### 📊 Tree Operations
- Height, Node count, Leaf count
- Sum, Min, Max values
- Diameter calculation
- BST validation
- Balance check
- Mirror/Invert tree

### 💾 Serialize/Deserialize
Convert between tree and LeetCode string format.

## 🚀 Quick Start

### Compilation
```bash
g++ -std=c++17 -Wall -o tree_visualizer printing_tree.cpp
```

### Usage
```bash
./tree_visualizer           # Interactive mode
./tree_visualizer --test    # Run demo tests
./tree_visualizer --help    # Show help
```

## 📋 Input Format

Use LeetCode-style level order format:

| Input | Tree |
|-------|------|
| `[1]` | Single node |
| `[1,2,3]` | Complete 3-node tree |
| `[1,null,2]` | Right child only |
| `[3,9,20,null,null,15,7]` | LeetCode example |

### Example
```
Input: [3,9,20,null,null,15,7]

           3              Level 0
          / \
         9   20           Level 1
            / \
           15  7          Level 2
```

## 🔧 Using as a Library

```cpp
#include "printing_tree.cpp"

// Create tree from LeetCode format
TreeNode* root = Codec::deserialize("[1,2,3,4,5]");

// Visualize
TreeVisualizer::print(root);

// Traversals
auto inorder = TreeTraversals::inorder(root);

// Operations
int h = TreeOperations::height(root);
bool isBST = TreeOperations::isBST(root);
bool isBalanced = TreeOperations::isBalanced(root);

// Mirror the tree
TreeNode* mirrored = TreeOperations::mirror(root);

// Serialize back to string
std::string serialized = Codec::serialize(root);

// Don't forget to cleanup!
TreeOperations::deleteTree(root);
```

## 📚 API Reference

### Codec
| Method | Description |
|--------|-------------|
| `serialize(root)` | Convert tree to LeetCode string |
| `deserialize(str)` | Create tree from LeetCode string |

### TreeVisualizer
| Method | Description |
|--------|-------------|
| `print(root)` | ASCII art visualization |
| `printCompact(root)` | Compact tree view |

### TreeTraversals
| Method | Description |
|--------|-------------|
| `inorder(root)` | Left → Root → Right |
| `preorder(root)` | Root → Left → Right |
| `postorder(root)` | Left → Right → Root |
| `levelOrder(root)` | Level by level |

### TreeOperations
| Method | Description |
|--------|-------------|
| `height(root)` | Tree height |
| `countNodes(root)` | Total node count |
| `countLeaves(root)` | Leaf node count |
| `sum(root)` | Sum of all values |
| `minValue(root)` | Minimum value |
| `maxValue(root)` | Maximum value |
| `diameter(root)` | Longest path length |
| `isBST(root)` | Check if valid BST |
| `isBalanced(root)` | Check if balanced |
| `mirror(root)` | Create mirrored copy |
| `invert(root)` | Invert in-place |
| `deleteTree(root)` | Free memory |

## 📸 Sample Output

```
╔══════════════════════════════════════════════════════════════════════════════╗
║                    🌲 BINARY TREE VISUALIZER 🌲                               ║
║                        LeetCode Style Edition                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

📌 Complete tree [1,2,3,4,5,6,7]:

====================
         1        
      /// \\\     
     2       3    
    / \     / \   
   4   5   6   7  
====================

📊 Tree Statistics:
   Height:      3
   Node count:  7
   Leaf count:  4
   Is BST:      No
   Is Balanced: Yes
```

## 📄 License

MIT License
