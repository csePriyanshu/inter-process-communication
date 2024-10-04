# Command Execution Timing using `execv` with Shared Memory, Pipes, and Basic Time Measurement

## Project Overview

This project showcases different methods for measuring the execution time of a command executed by a child process using `execv`. Three distinct approaches are explored:
1. **Basic Time Measurement (`time.c`)**: Uses the `clock_gettime()` function to measure time in both the parent and child processes.
2. **Pipe-based Inter-Process Communication (`pipe.c`)**: Uses pipes to pass the precise start time from the child process to the parent process.
3. **Shared Memory-based Communication (`shared.c`)**: Utilizes POSIX shared memory for inter-process communication, sharing the start time between child and parent processes for more accurate time measurement.

## Purpose

The project’s goal is to demonstrate the differences in accuracy, complexity, and performance overhead when using various approaches to measure the time taken for command execution. This is particularly useful for understanding inter-process communication and timing mechanisms in Linux environments.

## Features

- **Command Execution with `execv`**: Executes any specified command as a child process, allowing for accurate time measurements.
- **Time Measurement**: The command's execution time is captured using `clock_gettime()` with `CLOCK_MONOTONIC` for nanosecond-level precision.
- **Inter-Process Communication (IPC)**: Pipes and shared memory are utilized to pass the child process's start time to the parent for accurate time difference calculation.

## Files

- **`time.c`**: Uses direct time measurement in the parent process.
- **`pipe.c`**: Transfers the start time from child to parent using pipes.
- **`shared.c`**: Implements shared memory for communication between child and parent to share the start time.
- **`Makefile`**: A simple Makefile to build and run the project.

## Requirements

- GCC compiler
- POSIX library (`librt`) for shared memory support (required for `shared.c`)

## Build Instructions

To compile the project, simply run:

```bash
make
```

## Comparison of Approaches

| Approach   | Accuracy | Complexity | Overhead                                     |
|------------|----------|------------|----------------------------------------------|
| `time.c`   | Low      | Low        | High (includes scheduling and context switch overhead)      |
| `pipe.c`   | Medium   | Medium     | Medium (pipe management and IPC overhead)    |
| `shared.c` | High     | High       | Low (efficient shared memory communication)  |
