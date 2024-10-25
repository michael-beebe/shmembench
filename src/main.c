/**
  @file main.c
  @brief main driver file
  @author Michael Beebe (Texas Tech University)
 */

#include "parse_opts.h"
#include "shmembench.h"

/**
  @brief Main function for running the test suite.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
*/
int main(int argc, char *argv[]) {
  /******************************************************************
    Start the OpenSHMEM instance
  ******************************************************************/
#if defined(USE_14) || defined(USE_15)
  shmem_init();
#else
#error "Neither USE_14 nor USE_15 is defined."
#endif

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  /******************************************************************
    Display logo
  ******************************************************************/
  if (mype == 0) {
    display_logo();
  }

  /******************************************************************
    Get the OpenSHMEM impl version
  ******************************************************************/
#if defined(USE_14) || defined(USE_15)
  int major, minor;
  shmem_info_get_version(&major, &minor);
#endif

  char *version = (char *)malloc(16 * sizeof(char));
  if (version != NULL) {
    snprintf(version, 16, "%d.%d", major, minor);
#if defined(USE_14)
    if (!(strstr(version, "1.4") != NULL)) {
      if (mype == 0) {
        fprintf(stderr, RED_COLOR "\nERROR: " RESET_COLOR
                                  "OpenSHMEM v1.4 is required!\n\n");
      }
      free(version);
      shmem_global_exit(EXIT_FAILURE);
    }
#elif defined(USE_15)
    if (!(strstr(version, "1.5") != NULL)) {
      if (mype == 0) {
        fprintf(stderr, RED_COLOR "\nERROR: " RESET_COLOR
                                  "OpenSHMEM v1.5 is required!\n\n");
      }
      free(version);
      shmem_global_exit(EXIT_FAILURE);
    }
#endif
  }

  /******************************************************************
    Get the OpenSHMEM impl name
  ******************************************************************/
#if defined(USE_14) || defined(USE_15)
  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name != NULL) {
    shmem_info_get_name(name);
  }
#endif

  /******************************************************************
    Parse options
  ******************************************************************/
  options opts;
  char *benchmark = (char *)malloc(100 * sizeof(char));
  char *benchtype = (char *)malloc(100 * sizeof(char));
  int min_msg_size, max_msg_size;
  int ntimes, stride;
  shmem_barrier_all();
  if (!parse_opts(argc, argv, &opts, &benchmark, &benchtype, &min_msg_size,
                  &max_msg_size, &ntimes, &stride)) {
    if (mype == 0) {
      display_help();
    }
    shmem_global_exit(EXIT_FAILURE);
  }

  /******************************************************************
    Display help if requested
  ******************************************************************/
  shmem_barrier_all();
  if (opts.help) {
    if (mype == 0) {
      display_help();
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  /******************************************************************
    Display header
  ******************************************************************/
  shmem_barrier_all();
  if (mype == 0) {
    display_header(name, version, npes, benchmark, benchtype, min_msg_size,
                   max_msg_size, ntimes, stride);
  }

  /******************************************************************
    Run benchmarks
  ******************************************************************/
  shmem_barrier_all();
  run_benchmark(benchmark, benchtype, min_msg_size, max_msg_size, ntimes,
                stride);

  /******************************************************************
    Finalize the program
  ******************************************************************/
  free(version);
  free(name);
  free(benchmark);
  free(benchtype);

  shmem_finalize();
  return EXIT_SUCCESS;
}
