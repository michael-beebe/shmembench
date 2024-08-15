/**
  @file shmembench.c
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
  /* Initialize all options to default values */
  memset(opts, 0, sizeof(*opts));

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

  int option_index = 0;
  int c;

  while ((c = getopt_long(argc, argv, "", 
          long_options, &option_index)) != -1) {
    if (c == 0) { // Long option
      const char* option_name = long_options[option_index].name;

      if (strcmp(option_name, "shmem_put") == 0) {
        opts->shmem_put = true;
      }
      else if (strcmp(option_name, "shmem_p") == 0) {
        opts->shmem_p = true;
      }
      else if (strcmp(option_name, "shmem_iput") == 0) {
        opts->shmem_iput = true;
      }
      else if (strcmp(option_name, "shmem_get") == 0) {
        opts->shmem_get = true;
      }
      else if (strcmp(option_name, "shmem_g") == 0) {
        opts->shmem_g = true;
      }
      else if (strcmp(option_name, "shmem_iget") == 0) {
        opts->shmem_iget = true;
      }
      else if (strcmp(option_name, "shmem_put_nbi") == 0) {
        opts->shmem_put_nbi = true;
      }
      else if (strcmp(option_name, "shmem_get_nbi") == 0) {
        opts->shmem_get_nbi = true;
      }
      else if (strcmp(option_name, "shmem_alltoall") == 0) {
        opts->shmem_alltoall = true;
      }
      else if (strcmp(option_name, "shmem_alltoalls") == 0) {
        opts->shmem_alltoalls = true;
      }
      else if (strcmp(option_name, "shmem_broadcast") == 0) {
        opts->shmem_broadcast = true;
      }
      else if (strcmp(option_name, "shmem_collect") == 0) {
        opts->shmem_collect = true;
      }
      else if (strcmp(option_name, "shmem_fcollect") == 0) {
        opts->shmem_fcollect = true;
      }
      else if (strcmp(option_name, "shmem_min_reduce") == 0) {
        opts->shmem_min_reduce = true;
      }
      else if (strcmp(option_name, "shmem_max_reduce") == 0) {
        opts->shmem_max_reduce = true;
      }
      else if (strcmp(option_name, "min") == 0) {
        opts->min_msg_size = atoi(optarg);
      }
      else if (strcmp(option_name, "max") == 0) {
        opts->max_msg_size = atoi(optarg);
      }
      else if (strcmp(option_name, "benchtype") == 0) {
        if (
          strcmp(optarg, "bw") == 0 ||
          strcmp(optarg, "bibw") == 0 ||
          strcmp(optarg, "latency") == 0
        ) {
          opts->benchtype = optarg;
        }
        else {
          fprintf(stderr, 
            "Invalid benchtype specified: %s. "
            "Must be 'bw', 'bibw', or 'latency'.\n", optarg
          );
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
      fprintf(stderr, "Option parsing error\n");
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
"\n"
"━━━━┓━━━━━━━━━━━┓━━━━━━━━━━━┓━━\n"
"━━━━┃━━━━━━━━━━━┃━━━━━━━━━━━┃━━\n"
"┏━━┓┗━┓┓┏┓━━┓┓┏┓┗━┓━━┓━┓━━━┓┗━┓\n"
"┃━━┫┏┓┃┗┛┃┏┓┃┗┛┃┏┓┃┏┓┃┏┓┓┏━┛┏┓┃\n"
"┣━━┃┃┃┃┃┃┃┃━┫┃┃┃┗┛┃┃━┫┃┃┃┗━┓┃┃┃\n"
"┗━━┛┛┗┛┻┻┛━━┛┻┻┛━━┛━━┛┛┗┛━━┛┛┗┛\n"
"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
"\n"
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
