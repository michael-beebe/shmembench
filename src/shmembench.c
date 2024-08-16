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

/******************************************************************
  @param times Time array for benchmark timings
  @param msg_size Message size array
  @param result BW/Latency array
 ******************************************************************/
void display_results(double *times, int *msg_size, double *result) {
  // TODO: write print_results();
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
