/**
  @file shmembench.c
 */

#include "shmembench.h"

/*******************************************************************
  @brief Run the selected benchmark
  @param benchtype Either bw, bibw, or latency
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 *******************************************************************/
void run_benchmark(char *benchmark, char *benchtype,
                   int min_msg_size, int max_msg_size) {
  /* shmem_put */
  if (strcmp(benchmark, "shmem_put") == 0) {
    if (strcmp(benchtype, "bw") == 0) {
      // benchmark_shmem_put_bw(min_msg_size, max_msg_size);
    } 
    else if (strcmp(benchtype, "bibw") == 0) {
      // benchmark_shmem_put_bibw(min_msg_size, max_msg_size);
    }
    else if (strcmp(benchtype, "latency") == 0) {
      // benchmark_shmem_put_latency(min_msg_size, max_msg_size);
    }
  }
  else if (strcmp(benchmark, "shmem_p") == 0) {
    if (strcmp(benchtype, "bw") == 0) {
      // benchmark_shmem_p_bw(min_msg_size, max_msg_size);
    } 
    else if (strcmp(benchtype, "bibw") == 0) {
      // benchmark_shmem_p_bibw(min_msg_size, max_msg_size);
    }
    else if (strcmp(benchtype, "latency") == 0) {
      // benchmark_shmem_p_latency(min_msg_size, max_msg_size);
    }
  }
}

/*******************************************************************
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the benchmark suite.
 ******************************************************************/
void display_logo() {
  printf(
"     _                   _               _   \n"
" ___| |_ _____ ___ _____| |_ ___ ___ ___| |_ \n"
"|_ -|   |     | -_|     | . | -_|   |  _|   |\n"
"|___|_|_|_|_|_|___|_|_|_|___|___|_|_|___|_|_|\n"
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
void display_header(char *shmem_name, char* shmem_version, int npes,
                    char *benchmark, char *benchtype,
                    int min_msg_size, int max_msg_size) {
  printf("\n");
  printf("==============================================\n");
  printf("===            Test Information            ===\n");
  printf("==============================================\n");
  printf("  OpenSHMEM Name:         %s\n", shmem_name);
  printf("  OpenSHMEM Version:      %s\n", shmem_version);
  printf("  Number of PEs:          %d\n", npes);
  printf("  Benchmark:              %s\n", benchmark);
  printf("  Benchmark Type:         %s\n", benchtype);
  printf("  Min Msg Size (bytes):   %d\n", min_msg_size);
  printf("  Max Msg Size (bytes):   %d\n", max_msg_size);
  printf("==============================================\n");
  printf("\n");
}
