#include "common.h"

static mem_t mem;

void clean_memory() {
  printf("[Consumer] Cleaning\n");
  detach_shmemory(mem.ptr);

  if (semctl(mem.semid, 0, IPC_RMID) == ERROR) {
    perror("Error cleaning semaphores\n");
    exit(EXIT_FAILURE);
  }

  if (shmctl(mem.shmid, IPC_RMID, NULL) == ERROR) {
    perror("Error cleaning shmemory\n");
    exit(EXIT_FAILURE);
  }

  printf("[Consumer] Cleaned\n");
}

void run() {
  FILE *file = fopen("wynik.txt", "w");

  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  printf("[Consumer] Consuming started\n");

  sem_v(mem.semid, SEM_WRITE);  // permission to start production
  sem_v(mem.semid, SEM_WRITE);  // permission to send the first character

  srand(time(NULL));
  int wait_time = 0;

  while (sem_p_nowait(mem.semid, SEM_FINISH)) {
    if (sem_p_nowait(mem.semid, SEM_READ)) {
      continue;
    }

    // WRITE TO FILE
    if (fputc((int)(*(mem.ptr)), file) == EOF) {
      perror("Error writing to file");
      exit(EXIT_FAILURE);
    }

    wait_time = getRandomNumber();

    char c = *(mem.ptr);
    if (c == '\n') {
      printf("[Consumer] Recievied char \"\\n\", sleep(%d)\n", wait_time);
    } else {
      printf("[Consumer] Recieved \"%c\",  sleep(%d)\n", c, wait_time);
    }

    sem_v(mem.semid, SEM_WRITE);
    sleep(wait_time);
  }

  printf("[Consumer] Finished\n");

  // CLOSE FILE
  if (fclose(file) == ERROR) {
    perror("Error file closing");
    exit(EXIT_FAILURE);
  }
}

int main() {
  innit(&mem);
  run();
  clean_memory();

  return 0;
}
