#include "common.h"

int *lc;

void get_mem() {
  key_t key = ftok(".", 6);
  shmid = shmget(key, 3 * sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
  if (shmid == -1) {
    shmid = shmget(key, 3 * sizeof(int), 0600);
    if (shmid == -1) {
      perror("Shmget");
      printf("[READER] Problems with creating shared memory..\n");
      exit(EXIT_FAILURE);
    }
  }
}

void att_mem() {
  lc = shmat(shmid, 0, 0);
  if (*lc == -1) {
    perror("Shmat");
    printf("[READER] Problem with address allocation. \n");
    exit(EXIT_FAILURE);
  }
}

void det_mem() {
  if (shmdt(lc) == -1) {
    perror("shmdt");
    printf("[READER] Shared memory detaching problems \n");
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
  semid = semget(key, 5, 0600);
  if (semid == -1) {
    printf("[READER] Could not create semaphore\n");
    exit(EXIT_FAILURE);
  }
  get_mem();
  att_mem();

  srand(time(0) + getpid());

  while (1) {
    sem_p(sem_w3);
    sem_p(sem_sc);
    sem_p(sem_w1);
    if (lc[1] > lc[0]) {
      lc[0]++;

      if (lc[0] == 1) {
        sem_p(sem_sp);
      }

      sem_v(sem_w1);
      sem_v(sem_sc);
      sem_v(sem_w3);
      run();
      usleep(rand() % 3);
      sem_p(sem_w1);
      lc[0]--;

      if (!lc[0]) {
        sem_v(sem_sp);
      }

      sem_v(sem_w1);
    } else {
      sem_v(sem_w1);
      sem_v(sem_sc);
      sem_v(sem_w3);
    }
  }
  det_mem();
}