/**
  @file parse_opts.h
*/

#ifndef PARSE_OPTS_H
#define PARSE_OPTS_H

#include <getopt.h>
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
  @struct options
  @brief Struct to hold runtime options
 */
typedef struct {
  /* Benchmark options */
  char *bench;
  char *benchtype;

  /* Benchmark parameter options */
  int min_msg_size;
  int max_msg_size;
  int ntimes;
  int stride;

  /* Option to print help */
  bool help;
} options;

/**
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @param benchmark Routine to be tested (e.g., shmem_put, shmem_get)
  @param benchtype Type of benchmark to run (bw, bibw, or latency)
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
  @param stride Stride value to use for the benchmark (only used if applicable)
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], options *opts, char **benchmark,
                char **benchtype, int *min_msg_size, int *max_msg_size,
                int *ntimes, int *stride);

/**
  @brief Displays usage information.
 */
void display_help(void);

#endif /* PARSE_OPTS_H */
