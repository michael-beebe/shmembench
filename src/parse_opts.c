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
    {"shmem_p", no_argument, 0, 0},
    {"shmem_iput", no_argument, 0, 0},
    {"shmem_get", no_argument, 0, 0},
    {"shmem_g", no_argument, 0, 0},
    {"shmem_iget", no_argument, 0, 0},
    {"shmem_put_nbi", no_argument, 0, 0},
    {"shmem_get_nbi", no_argument, 0, 0},
    {"shmem_alltoall", no_argument, 0, 0},
    {"shmem_alltoalls", no_argument, 0, 0},
    {"shmem_broadcast", no_argument, 0, 0},
    {"shmem_collect", no_argument, 0, 0},
    {"shmem_fcollect", no_argument, 0, 0},
    {"shmem_min_reduce", no_argument, 0, 0},
    {"shmem_max_reduce", no_argument, 0, 0},
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
      else if (strcmp(option_name, "shmem_p") == 0) {
        opts->shmem_p = true;
        *benchmark = strdup("shmem_p");
      }
      else if (strcmp(option_name, "shmem_iput") == 0) {
        opts->shmem_iput = true;
        *benchmark = strdup("shmem_iput");
      }
      else if (strcmp(option_name, "shmem_get") == 0) {
        opts->shmem_get = true;
        *benchmark = strdup("shmem_get");
      }
      else if (strcmp(option_name, "shmem_g") == 0) {
        opts->shmem_g = true;
        *benchmark = strdup("shmem_g");
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
      else if (strcmp(option_name, "shmem_min_reduce") == 0) {
        opts->shmem_min_reduce = true;
        *benchmark = strdup("shmem_min_reduce");
      }
      else if (strcmp(option_name, "shmem_max_reduce") == 0) {
        opts->shmem_max_reduce = true;
        *benchmark = strdup("shmem_max_reduce");
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
  printf("\nThis program is a performance benchmark suite for "
          "OpenSHMEM Implementations\n");

  printf("\nUsage: shmemvv [options]\n");
  printf("\nOptions:\n");
  // TODO: list the options
}
