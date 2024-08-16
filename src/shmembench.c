/**
  @file shmembench.c
 */

#include "shmembench.h"

/* Function pointer type for benchmarks */
typedef void (*benchmark_func_t)(int min_msg_size, int max_msg_size);

/* Mapping of benchmarks and their types to functions */
typedef struct {
  const char *benchmark;
  const char *benchtype;
  benchmark_func_t func;
} benchmark_entry_t;

/* Dispatch table for benchmarks */
benchmark_entry_t benchmark_table[] = {
  {"shmem_put", "bw", bench_shmem_put_bw},
  {"shmem_put", "bibw", bench_shmem_put_bibw},
  {"shmem_put", "latency", bench_shmem_put_latency},
  // TODO: Add other benchmark-function mappings here
};

/*******************************************************************
  @brief Run the selected benchmark
  @param benchtype Either bw, bibw, or latency
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
 *******************************************************************/
void run_benchmark(char *benchmark, char *benchtype,
                   int min_msg_size, int max_msg_size)
{
  for (int i = 0; i < sizeof(benchmark_table) / sizeof(benchmark_entry_t); i++) {
    if (strcmp(benchmark, benchmark_table[i].benchmark) == 0 &&
        strcmp(benchtype, benchmark_table[i].benchtype) == 0) {
        benchmark_table[i].func(min_msg_size, max_msg_size);
        return;
    }
  }
  if (shmem_my_pe() == 0) {
    fprintf(stderr, "Error: Benchmark or benchtype not found.\n");
  }
}

/*************************************************************
  @brief Calculate bandwidth based on message size and time
  @param size Message size for the test in bytes
  @param time Time taken for the operation in microseconds
  @return The calculated bandwidth in MB/s
 *************************************************************/
double calculate_bw(int size, double time) {
  return (size / (1024.0 * 1024.0)) / (time / 1e6);
}

/*************************************************************
  @brief Calculate bidirectional bandwidth based on message size and time
  @param size Message size for the test in bytes
  @param time Time taken for the operation in microseconds
  @return The calculated bidirectional bandwidth in MB/s
 *************************************************************/
double calculate_bibw(int size, double time) {
  return 2 * (size / (1024.0 * 1024.0)) / (time / 1e6);
}

/*************************************************************
  @brief Calculate latency based on time
  @param time Time taken for the operation in microseconds
  @return The calculated latency in microseconds
 *************************************************************/
double calculate_latency(double time) {
  return time;
}

/******************************************************************
  @param times Time array for benchmark timings
  @param msg_size Message size array
  @param result BW/Latency array
  @param benchtype bw, bibw, or latency
  @param num_sizes Number of rows in the table
 ******************************************************************/
void display_results(double *times, int *msg_size, double *results,
                     const char *benchtype, int num_sizes) {
  printf("==============================================\n");
  printf("===          Benchmark Results             ===\n");
  printf("==============================================\n");

  /* Print table headers based on the benchtype */
  if (strcmp(benchtype, "bw") == 0 || strcmp(benchtype, "bibw") == 0) {
    printf("%-16s %-16s %-14s\n", "Message Size", "Avg Time (us)", "Avg MB/s");
  }
  else if (strcmp(benchtype, "latency") == 0) {
    printf("%-16s %-16s %-14s\n", "Message Size", "Avg Time (us)", "Latency (us)");
  }

  /* Print each row of the table */
  for (int i = 0; i < num_sizes; i++) {
    printf("%-16d %-16.2f %-14.2f\n", msg_size[i], times[i], results[i]);
  }

  // printf("==============================================\n");
  printf("\n");
}

/******************************************************************
  @return Current time
 ******************************************************************/
double mysecond() {
  struct timeval tp;
  struct timezone tzp;
  int i = gettimeofday(&tp, &tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
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
  printf("===          Test Information              ===\n");
  printf("==============================================\n");
  printf("  OpenSHMEM Name:         %s\n", shmem_name);
  printf("  OpenSHMEM Version:      %s\n", shmem_version);
  printf("  Number of PEs:          %d\n", npes);
  printf("  Benchmark:              %s\n", benchmark);
  printf("  Benchmark Type:         %s\n", benchtype);
  printf("  Min Msg Size (bytes):   %d\n", min_msg_size);
  printf("  Max Msg Size (bytes):   %d\n", max_msg_size);
  printf("  NTIMES:                 %d\n", NTIMES);
}
