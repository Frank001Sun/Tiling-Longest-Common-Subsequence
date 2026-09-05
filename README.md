# Tiling Dynamic Programming Computations to Maximize Parallelism

Companion code for the paper **"Tiling Dynamic Programming Computations to
Maximize Parallelism"** ([DOI: 10.1109/HPCC67675.2025.00077](https://doi.org/10.1109/HPCC67675.2025.00077)),
published at the 2025 IEEE International Conference on High Performance
Computing and Communications (HPCC).

This repository contains the sequential baseline, an OpenMP baseline, and the
tiled parallel implementation of the Longest Common Subsequence (LCS) dynamic
program described in the paper, together with a script for generating test
inputs.

## Problem

All implementations compute the length of the longest common subsequence (LCS)
of two input strings using the classic dynamic programming recurrence:

```
dp[i][j] = max(dp[i-1][j], dp[i][j-1], dp[i-1][j-1] + (A[i] == B[j]))
```

## Implementations

| File | Description |
|------|-------------|
| `plcs.c` | Original sequential implementation (baseline). |
| `plcs.omp.c` | OpenMP baseline: parallelizes the computation one anti-diagonal (wavefront) at a time. |
| `plcs.tile.c` | **Main implementation of the paper.** Partitions the columns of the DP table into `T` contiguous bands, one per thread, and synchronizes them with per-band semaphores so that each thread advances down its band as soon as its dependencies allow. |
| `plcs.queue.c` | Additional variant: dynamic tile scheduling through a shared work queue, where each worker computes a tile and enqueues the newly enabled neighboring tiles. Third-party implementation — see [Third-Party Code](#third-party-code). |
| `thread.h`, `thread-sync.h` | Minimal threading library on top of POSIX threads (create/join, spinlock, mutex, condition variable, semaphore), adapted from the NJU OS course lab framework — see [Third-Party Code](#third-party-code). |
| `generate_sequences.sh` | Script to generate random test inputs. |
| `stdin_1000` … `stdin_10000` | Pre-generated test inputs: two random strings of the indicated length. |
| `Makefile` | Build script. |

## Building

Requirements: Linux, GCC with OpenMP support, GNU Make, and Bash.

```sh
make
```

This compiles every `.c` file into an executable of the same name: `plcs`,
`plcs.omp`, `plcs.queue`, and `plcs.tile`.

```sh
make clean   # remove all executables
```

## Usage

Every executable takes exactly one command-line argument: the number of
threads. Input is read from standard input, so redirect one of the bundled
input files (or any file in the format below):

```sh
./<executable> <num-threads> < <input-file>
```

Examples:

```sh
./plcs 1 < stdin_10000       # sequential baseline
./plcs.omp 8 < stdin_10000   # OpenMP baseline with 8 threads
./plcs.tile 8 < stdin_10000  # tiled implementation with 8 threads
./plcs.queue 8 < stdin_10000 # work-queue variant with 8 threads
```

## Input format

The input consists of two lines:

- line 1: string `A`
- line 2: string `B`

The maximum supported length is 10,000 characters per string (`MAXN`). The
bundled `stdin_*` files contain two random alphanumeric strings of the
indicated length; `stdin_10000` is the largest.

## Generating test inputs

```sh
./generate_sequences.sh <length> <filename>
```

writes two random alphanumeric strings of `<length>` characters (one per line)
into `<filename>`. For example:

```sh
./generate_sequences.sh 5000 my_input
./plcs.tile 8 < my_input
```

## Output

Each program prints the LCS length of the two input strings, for example:

```
227
```

All four implementations produce identical results on the bundled inputs.

## Notes

- The DP table is statically allocated as `int dp[MAXN][MAXN]`, which requires
  roughly 400 MB of memory.
- The number of threads is capped at 64 (`NTHREAD` in `thread.h`); the tiled
  implementation uses at most `M` threads, where `M` is the length of the
  second input string.

## Citation

If you use this code in your research, please cite the paper:

```bibtex
@inproceedings{sun2025tiling,
  title     = {Tiling Dynamic Programming Computations to Maximize Parallelism},
  author    = {Sun, YanTao and Wang, Miaomiao and Guan, Jian and Li, Lin and Wang, Li and Liu, Guanjun},
  booktitle = {2025 IEEE International Conference on High Performance Computing and Communications (HPCC)},
  year      = {2025},
  pages     = {456--463},
  publisher = {IEEE},
  doi       = {10.1109/HPCC67675.2025.00077}
}
```

## Third-Party Code

`thread.h` and `thread-sync.h` are adapted from the lab framework of the
Nanjing University Operating Systems course by Prof. Yanyan Jiang (jyy),
<https://jyywiki.cn/OS/>. The upstream course repository
(<https://github.com/NJU-ProjectN/os-workbench>) does not declare a software
license for the lab framework, and the course materials on the website are
published under the Creative Commons BY-NC 4.0 license. These two files are
included here with attribution and retain the terms of the original course
materials.

`plcs.queue.c` is adapted from
<https://github.com/Ouhznehc/NJU-OSLAB/blob/1f8895143a60d4c45ffac3cf898466bd482c61ea/plcs/plcs.c>.
The original repository is not publicly accessible, so its license terms are
unknown.

All other code is the authors' own.

## License

The authors' own code in this repository is licensed under the
[MIT License](LICENSE). `thread.h`, `thread-sync.h`, and `plcs.queue.c` are
third-party code — see [Third-Party Code](#third-party-code).

