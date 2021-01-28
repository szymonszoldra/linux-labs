#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int semid;

static void sem_p(int nr) {
  int change_sem;
  struct sembuf bufor_sem;
  bufor_sem.sem_num = nr;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = SEM_UNDO;
  change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_p(nr);
      errno = 0;
    } else {
      perror("Semop");
      printf("[Philosopher] Could not close semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}

static void sem_v(int nr) {
  int change_sem;
  struct sembuf bufor_sem;
  bufor_sem.sem_num = nr;
  bufor_sem.sem_op = 1;
  bufor_sem.sem_flg = SEM_UNDO;
  change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_v(nr);
      errno = 0;
    } else {
      perror("Semop");
      printf("[Philosopher] Could not open semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}

void think(int id) {
  printf("[Philosopher %d PID %d] Thinking \n", id, getpid());
}

void eat(int id) { printf("[Philosopher %d PID %d] Eating \n", id, getpid()); }

int main(int argc, char *argv[]) {
  long id = strtol(argv[1], NULL, 10);
  srand(time(0) + getpid());
  if (errno == ERANGE) {
    printf("Invalid data range");
    exit(EXIT_FAILURE);
  }

  key_t key = ftok(".", 3);
  semid = semget(key, 5, 0600);
  if (semid == -1) {
    printf("[Philosopher] Could not join to semaphore set\n");
    exit(EXIT_FAILURE);
  }
  while (1) {
    think(id);
    sem_p(id);
    sem_p((id + 1) % 5);
    // sleep(rand() % 2);
    eat(id);
    sem_v(id);
    sem_v((id + 1) % 5);
  }
}
