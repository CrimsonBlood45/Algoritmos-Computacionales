# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is a C++ algorithms course repository for a 3rd-semester class at IPN (Instituto Politécnico Nacional). Each project is a standalone CLI program implementing a specific algorithm or data structure.

## Build & Run

Each project is compiled independently from its directory:

```bash
# For student projects (P012–P031)
g++ main.cpp -o main

# For teacher reference versions (P033+)
g++ mainFuensVersion.cpp -o main

# Run
./main
```

No Makefile or build system — each directory is self-contained.

## Project Structure

- **P012–P031**: Student-implemented algorithms (`main.cpp`)
- **P033–P044**: Teacher reference versions by Prof. Fuenlabrada (`mainFuensVersion.cpp`)
- **Evaluacion1Dep / Eval2doDep**: Graded evaluations (may use student-name filenames like `saavedra-rodriguez-aaron-emanuel.cpp`)

Each project directory typically contains: source file, compiled binary (`main` or `a.out`), and `.txt` files for input/output.

## Algorithm Topics by Range

| Range | Topic |
|-------|-------|
| P012–P014 | Basic sorts (insertion, binary search, selection) |
| P016–P020 | Recursion (factorial, Fibonacci, Hanoi, sums) |
| P021–P024, P031–P036 | Advanced sorts (QuickSort, HeapSort, RadixSort, IntroSort, TimSort, BinSort, ShellSort) |
| P025–P030 | Strings and pointers |
| P037–P039 | Hashing (basic, improved, text replacement) |
| P040–P041 | Trees (BST, AVL) |
| P042–P044 | Combinatorial algorithms (Brute Force, TSP/Traveling Salesman, Eight Queens) |

## Common Code Patterns

All sorting programs share the same menu-driven flow:

1. **Leer Archivo** — read integers from a user-specified `.txt` file (extension added automatically)
2. **Ordenar** — apply the algorithm; timing printed via `std::chrono`
3. **Grabar Archivo** — write sorted integers to a new `.txt` file
4. **Mostrar Arreglo** — display current array contents
5. **Salir** — exit (only allowed after read + sort + save)

State is tracked with boolean flags (`read`, `sorted`, `recorded`) in a struct, and `validAction()` enforces the correct operation order by printing `#define`'d error messages in Spanish.

## Language & Style

- All UI text, variable names, function names, and comments are in **Spanish**
- Uses `setlocale(LC_ALL, "")` for locale support
- Raw arrays with manual `new`/`delete` (no STL containers in earlier projects)
- `using namespace std;` is standard across all files
- `std::chrono::high_resolution_clock` used for timing sort operations
