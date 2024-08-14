/**
  @file shmembench.cpp
 */

#include "shmembench.h"

/******************************************************************
 * @brief Parses command-line options.
 *
 * This function parses the command-line arguments and sets the corresponding
 * options in the provided options structure.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @param opts Reference to the test options structure.
 * @return True if parsing is successful, false otherwise.
 ******************************************************************/
bool parse_opts(int argc, char *argv[], options *opts) {
  /* Initialize all options to false by default */
  memset(opts, 0, sizeof(*opts));

  /* Define runtime options */
  static struct option long_options[] = {
    {"cxx", no_argument, 0, 0},
    {"c11", no_argument, 0, 0},
    {"shmem_put", no_argument, 0, 0},
    {"shmem_p", no_argument, 0, 0},
    // TODO: Add more long options here as needed
    {0, 0, 0, 0} /* Terminator */
  };

  int option_index = 0;
  int c;

  while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
    if (c == 0) { // Long option
      const char* option_name = long_options[option_index].name;
      if (strcmp(option_name, "cxx") == 0) {
        opts->cxx = true;
      }
      else if (strcmp(option_name, "c11") == 0) {
        opts->c11 = true;
      }
      else if (strcmp(option_name, "shmem_put") == 0) {
        opts->shmem_put = true;
      }
      else if (strcmp(option_name, "shmem_p") == 0) {
        opts->shmem_p = true;
      }
      // TODO: Add additional option handling here as needed
    } else {
      return false;
    }
  }

  return true;
}

/******************************************************************
 * @brief Displays usage information for the test suite.
 *
 * Prints out the usage information and available options
 ******************************************************************/
void display_help() {
  printf("\nThis program is a performance benchmark suite for "
          "OpenSHMEM Implementations\n");

  printf("\nUsage: shmemvv [options]\n");
  printf("Options:\n");
  // TODO: write the 
}

/*******************************************************************
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the benchmark suite.
 ******************************************************************/
void display_logo() {
  printf(
"░░░░░░░ ░░   ░░ ░░░    ░░░ ░░░░░░░ ░░░    ░░░ ░░░░░░  ░░░░░░░ ░░░    ░░  ░░░░░░ ░░   ░░\n"
"▒▒      ▒▒   ▒▒ ▒▒▒▒  ▒▒▒▒ ▒▒      ▒▒▒▒  ▒▒▒▒ ▒▒   ▒▒ ▒▒      ▒▒▒▒   ▒▒ ▒▒      ▒▒   ▒▒\n"
"▒▒▒▒▒▒▒ ▒▒▒▒▒▒▒ ▒▒ ▒▒▒▒ ▒▒ ▒▒▒▒▒   ▒▒ ▒▒▒▒ ▒▒ ▒▒▒▒▒▒  ▒▒▒▒▒   ▒▒ ▒▒  ▒▒ ▒▒      ▒▒▒▒▒▒▒\n" 
"     ▓▓ ▓▓   ▓▓ ▓▓  ▓▓  ▓▓ ▓▓      ▓▓  ▓▓  ▓▓ ▓▓   ▓▓ ▓▓      ▓▓  ▓▓ ▓▓ ▓▓      ▓▓   ▓▓\n" 
"███████ ██   ██ ██      ██ ███████ ██      ██ ██████  ███████ ██   ████  ██████ ██   ██\n" 
  );
}


/*******************************************************************
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
 ******************************************************************/
void display_header(char *shmem_name, char* shmem_version, int npes, char* benchmark) {
  printf(
      "\n==================================================================\n");
  printf(
      "===                   Test Information                         ===\n");
  printf(
      "==================================================================\n");
  printf("  OpenSHMEM Name:       %s\n", shmem_name);
  printf("  OpenSHMEM Version:    %s\n", shmem_version);
  printf("  Number of PEs:        %d\n", npes);
  printf("  Benchmark:            %s\n", benchmark);
}
