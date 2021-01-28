#include "common.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Wrong number of arguments!\n");
    printf("Provide arguments as follows\n");
    printf("./kons CONSUMERS_NUMBER\n");
    exit(EXIT_FAILURE);
  }

  system("./clear_k.sh");
  int consumers_number = strtol_handler(argv[1]);

  if (consumers_number <= 0) {
    perror(
        "Error! Argument less than 1 or number is to big, so it was converted "
        "to negative\n");
    exit(EXIT_FAILURE);
  }

  int stream;

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

  if (limit < consumers_number) {
    printf("WARNING: To many processes to create\n");
    printf("Max number of consumers allowed is: %ld\n", limit);
    exit(EXIT_FAILURE);
  }

  printf("[CONSUMER] consumers: %ld\n", consumers_number);

  stream = open(FIFO_NAME, O_RDONLY);
  if (stream == -1) {
    perror("Run producer first! fifo open() error ");
    exit(EXIT_FAILURE);
  }

  FILE *f;
  char text;
  char filename[20];

  printf("[CONSUMER] Program running... \n");

  for (int i = 0; i < consumers_number; i++) {
    switch (fork()) {
      case -1:
        perror("fork() error ");
        exit(EXIT_FAILURE);
      case 0:
        sprintf(filename, "wy/wy_%d.txt", getpid());
        if ((f = fopen(filename, "w")) == NULL) {
          perror("Error opening file");
          exit(EXIT_FAILURE);
        }
        while (read(stream, (void *)&text, sizeof(char)) > 0) {
          fputc(text, f);
        }
        close(stream);
        exit(EXIT_SUCCESS);
        break;
      default:
        break;
    }
  }

  int x;
  for (int i = 0; i < consumers_number; i++) {
    if (wait(&x) == -1) {
      perror("wait() error ");
      exit(EXIT_FAILURE);
    }
  }

  unlink(FIFO_NAME);

  // Chars consumed
  FILE *chars_consumed_n;
  long chars_buf;
  chars_consumed_n = popen("cat wy/wy* | wc -c", "r");
  fgets(buf, BUF_SIZE, chars_consumed_n);
  pclose(chars_consumed_n);
  chars_buf = strtol(buf, NULL, 10);
  printf("Consumed %ld chars by %ld consuments\n", chars_buf, consumers_number);

  return EXIT_SUCCESS;
}
