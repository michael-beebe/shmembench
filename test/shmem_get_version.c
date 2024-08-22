// #include <shmem.h>
#include "/home/mbeebe/sw/el9-x86_64/sos@1.4/include/shmem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

int main(int argc, char *argv[]) {
  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int major, minor;
  shmem_info_get_version(&major, &minor);

  char *version = (char *)malloc(16 * sizeof(char));
  if (version != NULL) {
    snprintf(version, 16, "%d.%d", major, minor);
  }
  else {
    if (mype == 0) {
      printf("ISSUE ALLOCATING version[]!\n");
    }
  }

  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name != NULL) {
    shmem_info_get_name(name);
  }

  printf("-------------------------------------------\n");
  printf("  OpenSHMEM Name: %s\n", name);
  printf("  OpenSHMEM Version: %s\n", version);
  printf("-------------------------------------------\n");

  shmem_finalize();
  return EXIT_SUCCESS;
}


