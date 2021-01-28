#include "common.h"

int *lc;

void get_memory() {
  key_t key = ftok(".", 6);
  shmid = shmget(key, 2 * sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
  if (shmid == -1) {
    shmid = shmget(key, 2 * sizeof(int), 0600);
    if (shmid == -1) {
      perror("Shmget");
      printf("[READER] Problems with creating shared memory.\n");
      exit(EXIT_FAILURE);
    }
  }
}

void att_memory() {
  lc = shmat(shmid, 0, 0);
  if (*lc == -1) {
    perror("Shmat");
    printf("[READER] Problem with address allocation.\n");
    exit(EXIT_FAILURE);
  }
}

void det_memory() {
  if (shmdt(lc) == -1) {
    perror("shmdt");
    printf("[READER] Shared memory detaching problems.\n");
    exit(EXIT_FAILURE);
  }
}

void run() {
  const int size = 256;
  char buf[size];
  f = fopen("./czytelnia", "r");
  fgets(buf, size, f);
  fclose(f);
  printf("[READER PID: %d] Reading: %s\n", getpid(), buf);
}

int main() {
  key_t key = ftok(".", 3);
  semid = semget(key, 2, 0600);
  if (semid == -1) {
    printf("[READER] Could not join to semaphore queue\n");
    exit(EXIT_FAILURE);
  }
  get_memory();
  att_memory();

  srand(time(0) + getpid());

  while (1) {
    sem_p(w);
    if (lc[1] > lc[0]) {
      lc[0]++;

      if (lc[0] == 1) {
        sem_p(sp);
      }

      sem_v(w);
      run();
      usleep(rand() % 3);
      sem_p(w);
      lc[0]--;

      if (lc[0] == 0) {
        sem_v(sp);
      }
      sem_v(w);
    } else {
      sem_v(w);
    }
  }
  det_memory();
}
