# C Preprocessor

A simplified C preprocessor implemented in C, with separate modules for comment removal, header file inclusion and macro expansion.

## Overview

The program takes a C source file as input, processes it through multiple stages and generates a preprocessed `.i` file.

## Functionality

- **Comment Removal** — Removes single-line and multi-line comments.
- **File Inclusion** — Handles local header files using `#include "file.h"`.
- **Macro Expansion** — Supports object-like and function-like macros.
- **Build Automation** — Uses a Makefile to compile the project.

## Project Structure

```text
C---preprocessor/
│
├── README.md
│
└── code/
    ├── main.c
    ├── comment_removal.c
    ├── comment_removal.h
    ├── file_inclusion.c
    ├── file_inclusion.h
    ├── macro_handler.c
    ├── macro_handler.h
    ├── Makefile
    └── myheader.h
```

## Build

Requirements: GCC and GNU Make.

```bash
cd code
make
```

## Usage

Run the preprocessor with a C source file:

```bash
./mypreprocessor input.c
```

The processed output is saved as:

```text
input.i
```

Compile and run the generated file:

```bash
gcc -x c input.i -o output
./output
```

## Processing Flow

```text
    input.c
       |
       v
 File Inclusion
       |
       v
 Comment Removal
       |
       v
 Macro Expansion
       |
       v
    input.i
```

## Implementation

The project is divided into independent source and header files. Each module handles a specific preprocessing task, while `main.c` coordinates the overall execution.

---

**Language:** C  
**Compiler:** GCC  
**Build System:** GNU Make  
**Environment:** Linux
