# SQL Clone

## Overview
This project, developed as part of EECS 281 at the University of Michigan, replicates basic SQL functionalities through a command-line interface. It leverages data structures and algorithms to efficiently manage and manipulate table data.

## Components
- **silly.cpp**: Main implementation handling input commands, parsing, and execution of operations like table creation, data insertion, querying, deleting rows, joining tables, and indexing.
  
- **TableEntry.cpp** and **TableEntry.h**: Implementation and header files for the `TableEntry` class, essential for storing and manipulating data within tables.
  
- **test-8.txt**: Test input file used for validating functionality, containing commands to test various operations.

## Functionality
- **Command Parsing and Execution**: Parses commands from input, validates them, and executes corresponding operations defined in `Nincompoop.cpp`.
  
- **Table Operations**: Supports operations such as creating tables (`CREATE`), inserting data (`INSERT`), querying (`PRINT`), deleting rows (`DELETE`), joining tables (`JOIN`), and generating indexes (`GENERATE`).
  
- **Data Types**: Handles different data types (integers, booleans, doubles, strings) ensuring correct storage and retrieval.

## How to Run
To run the SQL clone:
```bash
./silly < test-8.txt
```

Replace test-8.txt with your test file or provide commands interactively.

Notes

- Demonstrates proficiency in data structure implementations and algorithms essential for efficient management of relational database operations.

- Developed as an educational tool, applying course concepts to practical scenarios in managing large datasets and optimizing query performance.

