# Vault Heist: Coding Conventions

## 1. Language and library rules

- C++17. **No templates written by us and no STL containers**
  (`vector`, `stack`, `queue`, `map`, `set`, `list`, `priority_queue`, `deque`, `array`).
- Allowed headers: `<cstdio> <cstdlib> <cstring> <cmath> <cstdint> <chrono> <iostream> <fstream>`.
- Not used: `<algorithm>` (write our own sorts and min/max), `<string>`/`<vector>` inside `src/core`,
  `src/events` and `src/model`. Event notes use `const char*` string literals.
- Our containers: `IntArray`, `NodeStack`, `NodeQueue` (in `src/core/`).
- The GUI layer may use whatever SFML itself requires (e.g. `sf::String`).
- Sorted-reference in tests: our own simple insertion sort, not `std::sort`.
- Open question for instructor: is `std::string` allowed outside the model? (Needed for JSON/save files.)

## 2. Definitions

- **Height counts nodes.** Empty tree = 0, leaf = 1. This matches the AVL spec.
  Balance factor = height(left) - height(right).
- **Depth** of the root = 0.
- **Node id**: positive `int`, unique, assigned from a counter at node creation, never reused.
  **`0` means "no node"** (null parent, no child, etc.). Rotations and value swaps never change ids.
- **Value / key type** is `int` everywhere. BST key and value are the same field (`value`).
- **Child side** encoding: `0 = left`, `1 = right`, `-1 = root / none`.
- **Heap**: zero-based array. parent = (i-1)/2, left = 2i+1, right = 2i+2.
  `size` = elements stored in the array, `heapSize` = active heap prefix. Sorted region = indices `[heapSize, size)`.

## 3. Return values

- Traversals return an `IntArray` of **node ids** (not values).
- No exceptions. Failures are reported via `bool`, `nullptr`, or an enum status
  (e.g. `BST::insert` returns `false` on duplicate).
- Invalid input (mismatched lengths in `build`, `extractMax` on empty) returns `nullptr` / `false` and never crashes.

## 4. Events and logging

- Every model operation that has visual effect takes a trailing `EventLog* log = nullptr`.
- `nullptr` means "do not record". Benchmarks pass `nullptr`.
- Emit through the helper `if (log) log->emit(...)`, never directly.
- Events are emitted **in the order the changes happen**, each describing one atomic step.
- Every operation that emits events ends with exactly one `DONE`.
- Event field meanings are frozen in `docs/events.md`. Do not change them without both members agreeing.

## 5. Ownership and memory

- A tree **owns** its nodes. The destructor frees every node (iteratively or recursively, but must not leak).
- Copying is **disabled** (`= delete`). Use `clone()` to copy a tree (needed for snapshot undo).
- A node never owns its parent. `parent` is a non-owning back pointer.
- `NodeStack`/`NodeQueue` hold non-owning `TreeNode*`.
- Run all tests under AddressSanitizer (default Debug build). A leak is a failed test.

## 6. Style

- Indentation: 4 spaces, no tabs. Braces on the same line (K&R). Always use braces, even for one-line `if`.
- Types `PascalCase`, functions and variables `camelCase`, constants and enum values `UPPER_SNAKE`.
- Private members use the `m_` prefix (`m_root`, `m_size`).
- Files: one class per `.h/.cpp`, named after the class. `#pragma once` in every header.
- Headers include only what they need. `.cpp` files include their own header first.
- Include paths are rooted at `src/`: `#include "core/IntArray.h"`.
- Comment the **why**, not the what. Every non-obvious algorithm (iterative postorder, delete case 3,
  rebalancing) gets a short header comment explaining the idea.
- Line length: aim for 100.
- Compile with `-Wall -Wextra -Wpedantic` and fix every warning.

## 7. Git workflow

- Each person edits only their own files. Shared files (`Event.h`, `CMakeLists.txt`, test harness, docs)
  are changed only after telling the other person.
- `main` must always build and pass `ctest`.
- Pull before starting work, push when done. Small commits with clear messages.

## 8. Testing

- One test file per module: `test_core.cpp`, `test_events.cpp`, `test_trees.cpp`, `test_heap.cpp`.
- Shared `test_harness.h` provides `CHECK(cond)` and `CHECK_EQ(a, b)`; `test_main.cpp` runs all and prints a summary.
- After every mutating operation in a test, run the matching Validator function.
- Tests for module X are reviewed by the other member.

## 9. Ownership of modules (Part 1)

| Member | Files |
| --- | --- |
| A | `TreeNode`, `BinaryTree`, `BST` (and later `AVLTree`), tree part of `Validator`, `test_trees.cpp` |
| B | `IntArray`, `NodeStack`, `NodeQueue`, `EventLog`, `MaxHeap`, heap part of `Validator`, `test_core.cpp`, `test_events.cpp`, `test_heap.cpp` |
| Shared | `Event.h`, `docs/*`, `CMakeLists.txt`, `test_harness.h` |
