/**
  @file parse_opts.c
  @details Parse the runtime options from the command line  
 */

#include "parse_opts.h"

/******************************************************************
  @brief Parses runtime options
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @param benchmark** Routine to be tested
  @param benchtype Either bw, bibw, or latency
  @return True if parsing is successful, false otherwise.
 
 FIXME: getopt_long is printing certain errors messages npes
          number of times. Not sure how to find a workaround
          for this. We could suppress the erros but that seems
          less than ideal
 ******************************************************************/
bool parse_opts(int argc, char *argv[], options *opts,
                char **benchmark, char **benchtype,
                int *min_msg_size, int *max_msg_size) 
{
  /* Initialize all options to default values */
  memset(opts, 0, sizeof(*opts));
  
  /* Set default values for min_msg_size and max_msg_size */
  *min_msg_size = 1;
  *max_msg_size = 1024;

  /* Define runtime options */
  static struct option long_options[] = {
    {"shmem_put", no_argument, 0, 0},
    {"shmem_iput", no_argument, 0, 0},
    {"shmem_get", no_argument, 0, 0},
    {"shmem_iget", no_argument, 0, 0},
    {"shmem_put_nbi", no_argument, 0, 0},
    {"shmem_get_nbi", no_argument, 0, 0},
    {"shmem_alltoall", no_argument, 0, 0},
    {"shmem_alltoalls", no_argument, 0, 0},
    {"shmem_broadcast", no_argument, 0, 0},
    {"shmem_collect", no_argument, 0, 0},
    {"shmem_fcollect", no_argument, 0, 0},
    {"min", required_argument, 0, 0},
    {"max", required_argument, 0, 0},
    {"benchtype", required_argument, 0, 0},
    {"help", no_argument, 0, 0},
    {0, 0, 0, 0} /* Terminator */
  };

  /* Change what errors getopt_long will print */
  opterr = 1;

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "", 
          long_options, &option_index)) != -1) {
    if (c == 0) {
      const char* option_name = long_options[option_index].name;

      if (strcmp(option_name, "shmem_put") == 0) {
        opts->shmem_put = true;
        *benchmark = strdup("shmem_put");
      }
      else if (strcmp(option_name, "shmem_iput") == 0) {
        opts->shmem_iput = true;
        *benchmark = strdup("shmem_iput");
      }
      else if (strcmp(option_name, "shmem_get") == 0) {
        opts->shmem_get = true;
        *benchmark = strdup("shmem_get");
      }
      else if (strcmp(option_name, "shmem_iget") == 0) {
        opts->shmem_iget = true;
        *benchmark = strdup("shmem_iget");
      }
      else if (strcmp(option_name, "shmem_put_nbi") == 0) {
        opts->shmem_put_nbi = true;
        *benchmark = strdup("shmem_put_nbi");
      }
      else if (strcmp(option_name, "shmem_get_nbi") == 0) {
        opts->shmem_get_nbi = true;
        *benchmark = strdup("shmem_get_nbi");
      }
      else if (strcmp(option_name, "shmem_alltoall") == 0) {
        opts->shmem_alltoall = true;
        *benchmark = strdup("shmem_alltoall");
      }
      else if (strcmp(option_name, "shmem_alltoalls") == 0) {
        opts->shmem_alltoalls = true;
        *benchmark = strdup("shmem_alltoalls");
      }
      else if (strcmp(option_name, "shmem_broadcast") == 0) {
        opts->shmem_broadcast = true;
        *benchmark = strdup("shmem_broadcast");
      }
      else if (strcmp(option_name, "shmem_collect") == 0) {
        opts->shmem_collect = true;
        *benchmark = strdup("shmem_collect");
      }
      else if (strcmp(option_name, "shmem_fcollect") == 0) {
        opts->shmem_fcollect = true;
        *benchmark = strdup("shmem_fcollect");
      }
      else if (strcmp(option_name, "min") == 0) {
        opts->min_msg_size = atoi(optarg);
        if (opts->min_msg_size <= 0) {
          opts->min_msg_size = 1; /* Set a default if not provided */
        }
        *min_msg_size = opts->min_msg_size;
      }
      else if (strcmp(option_name, "max") == 0) {
        opts->max_msg_size = atoi(optarg);
        if (opts->max_msg_size <= 0 || 
            opts->max_msg_size < opts->min_msg_size)
        {
          opts->max_msg_size = *min_msg_size * 128; /* Set a default max if invalid */
        }
        *max_msg_size = opts->max_msg_size;
      }
      else if (strcmp(option_name, "benchtype") == 0) {
        if (
          strcmp(optarg, "bw") == 0 ||
          strcmp(optarg, "bibw") == 0 ||
          strcmp(optarg, "latency") == 0
        ) {
          opts->benchtype = optarg;
          *benchtype = strdup(optarg);
        }
        else {
          if (shmem_my_pe() == 0) {
            fprintf(stderr, 
              "Invalid benchtype specified: %s. "
              "Must be 'bw', 'bibw', or 'latency'.\n", optarg
            );
          }
          return false;
        }
      }
      else if (strcmp(option_name, "help") == 0) {
        opts->help = true;
      }
      else {
        return false; /* No valid options */
      }
    }
    else {
      return false;
    }
  }

  return true;
}

/******************************************************************
  @brief Displays usage information for the test suite.

  Prints out the usage information and available options
 ******************************************************************/
void display_help() {
  printf("\n\nThis program is a performance benchmark suite for "
          "OpenSHMEM Implementations\n");

  printf("\nUsage:  oshrun -np <num PEs> shmembench [options]\n");

  printf("\nOptions:\n");
  printf("  --shmem_put             Enable shmem_put benchmark\n");
  printf("  --shmem_iput            Enable shmem_iput benchmark\n");
  printf("  --shmem_get             Enable shmem_get benchmark\n");
  printf("  --shmem_iget            Enable shmem_iget benchmark\n");
  printf("  --shmem_put_nbi         Enable shmem_put_nbi benchmark\n");
  printf("  --shmem_get_nbi         Enable shmem_get_nbi benchmark\n");
  printf("  --shmem_alltoall        Enable shmem_alltoall benchmark\n");
  printf("  --shmem_alltoalls       Enable shmem_alltoalls benchmark\n");
  printf("  --shmem_broadcast       Enable shmem_broadcast benchmark\n");
  printf("  --shmem_collect         Enable shmem_collect benchmark\n");
  printf("  --shmem_fcollect        Enable shmem_fcollect benchmark\n");
  printf("\n");
  printf("  --benchtype <type>      Set the benchmark type (bw, bibw, latency)\n");
  printf("\n");
  printf("  --min <size>            Set the minimum message size in bytes (default: 1)\n");
  printf("  --max <size>            Set the maximum message size in bytes (default: 1024)\n");
  printf("\n");
  printf("  --help                  Display this help message\n");

  printf("\nExample Usage:\n");
  printf("   oshrun -np 2 shmembench --shmem_put --benchtype bw --min 128 --max 1024\n\n");
}
