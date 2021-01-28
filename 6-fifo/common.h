#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 256
#define FIFO_NAME "./moje_fifo"

long strtol_handler(char *arg) {
  char *endptr;

  long temp = strtol(arg, &endptr, 10);

  if (endptr == arg) {
    perror("strtol error. Wrong format of an argument (not a number) \n");
    exit(EXIT_FAILURE);
  }

  if (errno == ERANGE) {
    perror("strtol error. To many chars \n");
    exit(EXIT_FAILURE);
  }

  return temp;
}