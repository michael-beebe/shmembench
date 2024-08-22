/**
  @file shmembench.c
  @author Michael Beebe (Texas Tech University)
 */

#include "shmembench.h"

/* Function pointer type for benchmarks without stride */
typedef void (*benchmark_func_t)(int min_msg_size, int max_msg_size,
                                 int ntimes);

/* Function pointer type for benchmarks with stride */
typedef void (*benchmark_func_with_stride_t)(int min_msg_size, int max_msg_size,
                                             int ntimes, int stride);

/* Function pointer type for benchmarks without message size parameters (like barriers and atomics) */
typedef void (*benchmark_func_no_size_t)(int ntimes);

/* Mapping of benchmarks and their types to functions */
typedef struct {
  const char *benchmark;
  const char *benchtype;
  benchmark_func_t func;
  benchmark_func_with_stride_t func_with_stride;
  benchmark_func_no_size_t func_no_size;
  bool uses_stride;
} benchmark_entry_t;

/* Dispatch table for benchmarks */
benchmark_entry_t benchmark_table[] = {
  {"shmem_put", "bw", bench_shmem_put_bw, NULL, NULL, false},
  {"shmem_put", "bibw", bench_shmem_put_bibw, NULL, NULL, false},
  {"shmem_put", "latency", bench_shmem_put_latency, NULL, NULL, false},

  {"shmem_get", "bw", bench_shmem_get_bw, NULL, NULL, false},
  {"shmem_get", "bibw", bench_shmem_get_bibw, NULL, NULL, false},
  {"shmem_get", "latency", bench_shmem_get_latency, NULL, NULL, false},

  {"shmem_iput", "bw", NULL, bench_shmem_iput_bw, NULL, true},
  {"shmem_iput", "bibw", NULL, bench_shmem_iput_bibw, NULL, true},
  {"shmem_iput", "latency", NULL, bench_shmem_iput_latency, NULL, true},

  {"shmem_iget", "bw", NULL, bench_shmem_iget_bw, NULL, true},
  {"shmem_iget", "bibw", NULL, bench_shmem_iget_bibw, NULL, true},
  {"shmem_iget", "latency", NULL, bench_shmem_iget_latency, NULL, true},

  {"shmem_put_nbi", "bw", bench_shmem_put_nbi_bw, NULL, NULL, false},
  {"shmem_put_nbi", "bibw", bench_shmem_put_nbi_bibw, NULL, NULL, false},
  {"shmem_put_nbi", "latency", bench_shmem_put_nbi_latency, NULL, NULL, false},

  {"shmem_get_nbi", "bw", bench_shmem_get_nbi_bw, NULL, NULL, false},
  {"shmem_get_nbi", "bibw", bench_shmem_get_nbi_bibw, NULL, NULL, false},
  {"shmem_get_nbi", "latency", bench_shmem_get_nbi_latency, NULL, NULL, false},

  {"shmem_alltoall", "bw", bench_shmem_alltoall_bw, NULL, NULL, false},
  {"shmem_alltoall", "latency", bench_shmem_alltoall_latency, NULL, NULL, false},

  {"shmem_alltoalls", "bw", bench_shmem_alltoalls_bw, NULL, NULL, false},
  {"shmem_alltoalls", "latency", bench_shmem_alltoalls_latency, NULL, NULL, false},

  {"shmem_broadcast", "bw", bench_shmem_broadcast_bw, NULL, NULL, false},
  {"shmem_broadcast", "latency", bench_shmem_broadcast_latency, NULL, NULL, false},
    
  {"shmem_collect", "bw", bench_shmem_collect_bw, NULL, NULL, false},
  {"shmem_collect", "latency", bench_shmem_collect_latency, NULL, NULL, false},
    
  {"shmem_fcollect", "bw", bench_shmem_fcollect_bw, NULL, NULL, false},
  {"shmem_fcollect", "latency", bench_shmem_fcollect_latency, NULL, NULL, false},

  {"shmem_barrier_all", "latency", NULL, NULL, bench_shmem_barrier_all_latency, false},

  {"shmem_atomic_add", "latency", NULL, NULL, bench_shmem_atomic_add_latency, false},
  {"shmem_atomic_compare_swap", "latency", NULL, NULL, bench_shmem_atomic_compare_swap_latency, false},
  //{"shmem_atomic_fetch", "latency", bench_shmem_atomic_fetch_latency, NULL, NULL, false},
  //{"shmem_atomic_fetch_nbi", "latency", bench_shmem_atomic_fetch_nbi_latency, NULL, NULL, false},
  //{"shmem_atomic_inc", "latency", bench_shmem_atomic_inc_latency, NULL, NULL, false},
  //{"shmem_atomic_set", "latency", bench_shmem_atomic_set_latency, NULL, NULL, false},
  //{"shmem_atomic_swap", "latency", bench_shmem_atomic_swap_latency, NULL, NULL, false},
};


