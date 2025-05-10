# Parallel Sorting with Two Threads

## Description

This program implements a simple sort using two threads with POSIX pthreads.

- **Thread T1** handles even-indexed pairs (`A[0]&A[1]`, `A[2]&A[3]`, ...).
- **Thread T2** handles odd-indexed pairs (`A[1]&A[2]`, `A[3]&A[4]`, ...).
- Threads take **alternating turns**, using **mutexes** and **condition variables**.
- Sorting stops when **both threads perform zero swaps** in the same roun

## Compilation & Usage

**To compile:**

```bash
gcc main.c -o main
```

**To run:**

```bash
./main ToSort
```
