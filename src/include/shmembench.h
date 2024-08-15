/**
  * @file shmembench.h
 */

#ifndef SHMEMBENCH_H
#define SHMEMBENCH_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define HLINE "--------------------------------------------"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct options
  @brief Struct to hold runtime options
 */
typedef struct {
  /* RMA options */
  bool shmem_put; bool shmem_p; bool shmem_iput;
  bool shmem_get; bool shmem_g; bool shmem_iget;
  bool shmem_put_nbi; bool shmem_get_nbi;
  /* Collective options */
  bool shmem_alltoall; bool shmem_alltoalls;
  bool shmem_broadcast;
  bool shmem_collect; bool shmem_fcollect;
  bool shmem_min_reduce; bool shmem_max_reduce;

  /* Option for which test to run */
  char *benchtype;
  
  /* Benchmark parameter options */
  int min_msg_size; /**< Minimum message size for benchmarks */
  int max_msg_size; /**< Maximum message size for benchmarks */
  int stride;

  /* Option to print help */
  bool help;
} options;

/**
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], options *opts,
                char **benchmark, char **benchtype);

/**
  @brief Displays usage information.
 */
void display_help(void);

/**
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the test suite.
 */
void display_logo(void);

/**
 * @brief Displays information about the test suite.
 *
 * This function prints out information about the OpenSHMEM library being
 * tested, including the library name, version, the number of PEs
 * (Processing Elements), and the benchmark that's being tested
 *
 * @param shmem_name Name of the OpenSHMEM library.
 * @param shmem_version Version of the OpenSHMEM library.
 * @param npes Number of PEs (Processing Elements).
 * @param benchmark Selected benchmark
 */
void display_header(char *shmem_name, char* shmem_version, int npes,
                    char *benchmark, char *benchtype);

#ifdef __cplusplus
}
#endif

#endif /* SHMEMBENCH_H */