/*******************************************************************
  @brief Run the selected benchmark
  @param benchmark The benchmark to be run (e.g., "shmem_put", "shmem_get")
  @param benchtype The type of benchmark to run, either "bw", "bibw", or
 "latency"
  @param min_msg_size Minimum message size for test in bytes
  @param max_msg_size Maximum message size for test in bytes
  @param ntimes Number of times the benchmark should run
  @param stride Stride value to use for the benchmark (only used if applicable)
 *******************************************************************/
void run_benchmark(char *benchmark, char *benchtype, int min_msg_size,
                   int max_msg_size, int ntimes, int stride) {
  for (int i = 0; i < sizeof(benchmark_table) / sizeof(benchmark_entry_t); i++) {
    if (strcmp(benchmark, benchmark_table[i].benchmark) == 0 &&
        strcmp(benchtype, benchmark_table[i].benchtype) == 0) {
      if (benchmark_table[i].uses_stride) {
        benchmark_table[i].func_with_stride(min_msg_size, max_msg_size, ntimes, stride);
      } else if (benchmark_table[i].func != NULL) {
        benchmark_table[i].func(min_msg_size, max_msg_size, ntimes);
      } else if (benchmark_table[i].func_no_size != NULL) {
        benchmark_table[i].func_no_size(ntimes);
      }
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

// /*************************************************************
//   @brief Calculate collective bandwidth based on message size,
//     time, and number of PEs
//   @param size Message size for the test in bytes
//   @param time Time taken for the operation in microseconds
//   @param npes Number of PEs involved in the collective operation
//   @return The calculated collective bandwidth in MB/s
//  *************************************************************/
// double calculate_collective_bw(int size, double time, int npes) {
//   return (size * npes / (1024.0 * 1024.0)) / (time / 1e6);
// }

/*************************************************************
  @brief Calculate bidirectional bandwidth based on message size and time
  @param size Message size for the test in bytes
  @param time Time taken for the operation in microseconds
  @return The calculated bidirectional bandwidth in MB/s
 *************************************************************/
double calculate_bibw(int size, double time) {
  return (2 * size / (1024.0 * 1024.0)) / (time / 1e6);
}

/*************************************************************
  @brief Calculate latency based on time
  @param time Time taken for the operation in microseconds
  @return The calculated latency in microseconds
 *************************************************************/
double calculate_latency(double time) { return time; }

/******************************************************************
  @param times Time array for benchmark timings
  @param msg_size Message size array
  @param results BW/Latency array
  @param benchtype Either "bw", "bibw", or "latency"
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
  } else if (strcmp(benchtype, "latency") == 0) {
    printf("%-16s %-16s\n", "Message Size", "Latency (us)");
  }

  /* Print each row of the table */
  for (int i = 0; i < num_sizes; i++) {
    if (strcmp(benchtype, "latency") == 0) {
      printf("%-16d %-16.2f\n", msg_size[i], results[i]);
    } else {
      printf("%-16d %-16.2f %-14.2f\n", msg_size[i], times[i], results[i]);
    }
  }

  printf("\n");
}

/******************************************************************
  @brief Display results for atomic latency benchmarks
  @param benchmark The name of the benchmark (e.g., "shmem_atomic_add")
  @param total_time The total time taken for the operations in microseconds
  @param ntimes The number of times the operation was performed
 ******************************************************************/
void display_atomic_latency_results(const char *benchmark, double total_time, int ntimes) {
  double avg_time = total_time / ntimes;

  printf("==============================================\n");
  printf("%s Latency        \n", benchmark);
  printf("==============================================\n");
  printf("Avg Time per Operation (us):   %.2f\n", avg_time);
  printf("Total Time for %d Operations (us): %.2f\n", ntimes, total_time);
  printf("==============================================\n");
}

/******************************************************************
  @return Current time in seconds
 ******************************************************************/
double mysecond(void) {
  struct timeval tp;
  struct timezone tzp;
  int i = gettimeofday(&tp, &tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

/******************************************************************
  @brief Check if there are exactly 2 PEs
  @return True if there are exactly 2 PEs, false otherwise
 ******************************************************************/
bool check_if_exactly_2_pes(void) {
  if (shmem_n_pes() != 2) {
    if (shmem_my_pe() == 0) {
      printf(RED_COLOR "\nERROR: " RESET_COLOR "This test requires"
                       " exactly 2 PEs!\n\n");
    }
    return false;
  }
  return true;
}

/******************************************************************
  @brief Check if there are at least 2 PEs
  @return True if there are at least 2 PEs, false otherwise
 ******************************************************************/
bool check_if_atleast_2_pes(void) {
  if (!(shmem_n_pes() >= 2)) {
    if (shmem_my_pe() == 0) {
      printf(RED_COLOR "\nERROR: " RESET_COLOR "This test requires"
                       " at least 2 PEs!\n\n");
    }
    return false;
  }
  return true;
}

/******************************************************************
  @brief Setup the benchmark by calculating the number of message
         sizes and allocating arrays
  @param min_msg_size Minimum message size for the benchmark in bytes
  @param max_msg_size Maximum message size for the benchmark in bytes
  @param num_sizes Pointer to store the number of message sizes
  @param msg_sizes Pointer to an array to store the message sizes
  @param times Pointer to an array to store the timing results
  @param results Pointer to an array to store the results
                (e.g., bandwidth or latency)
  @return True if setup was successful, false otherwise
 ******************************************************************/
bool setup_bench(int min_msg_size, int max_msg_size, int *num_sizes,
                 int **msg_sizes, double **times, double **results) {
  /* Determine the number of message sizes */
  *num_sizes = 0;
  for (int size = min_msg_size; size <= max_msg_size; size *= 2) {
    (*num_sizes)++;
  }

  /* Allocate memory for arrays */
  *msg_sizes = (int *)malloc(*num_sizes * sizeof(int));
  *times = (double *)malloc(*num_sizes * sizeof(double));
  *results = (double *)malloc(*num_sizes * sizeof(double));

  if (msg_sizes == NULL || times == NULL || results == NULL ||
      !(num_sizes > 0)) {
    printf(RED_COLOR "\nERROR: " RESET_COLOR "Unable to set up test!\n\n");
    return false;
  }

  return true;
}

/*******************************************************************
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the benchmark suite.
 ******************************************************************/
void display_logo() {
  printf("     _                   _               _   \n"
         " ___| |_ _____ ___ _____| |_ ___ ___ ___| |_ \n"
         "|_ -|   |     | -_|     | . | -_|   |  _|   |\n"
         "|___|_|_|_|_|_|___|_|_|_|___|___|_|_|___|_|_|\n");
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
 * @param benchtype The type of benchmark to run, either "bw", "bibw", or
 *"latency"
 * @param min_msg_size Minimum message size for test in bytes
 * @param max_msg_size Maximum message size for test in bytes
 * @param ntimes Number of times the benchmark should run
 * @param stride Stride value to use for the benchmark (only applicable to
 *certain benchmarks)
 ******************************************************************/
void display_header(char *shmem_name, char *shmem_version, int npes,
                    char *benchmark, char *benchtype, int min_msg_size,
                    int max_msg_size, int ntimes, int stride) {
  printf("\n");
  printf("==============================================\n");
  printf("===          Test Information              ===\n");
  printf("==============================================\n");
  printf("  OpenSHMEM Name:         %s\n", shmem_name);
  printf("  OpenSHMEM Version:      %s\n", shmem_version);
  printf("\n");
  printf("  Number of PEs:          %d\n", npes);
  printf("  Benchmark:              %s\n", benchmark);
  printf("  Benchmark Type:         %s\n", benchtype);
  if (strstr(benchmark, "atomic") == NULL) {
    printf("  Min Msg Size (bytes):   %d\n", min_msg_size);
    printf("  Max Msg Size (bytes):   %d\n", max_msg_size);
  }
  printf("  Ntimes:                 %d\n", ntimes);
  if (strcmp(benchmark, "shmem_iput") == 0 ||
      strcmp(benchmark, "shmem_iget") == 0) {
    printf("  Stride:                 %d\n", stride);
  }
  printf("\n");
}
