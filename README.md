# Course work from Intro to Compilers

* from 2023
* Definitely still some work to be done
* planning to circle back after the go compiler

[Course Page](https://sites.google.com/trinity.edu/csci3334-fl23/home?authuser=0)

## Project Overview
This repository contains coursework and projects from an Introduction to Compilers course taken in 2023. The codebase includes implementations of various compiler components and techniques.

## Repository Structure
The repository contains multiple compiler-related projects:
- `p3-wwelden`: Project 3
- `tipc-wwelden`: TIP Compiler implementation

## How to Build and Run the Compiler

### Building the Compiler
To build the TIP compiler, navigate to the `tipc-wwelden` directory and use the provided Makefile:

```bash
cd tipc-wwelden
make
```

This will generate the `tipc` executable.

### Running the Compiler
The compiler supports various command-line options:

```bash
./tipc [options] input_file
```

#### Command-line Options:
- `-h, --help`: Print the help menu
- `-p, --print`: Pretty print the input with added comments on relations
- `-t, --table`: Print the input in tabular form
- `-l, --lexer`: Print lexer tokens
- `-k NUM`: Set the number of registers for allocation (default: 3)
- `-d`: Print debug information at verbosity level 1
- `-v NUM`: Print debug information at specified verbosity level

### Examples:
```bash
# Run with register allocation (k=3) and pretty print
./tipc -k 3 test.in

# Print the input in tabular form
./tipc -t test.in

# Print lexer tokens
./tipc -l test.in
```

## Running Tests

The project includes testing scripts for different phases:

```bash
# Run tests for Project 2
./run_p2_tests.sh

# Run tests for a specific test case
./tipc -k 3 testing-compilers/blocks/block1.i
```

Test files are organized by project phase in the `testing-compilers` directory:
- `testing-compilers/p0/`: Project 0 test files
- `testing-compilers/p1/`: Project 1 test files
- `testing-compilers/p2/`: Project 2 test files
- `testing-compilers/blocks/`: Block-related test files

## Future Plans
As noted above, there are plans to revisit and improve these projects after completing work on a Go compiler.

This is a clone of the original repo.
