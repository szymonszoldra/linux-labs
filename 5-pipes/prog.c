#include <limits.h>

#include "common.h"

#define BUF_SIZE 256

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

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Wrong number of arguments!\n");
    printf("Provide arguments as follows\n");
    printf("CONSUMERS_NUMBER PRODUCERS_NUMBER CHARS_TO_PRODUCE_NUMBER\n");
    exit(EXIT_FAILURE);
  }

  system("./clear.sh");

  long consumers_number = strtol_handler(argv[1]);
  long producers_number = strtol_handler(argv[2]);
  long chars_to_produce_number = strtol_handler(argv[3]);

  if (consumers_number <= 0 || producers_number <= 0 ||
      chars_to_produce_number <= 0) {
    perror("Error! One or more arguments less than 1.\n");
    exit(EXIT_FAILURE);
  }

  // Universal buffer, will be used for later functions
  char buf[BUF_SIZE];

  // Reading the number of currently running processes
  FILE *proc_file;
  proc_file = popen("ps ux | wc -l", "r");
  fgets(buf, BUF_SIZE, proc_file);
  pclose(proc_file);
  int proc_in_use_number = atoi(buf);

  // Removing the first line that displays the column titles
  proc_in_use_number--;

  // Reading the maximum number of processes that can be run
  struct rlimit limit_struct;
  getrlimit(RLIMIT_NPROC, &limit_struct);

  int limit = limit_struct.rlim_cur / 2 - proc_in_use_number;

  if (limit < (producers_number + consumers_number)) {
    printf("WARNING: To many processes to create\n");
    printf("Max combined number of producers and consuments allowed is: %d\n",
           limit);
    exit(EXIT_FAILURE);
  }

  printf("Consumers: %ld\n", consumers_number);
  printf("Producers: %ld\n", producers_number);
  printf("Chars for each producer: %ld\n", chars_to_produce_number);

  int fd[2];
  if (pipe(fd) == -1) {
    perror("Pipe() error ");
    exit(EXIT_FAILURE);
  }

  printf("Program running... \n");
  for (int i = 0; i < producers_number; i++) {
    switch (fork()) {
      case -1:
        perror("Fork error ");
        exit(EXIT_FAILURE);
      case 0:
        if (close(fd[0]) == -1) {
          perror("PROG:PROD: Close() error ");
          exit(EXIT_FAILURE);
        }
        if (dup2(fd[1], 1) == -1) {
          perror("Dup2() error ");
          exit(EXIT_FAILURE);
        }
        execl("./producer", "producer", argv[3], NULL);

        // The lines below are only executed when excel is not working
        perror("execl error\n");
        exit(EXIT_FAILURE);
        break;
      default:
        break;
    }
  }

  close(fd[1]);

  for (int i = 0; i < consumers_number; i++) {
    switch (fork()) {
      case -1:
        perror("Fork error ");
        exit(EXIT_FAILURE);
      case 0:
        if (dup2(fd[0], 0) == -1) {
          perror("Dup2() error ");
          exit(EXIT_FAILURE);
        }
        execl("./consumer", "consumer", NULL);

        // The lines below are only executed when excel is not working
        perror("execl error\n");
        exit(EXIT_FAILURE);
        break;
      default:
        break;
    }
  }

  close(*fd);

  int x;
  for (int i = 0; i < consumers_number + producers_number; i++) {
    if (wait(&x) == -1) {
      perror("Wait() error ");
      exit(EXIT_FAILURE);
    }
  }

  long chars_buf;

  // chars produced
  FILE *chars_produced_n;
  chars_produced_n = popen("cat we/we* | wc -c", "r");
  fgets(buf, BUF_SIZE, chars_produced_n);
  pclose(chars_produced_n);
  chars_buf = strtol(buf, NULL, 10);
  printf("Produced %ld chars by %ld producers\n", chars_buf, producers_number);

  // chars consumed
  FILE *chars_consumed_n;
  chars_consumed_n = popen("cat wy/wy* | wc -c", "r");
  fgets(buf, BUF_SIZE, chars_consumed_n);
  pclose(chars_consumed_n);
  chars_buf = strtol(buf, NULL, 10);
  printf("Consumed %ld chars by %ld consuments\n", chars_buf, consumers_number);

  return EXIT_SUCCESS;
}
