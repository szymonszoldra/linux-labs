#include "common.h"

int *lp;

void get_mem() {
  key_t key = ftok(".", 6);
  shmid = shmget(key, 3 * sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
  if (shmid == -1) {
    shmid = shmget(key, 3 * sizeof(int), 0600);
    if (shmid == -1) {
      perror("Shmget");
      printf("[WRITER] Error creating shared memory.\n");
      exit(EXIT_FAILURE);
    }
  }
}

void att_mem() {
  lp = shmat(shmid, 0, 0);
  if (*lp == -1) {
    perror("Shmat");
    printf("[WRITER] Problem with address allocation\n");
    exit(EXIT_FAILURE);
  }
}

void det_mem() {
  if (shmdt(lp) == -1) {
    perror("shmdt");
    printf("[WRITER] Shared memory detaching problems..\n");
    exit(EXIT_FAILURE);
  }
}

void run(int pid, int i) {
  f = fopen("./czytelnia", "w");
  fprintf(f, "PID: %d, loop: %d\n", pid, i);
  fclose(f);
  printf("[WRITER PID %d] Writing \n", pid);
  printf("Writers in queue: %d\n", semctl(semid, 1, GETNCNT));
}

int main() {
  int i = 0;
  key_t key = ftok(".", 3);
  semid = semget(key, 5, 0600);
  if (semid == -1) {
    printf("[WRITER] Could not create semaphore.\n");
    exit(EXIT_FAILURE);
  }

  get_mem();
  att_mem();

  while (1) {
    sem_p(sem_w2);
    lp[2]++;

    if (lp[2] == 1) {
      sem_p(sem_sc);
    }

    sem_v(sem_w2);
    sem_p(sem_sp);
    run(getpid(), ++i);
    usleep(rand() % 3);
    sem_v(sem_sp);
    sem_p(sem_w2);
    lp[2]--;

    if (!lp[2]) {
      sem_v(sem_sc);
    }
    sem_v(sem_w2);
  }
  det_mem();
}