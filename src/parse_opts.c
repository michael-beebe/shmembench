/**
  @file parse_opts.c @details Parse the runtime options from the command line @author Michael Beebe (Texas Tech University)
 */

#include "parse_opts.h"

#include <errno.h>
#include <limits.h>

/**
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], options *opts) {
  /* Initialize all options to default values */
  memset(opts, 0, sizeof(*opts));

  /* Set default value for Warmup Rounds */

  opts->warmups = DEFAULT_WARMUP_ITERATIONS;

  /* Set default values for min_msg_size, max_msg_size, ntimes, and stride */
  opts->min_msg_size = 8;
  opts->max_msg_size = 1024;
  opts->ntimes = 10;
  opts->stride = 10;

  /* Set bench and benchtype to NULL */
  opts->bench = NULL;
  opts->benchtype = NULL;

  /* Define runtime options */
  static struct option long_options[] = {
      {"bench", required_argument, 0, 0},
      {"benchtype", required_argument, 0, 0},
      {"min", required_argument, 0, 0},
      {"max", required_argument, 0, 0},
      {"ntimes", required_argument, 0, 0},
      {"warmup", required_argument, 0, 0},
      {"warmups", required_argument, 0, 0},
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
      } else if (strcmp(option_name, "benchtype") == 0) {
        if (strcmp(optarg, "bw") == 0 || strcmp(optarg, "bibw") == 0 ||
            strcmp(optarg, "latency") == 0) {
          opts->benchtype = strdup(optarg);
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
        if (opts->bench && (strcmp(opts->bench, "shmem_barrier_all") != 0)) {
          opts->min_msg_size = atoi(optarg);
          if (opts->min_msg_size <= 0) {
            opts->min_msg_size = 1; /* Set a default if not provided */
          }
        }
      } else if (strcmp(option_name, "max") == 0) {
        if (opts->bench && (strcmp(opts->bench, "shmem_barrier_all") != 0)) {
          opts->max_msg_size = atoi(optarg);
          if (opts->max_msg_size <= 0 ||
              opts->max_msg_size < opts->min_msg_size) {
            opts->max_msg_size =
                opts->min_msg_size * 128; /* Set a default max if invalid */
          }
        }
      } else if (strcmp(option_name, "ntimes") == 0 ||
                 strcmp(option_name, "warmup") == 0 ||
                 strcmp(option_name, "warmups") == 0) {
        bool timed = strcmp(option_name, "ntimes") == 0;
        char *end;
        errno = 0;
        long count = strtol(optarg, &end, 10);
        if (errno == ERANGE || end == optarg || *end != '\0' ||
            count < (timed ? 1 : 0) || count > INT_MAX) {
          if (shmem_my_pe() == 0) {
            fprintf(stderr, "Invalid --%s count: '%s'. Must be an integer "
                            "from %d to %d.\n",
                    option_name, optarg, timed ? 1 : 0, INT_MAX);
          }
          return false;
        }
        if (timed) {
          opts->ntimes = (int)count;
        } else {
          opts->warmups = (int)count;
        }
      } else if (strcmp(option_name, "stride") == 0) {
        if (opts->bench && (strcmp(opts->bench, "shmem_iput") == 0 ||
                           strcmp(opts->bench, "shmem_iget") == 0)) {
          opts->stride = atoi(optarg);
          if (opts->stride <= 0) {
            opts->stride = 10; /* Default to 10 if not provided */
          }
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

  /* return false if benchmark not specified */
  if (opts->bench == NULL){
    return false;
  }

  /* Set default benchtype if not provided by user */
  if (opts->benchtype == NULL || strlen(opts->bench) == 0) {
    /* Atomics: default to latency */
    if (strstr(opts->bench, "atomic") != NULL) {
      opts->benchtype = strdup("latency");
    }
    /* Barrier: default to latency */
    else if (strcmp(opts->bench, "shmem_barrier_all") == 0) {
      opts->benchtype = strdup("latency");
    }
    /* Collectives and pt2pt RMA: default to bw */
    else if (strcmp(opts->bench, "shmem_put") == 0 ||
             strcmp(opts->bench, "shmem_putmem") == 0 ||
             strcmp(opts->bench, "shmem_iput") == 0 ||
             strcmp(opts->bench, "shmem_get") == 0 ||
             strcmp(opts->bench, "shmem_getmem") == 0 ||
             strcmp(opts->bench, "shmem_iget") == 0 ||
             strcmp(opts->bench, "shmem_put_nbi") == 0 ||
             strcmp(opts->bench, "shmem_putmem_nbi") == 0 ||
             strcmp(opts->bench, "shmem_get_nbi") == 0 ||
             strcmp(opts->bench, "shmem_getmem_nbi") == 0 ||
             strcmp(opts->bench, "shmem_alltoall") == 0 ||
             strcmp(opts->bench, "shmem_alltoallmem") == 0 ||
             strcmp(opts->bench, "shmem_alltoalls") == 0 ||
             strcmp(opts->bench, "shmem_alltoallsmem") == 0 ||
             strcmp(opts->bench, "shmem_broadcast") == 0 ||
             strcmp(opts->bench, "shmem_broadcastmem") == 0 ||
             strcmp(opts->bench, "shmem_collectmem") == 0 ||
             strcmp(opts->bench, "shmem_collect") == 0 ||
             strcmp(opts->bench, "shmem_fcollect") == 0 ||
             strcmp(opts->bench, "shmem_fcollectmem") == 0) {
      opts->benchtype = strdup("bw");
    }
  }
  return true;
}

/**
  @brief Displays usage information for the test suite.
  Prints out the usage information and available options.
 */
/* clang-format off */
void display_help() {
  printf("\n\nThis program is a performance benchmark suite for "
         "OpenSHMEM Implementations (v1.4 and v1.5)\n");

  printf("\nUsage:  oshrun -np <num PEs> shmembench --bench <routine> "
         "--benchtype <type> [options]\n");

  printf("\nRequired Options:\n");
  printf("  --bench <routine>      Specify the routine to benchmark\n");
  printf("                          Available options:\n");
  printf("                            shmem_put\n");
  printf("                            shmem_putmem\n");
  printf("                            shmem_iput\n");
  printf("                            shmem_get\n");
  printf("                            shmem_getmem\n");
  printf("                            shmem_iget\n");
  printf("                            shmem_put_nbi\n");
  printf("                            shmem_putmem_nbi\n");
  printf("                            shmem_get_nbi\n");
  printf("                            shmem_getmem_nbi\n");
  printf("\n");
  printf("                            shmem_alltoall\n");
  printf("                            shmem_alltoallmem\n");
  printf("                            shmem_alltoalls\n");
  printf("                            shmem_alltoallsmem\n");
  printf("                            shmem_broadcast\n");
  printf("                            shmem_broadcastmem\n");
  printf("                            shmem_collect\n");
  printf("                            shmem_collectmem\n");
  printf("                            shmem_fcollect\n");
  printf("                            shmem_fcollectmem\n");
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
  printf("  --benchtype <type>     Set the benchmark type (bw, bibw, latency)\n");
  printf("                           Pt2pt RMA benchmarks support 'bw' (default) and 'bibw'.\n");
  printf("                             - Both 'bw' and 'bibw' benchmarks also report latency results.\n");
  printf("                           Collectives benchmarks support 'bw' (default).\n");
  printf("                             - The 'bw' benchmark also reports latency results.\n");
  printf("                           Atomic benchmarks only support 'latency' (default).\n");
  printf("\nOptional Parameters:\n");
  printf("  --min <size>           Minimum message size in bytes (default: 1)\n");
  printf("                            Note: Not applicable for atomic benchmarks.\n");
  printf("                            Note: Not applicable for shmem_barrier_all benchmark.\n");
  printf("\n");
  printf("  --max <size>           Maximum message size in bytes (default: 1024)\n");
  printf("                            Note: Not applicable for atomic benchmarks.\n");
  printf("                            Note: Not applicable for shmem_barrier_all benchmark.\n");
  printf("\n");
  printf("  --ntimes <count>       Number of repetitions.\n");
  printf("                         Average among them is reported (default: 10)\n");
  printf("\n");
  printf("  --warmup <count>       Warmup iterations before timing (default: %d).\n",
         DEFAULT_WARMUP_ITERATIONS);
  printf("                         Zero disables warmup; --warmups is an alias.\n");
  printf("\n");
  printf("  --stride <value>       Stride value for strided operations, only used by\n");
  printf("                         the shmem_iput and shmem_iget (default: 10)\n");
  printf("\n");
  printf("  --help                 Display this help message\n");

  printf("\nExample Usage:\n");
  printf("   oshrun -np 2 shmembench --bench shmem_put --benchtype bw --min 128 --max 1024 --ntimes 20\n");
  printf("   oshrun -np 2 shmembench --bench shmem_iget --benchtype bw --min 128 --max 1024 --ntimes 20 --stride 20\n");
  printf("   oshrun -np 64 shmembench --bench shmem_broadcast --benchtype bw --min 128 --max 1024 --ntimes 20\n");
  printf("   oshrun -np 64 shmembench --bench shmem_barrier_all --benchtype latency --ntimes 100\n");
  printf("   oshrun -np 6 shmembench --bench shmem_atomic_add --benchtype latency --ntimes 100\n");
  printf("\n");
}
/* clang-format on */
