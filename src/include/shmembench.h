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

#ifdef _cplusplus
extern "C" {
#endif

/**
  @struct options
  @brief Struct to hold runtime options
 */
typedef struct {
  bool cxx;             /**< Flag to enable C/C++ version of the tests */
  bool c11;             /**< Flag to enable C11 version of the tests */
  
  bool shmem_put;       /**< Flag to enable shmem_put test */
  bool shmem_p;         /**< Flag to enable shmem_p test */
  
  // TODO: add options of the rest of the tests and benchmark parameters

  bool help;            /**< Flag to display help message */
} options;

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
void display_header(char *shmem_name, char* shmem_version, int npes, char* benchmark);


#ifdef _cplusplus
}
#endif

#endif /* SHMEMBENCH_H */