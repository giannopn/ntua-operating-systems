# Operating Systems - Lab 03
NTUA - Electrical and Computer Engineering

## 👥 oslab064:
- Nikolaos Giannopoulos
- Konstantinos Kargakos

## 📁 Contents
- `1-mmap/`: Source code and Makefile for task 1
- `2-mandel/`: Source code and Makefile for task 2.1
- `3-mandel-without-semaphores/`: Source code and Makefile for task 2.2
- `4-lab01-extension/`: Source code and Makefile for task 3
- Report describing the solutions (`OS Lab 03 - Report.pdf`)

## 🛠️ Compilation
Each task has its own dedicated directory. To compile a specific program, navigate into its folder and run the `make` command:

```bash
cd 1-mmap/  # or 2-mandel/, 3-mandel-without-semaphores/, 4-lab01-extension/
make
```

This will create the corresponding executable file in that directory:
- `1-mmap/mmap`
- `2-mandel/mandel-fork`
- `3-mandel-without-semaphores/mandel-fork`
- `4-lab01-extension/ex03`

To remove all compiled executables in each directory, you can run:
```bash
make clean
```

## 📁 Source Files Overview

| Executable File                           | Task | Description                        |
|:------------------------------------------| :--- |:-----------------------------------|
| `1-mmap/mmap`                             | 1    | mmap completed                     |
| `2-mandel/mandel-fork`                    | 2.1  | Mandelbrot with processes          |
| `3-mandel-without-semaphores/mandel-fork` | 2.2  | Mandelbrot without semaphores      |
| `4-lab01-extension/ex03`                  | 3    | Lab01 (Character Search) extension |

## 🚀 How to Run
After compiling, you can run each program in its directory with:

```bash
./mmap
./mandel-fork <NPROCS>
./ex03 <text.txt> <result.txt> <character>
```

- Replace `<NPROCS>` with the number of prosecces.
- Replace `<text.txt>`, `<result.txt>` with the input & output text file and `<character>` with a single character search.