# Command Execution Timing using `execv` with Shared Memory, Pipes, and Basic Time Measurement

## Overview

This project demonstrates three different approaches to measure the execution time of a command executed by a child process using `execv`:
1. **Basic Time Measurement (`time.c`)**: Uses `clock_gettime()` in both the parent and child processes.
2. **Pipes for Inter-Process Communication (`pipe.c`)**: Uses pipes to transfer the start time from the child process to the parent process.
3. **Shared Memory (`shared.c`)**: Uses POSIX shared memory to communicate the precise start time from the child to the parent process.

## Key Features

- **`execv`**: Executes the command specified as a command-line argument.
- **Time Measurement**: The execution time of the command is measured using `clock_gettime()` with `CLOCK_MONOTONIC` for high precision.
- **Inter-Process Communication**: Both pipes and shared memory are used to pass data (e.g., start time) between parent and child processes.

## Files

- `time.c`: Measures execution time directly in the parent process (simplistic approach).
- `pipe.c`: Uses pipes to transfer the start time from child to parent.
- `shared.c`: Uses POSIX shared memory for inter-process communication to share the start time.
- `Makefile`: A simple Makefile to build and run the project.

## Requirements

- GCC compiler
- POSIX library (`librt`) for shared memory functions (required for `shared.c`)

## Build

To compile the project, simply run:

```bash
make

