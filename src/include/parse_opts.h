/** 
  @file parse_opts.h
*/

#ifndef PARSE_OPTS_H
#define PARSE_OPTS_H

#include <shmem.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

/**
  @struct options
  @brief Struct to hold runtime options
 */
typedef struct {
  /* RMA options */
  bool shmem_put; bool shmem_iput;
  bool shmem_get; bool shmem_iget;
  bool shmem_put_nbi; bool shmem_get_nbi;
  /* Collective options */
  bool shmem_alltoall; bool shmem_alltoalls;
  bool shmem_broadcast;
  bool shmem_collect; bool shmem_fcollect;

  /* Option for which test to run */
  char *benchtype;
  
  /* Benchmark parameter options */
  int min_msg_size;
  int max_msg_size;

  /* Option to print help */
  bool help;
} options;

/**
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @param benchmark** Routine to be tested
  @param benchtype Either bw, bibw, or latency
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], options *opts,
                char **benchmark, char **benchtype,
                int *min_msg_size, int *max_msg_size);

/**
  @brief Displays usage information.
 */
void display_help(void);

#endif /* PARSE_OPTS_H */
