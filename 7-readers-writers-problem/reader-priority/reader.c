#include "common.h"

void run(int pid, int i) {
  f = fopen("./czytelnia", "w");
  fprintf(f, "PID: %d, loop: %d\n", pid, i);
  fclose(f);
  printf("[WRITER PID: %d] Writing \n", pid);
}

int main() {
  key_t key = ftok(".", 3);
  semid = semget(key, 2, 0600);

  if (semid == -1) {
    printf("[WRITER %d] Could not set semaphore\n", getpid());
    exit(EXIT_FAILURE);
  }

  srand(time(0) + getpid());

  int i = 0;
  while (1) {
    sem_p(sp);
    run(getpid(), ++i);
    usleep(rand() % 3);
    sem_v(sp);
  }
}
