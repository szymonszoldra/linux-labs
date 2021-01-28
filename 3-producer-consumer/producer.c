#include "common.h"

static mem_t mem;

void clean_memory() {
  detach_shmemory(mem.ptr);
  sem_p(mem.semid, SEM_WRITE);
  sem_v(mem.semid, SEM_FINISH);
  printf("[PRODUCENT] Cleaned\n");
}

void run() {
  FILE *file = fopen("dane_wejsciowe.txt", "r");

  if (!file) {
    perror("Error opening file\n");
    exit(EXIT_FAILURE);
  }

  printf("[PRODUCENT] Started producing...\n");

  srand(time(NULL));
  int wait_time = 0;
  char c;
  sem_p(mem.semid, SEM_WRITE);

  while ((c = (char)fgetc(file)) != EOF) {
    sem_p(mem.semid, SEM_WRITE);
    *(mem.ptr) = c;

    wait_time = getRandomNumber();

    if (c == '\n') {
      printf("[PRODUCENT] Sent char: \"\\n\", sleep(%d)\n", wait_time);
    } else {
      printf("[PRODUCENT] Sent char: \"%c\",  sleep(%d)\n", c, wait_time);
    }
    sem_v(mem.semid, SEM_READ);
    sleep(wait_time);
  }

  printf("[PRODUCENT] Finished\n");

  if (fclose(file) == ERROR) {
    perror("Error closing file");
    exit(EXIT_FAILURE);
  }
}

int main() {
  innit(&mem);
  run();
  clean_memory();

  return 0;
}
