# Simple Calculator (C++ Course Work)

## Overview

This project implements a command-line calculator in C++ that:

- Reads an input text file containing one or more _sessions_.
- Each session starts with a `----` header line, followed by:
  - Optional variable definitions (e.g. `a = 10`)
  - A single expression to evaluate.
- Supports numbers in bases:
  - Binary (e.g. `1100b`)
  - Decimal (e.g. `100`)
  - Hexadecimal (e.g. `0x1F`)
- Supports operators: `+ - * / ^ ( )`
- Supports functions: `sin()` and `cos()`
- Supports variables and prints results in decimal.
- Usage format:

```bash
calc inputFileName
```
