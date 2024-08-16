/**
  @file shmembench.h
 */

#ifndef SHMEMBENCH_H
#define SHMEMBENCH_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

#include "benchmarks.h"

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define HLINE "--------------------------------------------"

/**
  @brief Run the selected benchmark
  @param benchtype Either bw, bibw, or latency
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void run_benchmark(char *benchmark, char *benchtype,
                   int min_msg_size, int max_msg_size);

/**
 @brief Displays the ASCII art logo.
 This function prints out the ASCII art logo for the test suite.
 */
void display_logo(void);

/**
  @brief Displays information about the test suite.
  This function prints out information about the OpenSHMEM library being
  tested, including the library name, version, the number of PEs
  (Processing Elements), and the benchmark that's being tested
  @param shmem_name Name of the OpenSHMEM library.
  @param shmem_version Version of the OpenSHMEM library.
  @param npes Number of PEs (Processing Elements).
  @param benchmark Selected benchmark
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 TODO: probably add min and max message sizes to this
 */
void display_header(char *shmem_name, char* shmem_version, int npes,
                    char *benchmark, char *benchtype,
                    int min_msg_size, int max_msg_size);

#endif /* SHMEMBENCH_H */
