# Operating Systems - Lab 01
NTUA - Electrical and Computer Engineering

## 👥 oslab064:
- Nikolaos Giannopoulos
- Konstantinos Kargakos

## 📁 Contents
- C files for each exercise (`.c`)
- `Makefile` to compile the programs
- Report describing the solutions (`OS Lab 01 - Report.pdf`)
- Input demo text file (`text-s.txt`)
- Output demo text file (`result.txt`)

## 🛠️ Compilation
To compile all the programs, simply run:

```bash
make
```

This will create the following executables:
- `main-source-code`
- `ex01`
- `ex02-1` / `ex02-2` / `ex02-3` / `ex02-4`
- `ex03`

To remove all compiled executables:
```bash
make clean
```

## 🚀 How to Run
After compiling, you can run each program with:

```bash
./<executable> <text.txt> <result.txt> <character>
```

- `<text.txt>` → Path to the input text file
- `<result.txt>` → Path where the result will be saved
- `<character>` → A single character to search

ℹ️ **Note:** `ex02-1` & `ex02-2` do **not** require any command-line arguments to run.

## 📁 Source Files Overview

| Source File       | Exercise     | Compiled File  |
|-------------------|--------------|----------------|
| `main-source-code.c` | Given Code   | `main-source-code` |
| `ex01.c`          | Exercise 1   | `ex01`         |
| `ex02-1.c`        | Exercise 2.1 | `ex02-1`       |
| `ex02-2.c`        | Exercise 2.2 | `ex02-2`       |
| `ex02-3.c`        | Exercise 2.3 | `ex02-3`       |
| `ex02-4.c`        | Exercise 2.4 | `ex02-4`       |
| `ex03.c`          | Exercise 3   | `ex03`         |
