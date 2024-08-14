/**
  @file main.cpp
  @brief main driver file
 */

#include "shmembench.h"
#include "benchmarks.h"

/**
  @brief Main function for running the test suite.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
*/
int main(int argc, char *argv[]) {
  /* Start the OpenSHMEM instance */
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  
  /* Display logo */
  if (mype == 0) { display_logo(); }

  /* Get the OpenSHMEM impl version */
  int major, minor;
  shmem_info_get_version(&major, &minor);
  char *version = (char *)malloc(16 * sizeof(char));
  if (version != NULL) {
    snprintf(version, 16, "%d.%d", major, minor);
    if ( !(strstr(version, "1.5") != NULL) ) {
      if (mype == 0) {
        fprintf(stderr,
          RED_COLOR "ERROR: OpenSHMEM v1.5 is required!" RESET_COLOR "\n"
        );
      }
      free(version);
      shmem_finalize();
      return EXIT_FAILURE;
    }
  }

  /* Get the OpenSHMEM impl name */
  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name != NULL) {
    shmem_info_get_name(name);
  }

  /* TODO: parse options */

  /* TODO: Display header */

  /* TODO: Run benchmarks */
  
  /* Finalize the program */
  free(version);
  free(name);
  shmem_finalize();  
  return EXIT_SUCCESS;

}