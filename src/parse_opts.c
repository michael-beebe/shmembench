/**
  @file parse_opts.c
  @details Parse the runtime options from the command line
  @author Michael Beebe (Texas Tech University)
 */

#include "parse_opts.h"

/******************************************************************
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
 ******************************************************************/
bool parse_opts(int argc, char *argv[], options *opts, char **benchmark,
                char **benchtype, int *min_msg_size, int *max_msg_size,
                int *ntimes, int *stride) {
  /* Initialize all options to default values */
  memset(opts, 0, sizeof(*opts));

  /* Set default values for min_msg_size, max_msg_size, ntimes, and stride */
  *min_msg_size = BENCHMARK_DATATYPE_SIZE;
  *max_msg_size = BENCHMARK_DATATYPE_SIZE * 128;
  *ntimes = 10;
  *stride = 10;

  /* Define runtime options */
  static struct option long_options[] = {
      {"bench", required_argument, 0, 0},
      {"benchtype", required_argument, 0, 0},
      {"min", required_argument, 0, 0},
      {"max", required_argument, 0, 0},
      {"ntimes", required_argument, 0, 0},
      {"stride", required_argument, 0, 0},
      {"help", no_argument, 0, 0},
      {0, 0, 0, 0} /* Terminator */
  };

  /* Change what errors getopt_long will print */
  opterr = 1;

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
    if (c == 0) {
      const char *option_name = long_options[option_index].name;

      if (strcmp(option_name, "bench") == 0) {
        opts->bench = strdup(optarg);
        *benchmark = opts->bench;
      } else if (strcmp(option_name, "benchtype") == 0) {
        if (strcmp(optarg, "bw") == 0 || strcmp(optarg, "bibw") == 0 ||
            strcmp(optarg, "latency") == 0) {
          opts->benchtype = strdup(optarg);
          *benchtype = opts->benchtype;
        } else {
          if (shmem_my_pe() == 0) {
            fprintf(stderr,
                    "Invalid benchtype specified: %s. "
                    "Must be 'bw', 'bibw', or 'latency'.\n",
                    optarg);
          }
          return false;
        }
      } else if (strcmp(option_name, "min") == 0) {
        if (*benchmark && (strcmp(*benchmark, "shmem_barrier_all") != 0)) {
          opts->min_msg_size = atoi(optarg);
          if (opts->min_msg_size <= 0) {
            opts->min_msg_size = 1; /* Set a default if not provided */
          }
          *min_msg_size = opts->min_msg_size;
        }
      } else if (strcmp(option_name, "max") == 0) {
        if (*benchmark && (strcmp(*benchmark, "shmem_barrier_all") != 0)) {
          opts->max_msg_size = atoi(optarg);
          if (opts->max_msg_size <= 0 ||
              opts->max_msg_size < opts->min_msg_size) {
            opts->max_msg_size =
                *min_msg_size * 128; /* Set a default max if invalid */
          }
          *max_msg_size = opts->max_msg_size;
        }
      } else if (strcmp(option_name, "ntimes") == 0) {
        opts->ntimes = atoi(optarg);
        if (opts->ntimes <= 0) {
          opts->ntimes = 10; /* Default to 10 if not provided */
        }
        *ntimes = opts->ntimes;
      } else if (strcmp(option_name, "stride") == 0) {
        if (*benchmark && (strcmp(*benchmark, "shmem_iput") == 0 ||
                           strcmp(*benchmark, "shmem_iget") == 0)) {
          opts->stride = atoi(optarg);
          if (opts->stride <= 0) {
            opts->stride = 10; /* Default to 10 if not provided */
          }
          *stride = opts->stride;
        }
      } else if (strcmp(option_name, "help") == 0) {
        opts->help = true;
      } else {
        return false; /* No valid options */
      }
    } else {
      return false;
    }
  }

  return true;
}

/******************************************************************
  @brief Displays usage information for the test suite.
  Prints out the usage information and available options.
 ******************************************************************/
