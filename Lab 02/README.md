# Operating Systems - Lab 02
NTUA - Electrical and Computer Engineering

## 👥 oslab064:
- Nikolaos Giannopoulos
- Konstantinos Kargakos

## 📁 Contents
- C files for each exercise (`.c`)
- `Makefile` to compile the programs
- Report describing the solutions (`OS Lab 02 - Report.pdf`)

## 🛠️ Compilation
To compile all the programs, simply run:

```bash
make
```

This will create the following executables:
- `simplesync-mutex` / `simplesync-atomic`
- `mandel`
- `mandel-semaphores`
- `mandel-condition-var`

To remove all compiled executables:
```bash
make clean
```

## 📁 Source Files Overview

| Executable File        | Description                             |
|------------------------|-----------------------------------------|
| `simplesync-mutex`     | Simplesync implemented with **mutexes**.|
| `simplesync-atomic`    | Simplesync implemented with **atomic operations**.|
| `mandel`               | The original Mandel program, including a **Ctrl-C handler**.|
| `mandel-semaphores`    | Mandel program synchronized using **semaphores**.|
| `mandel-condition-var` | Mandel program synchronized using **condition variables**.|

## 🚀 How to Run
After compiling, you can run each program with:

```bash
./simplesync-mutex
./simplesync-atomic
./mandel
./mandel-semaphores <NTHREADS>
./mandel-condition-var <NTHREADS>
```

Replace `<NTHREADS>` with the number of threads.