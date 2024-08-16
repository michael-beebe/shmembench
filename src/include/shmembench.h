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
#include <sys/time.h>

#include "benchmarks.h"

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define HLINE "--------------------------------------------"

#define NTIMES 100 // TODO: take this in at runtime instead

/**
  @brief Run the selected benchmark
  @param benchtype Either bw, bibw, or latency
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 */
void run_benchmark(char *benchmark, char *benchtype,
                   int min_msg_size, int max_msg_size);

/**
  @brief Calculate bandwidth based on message size and time
  @param size Message size for the test in bytes
  @param time Time taken for the operation in microseconds
  @return The calculated bandwidth in MB/s
 */
double calculate_bw(int size, double time);

/**
  @brief Calculate bidirectional bandwidth based on message size and time
  @param size Message size for the test in bytes
  @param time Time taken for the operation in microseconds
  @return The calculated bidirectional bandwidth in MB/s
  FIXME: this doesn't seem right
 */
double calculate_bibw(int size, double time);

/**
  @brief Calculate latency based on time
  @param time Time taken for the operation in microseconds
  @return The calculated latency in microseconds
  */
double calculate_latency(double time);

/**
  @brief Display benchmark results
  @param times Time array for benchmark timings
  @param msg_size Message size array
  @param result BW/Latency array
  @param benchtype bw, bibw, or latency
  @param num_sizes Number of rows in the table
  */
void display_results(double *times, int *msg_size, double *results,
                     const char *benchtype, int num_sizes);

/**
  @brief Get current time
  @return Current time
 */
double mysecond(void);

/**
  @brief Check if there are exactly 2 PEs
  @return True or false
 */
bool check_if_exactly_2_pes(void);

/**
  @brief Check if there are at least 2 PEs
  @return True or false
 */
bool check_if_atleast_2_pes(void);

/**
  @brief Setup the benchmark by calculating the number of message sizes and allocating arrays
  @param min_msg_size Minimum message size for the benchmark in bytes
  @param max_msg_size Maximum message size for the benchmark in bytes
  @param num_sizes Pointer to store the number of message sizes
  @param msg_sizes Pointer to an array to store the message sizes
  @param times Pointer to an array to store the timing results
  @param results Pointer to an array to store the results (e.g., bandwidth or latency)
*/
bool setup_bench(int min_msg_size, int max_msg_size, int *num_sizes,
                 int **msg_sizes, double **times, double **results);

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
 */
void display_header(char *shmem_name, char* shmem_version, int npes,
                    char *benchmark, char *benchtype,
                    int min_msg_size, int max_msg_size);


#endif /* SHMEMBENCH_H */
