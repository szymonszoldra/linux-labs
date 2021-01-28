#include <time.h>

#include "common.h"

char generate_random_char() {
  const int MIN = 33;
  const int MAX = 125;

  return (char)(rand() % (MAX - MIN + 1) + MIN);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Wrong number of arguments!\n");
    printf("Provide arguments as follows\n");
    printf("./prod PRODUCERS_NUMBER CHARS_TO_PRODUCE_NUMBER\n");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));
  system("./clear_p.sh");

  int producers_number = strtol_handler(argv[1]);
  long chars_to_produce = strtol_handler(argv[2]);

  if (producers_number <= 0 || chars_to_produce <= 0) {
    perror(
        "Error! One or more arguments less than 1 or number was too big so it "
        "was converted "
        "to negative\n");
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

  // additionally I decrease the limit by 2 to be sure that it
  // will stay space for at least one consumer, otherwise
  // launching such a number of producers will not make sense

  int limit = limit_struct.rlim_cur / 2 - proc_in_use_number - 2;

  if (limit < producers_number) {
    printf("WARNING: To many processes to create\n");
    printf("Max number of producers allowed is: %ld\n", limit);
    exit(EXIT_FAILURE);
  }

  printf("[PRODUCER] Producers: %ld\n", producers_number);
  printf("[PRODUCER] Chars for each producer: %ld\n", chars_to_produce);

  FILE *f;
  char text;
  char filename[20];
  int my_fifo;
  if (access(FIFO_NAME, F_OK) == -1) {
    my_fifo = mkfifo(FIFO_NAME, 0700);
    if (my_fifo != 0) {
      perror("mkfifo() error ");
      exit(EXIT_FAILURE);
    }
  }

  int stream;
  printf("[PRODUCER] Program running... \n");
  for (int i = 0; i < producers_number; i++) {
    switch (fork()) {
      case -1:
        perror("Fork error ");
        exit(EXIT_FAILURE);
      case 0:
        stream = open(FIFO_NAME, O_WRONLY);
        if (stream == -1) {
          perror("fifo open() error ");
          exit(EXIT_FAILURE);
        }
        sprintf(filename, "we/we_%d.txt", getpid());
        if ((f = fopen(filename, "w")) == NULL) {
          perror("Error opening file ");
          exit(EXIT_FAILURE);
        }
        for (int i = 0; i < chars_to_produce; i++) {
          text = generate_random_char();
          fputc(text, f);
          my_fifo = write(stream, (void *)&text, sizeof(char));
          if (my_fifo == -1) {
            printf("write() error \n");
            exit(EXIT_FAILURE);
          }
        }
        close(stream);
        fclose(f);
        exit(EXIT_SUCCESS);
        break;
      default:
        break;
    }
  }

  int x;
  for (int i = 0; i < producers_number; i++) {
    if (wait(&x) == -1) {
      perror("wait() error ");
      exit(EXIT_FAILURE);
    }
  }

  // Chars produced
  FILE *chars_produced_n;
  long chars_buf;
  chars_produced_n = popen("cat we/we* | wc -c", "r");
  fgets(buf, BUF_SIZE, chars_produced_n);
  pclose(chars_produced_n);
  chars_buf = strtol(buf, NULL, 10);
  printf("Produced %ld chars by %ld producers\n", chars_buf, producers_number);

  return EXIT_SUCCESS;
}
