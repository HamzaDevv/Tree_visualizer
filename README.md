# 🌲 Binary Tree Visualizer

A robust C++ program for visually printing binary trees in the terminal with ASCII art.

## Features

- **Visual Tree Printing** - Displays trees with `/` and `\` branch connectors
- **Multi-digit Support** - Handles numbers with multiple digits
- **Negative Numbers** - Full support for negative values
- **Edge Case Handling** - Empty trees, single nodes, skewed trees
- **Interactive Menu** - Easy-to-use interface
- **Memory Safe** - Proper cleanup with destructor
- **Built-in Tests** - Verify functionality

## Compilation

```bash
g++ -std=c++17 -Wall -o printing_tree printing_tree.cpp
```

## Usage

```bash
./printing_tree           # Interactive mode
./printing_tree --test    # Run tests
./printing_tree --help    # Show help
```

## Input Format

Enter tree data in **preorder** format. Use `-1` to indicate null nodes.

```
Example: 5 3 1 -1 -1 4 -1 -1 7 6 -1 -1 8 -1 -1

Creates:
       5
      / \
     3   7
    / \ / \
   1  4 6  8
```

## Sample Output

```
============
   5    
  / \   
 /   \  
 3   7  
/ \ / \ 
1 4 6 8 
============
```

## Menu Options

| Option | Description |
|--------|-------------|
| 1 | Build tree from preorder input |
| 2 | Print current tree |
| 3 | Show tree statistics |
| 4 | Clear tree |
| 5 | Load example tree |
| 0 | Exit |

## License

MIT License