// clang-format off
void display_help() {
  printf("\n\nThis program is a performance benchmark suite for "
         "OpenSHMEM Implementations (v1.4 and v1.5)\n");

  printf("\nUsage:  oshrun -np <num PEs> shmembench --bench <routine> "
         "--benchtype <type> [options]\n");

  printf("\nRequired Options:\n");
  printf("  --bench <routine>      Specify the routine to benchmark\n");
  printf("                          Available options:\n");
  printf("                            shmem_put\n");
  printf("                            shmem_iput\n");
  printf("                            shmem_get\n");
  printf("                            shmem_iget\n");
  printf("                            shmem_put_nbi\n");
  printf("                            shmem_get_nbi\n");
  printf("\n");
  printf("                            shmem_alltoall\n");
  printf("                            shmem_alltoalls\n");
  printf("                            shmem_broadcast\n");
  printf("                            shmem_collect\n");
  printf("                            shmem_fcollect\n");
  printf("                            shmem_barrier_all\n");
  printf("\n");
  printf("                            shmem_atomic_add\n");
  printf("                            shmem_atomic_compare_swap\n");
  printf("                            shmem_atomic_fetch\n");
  printf("                            shmem_atomic_fetch_nbi\n");
  printf("                            shmem_atomic_inc\n");
  printf("                            shmem_atomic_set\n");
  printf("                            shmem_atomic_swap\n");
  printf("\n");
  printf(
      "  --benchtype <type>     Set the benchmark type (bw, bibw, latency)\n");
  printf("                           Note: Pt2pt RMA benchmarks support bw, "
         "bibw and latency.\n");
  printf("                           Note: Collectives benchmarks only support "
         "bw and latency.\n");
  printf("                           Note: Atomic benchmarks only support "
         "latency.\n");
  printf("\nOptional Parameters:\n");
  printf(
      "  --min <size>           Minimum message size in bytes (default: %zu)\n", BENCHMARK_DATATYPE_SIZE);
  printf("                            Note: Will be automatically increased to \n");
  printf("                            sizeof(%s) (%zu bytes) if specified value is smaller.\n", BENCHMARK_DATATYPE_NAME, BENCHMARK_DATATYPE_SIZE);
  printf("                            Note: Not applicable for atomic "
         "benchmarks.\n");
  printf("                            Note: Not applicable for "
         "shmem_barrier_all benchmark.\n");
  printf("\n");
  printf("  --max <size>           Maximum message size in bytes (default: "
         "1048576)\n");
  printf("                            Note: Not applicable for atomic "
         "benchmarks.\n");
  printf("                            Note: Not applicable for "
         "shmem_barrier_all benchmark.\n");
  printf("\n");
  printf("  --ntimes <count>       Number of repetitions.\n");
  printf("                         Average among them is reported (default: "
         "10)\n");
  printf("\n");
  printf("  --stride <value>       Stride value for strided operations, only "
         "used by\n");
  printf(
      "                         the shmem_iput and shmem_iget (default: 10)\n");
  printf("\n");
  printf("  --help                 Display this help message\n");

  printf("\nExample Usage:\n");
  printf("   oshrun -np 2 shmembench --bench shmem_put --benchtype bw --min "
         "128 --max 1024 --ntimes 20\n");
  printf("   oshrun -np 2 shmembench --bench shmem_iget --benchtype bw --min "
         "128 --max 1024 --ntimes 20 --stride 20\n");
  printf("   oshrun -np 64 shmembench --bench shmem_broadcast --benchtype bw "
         "--min 128 --max 1025 --ntimes 20\n");
  printf("   oshrun -np 64 shmembench --bench shmem_barrier_all --benchtype "
         "latency --ntimes 100\n");
  printf("   oshrun -np 6 shmembench --bench shmem_atomic_add --benchtype "
         "latency --ntimes 100\n");
  printf("\n");
}
// clang-format on
