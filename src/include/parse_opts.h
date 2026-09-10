/**
  @file parse_opts.h
  @author Michael Beebe (Texas Tech University)
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
  int warmups;
  int stride;
  int window;
  int block_size;
  int mask;
  bool ready;
  char *target;

  /* Option to print help */
  bool help;
} options;

/* Define default number of warmup iterations */
#define DEFAULT_WARMUP_ITERATIONS 20

/**
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], options *opts);

/**
  @brief Displays usage information.
 */
void display_help(void);

#endif /* PARSE_OPTS_H */
