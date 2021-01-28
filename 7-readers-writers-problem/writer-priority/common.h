#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// exclude mutual reading processes at the beginning and ending
// use the resource
#define sem_w1 0
// exclude the reciprocal writing process with respect to all other processes
#define sem_sp 1
// protect entry to the critical section of the reading process
#define sem_sc 2
// exclude reciprocal writing processes at start and end
// use the resource
#define sem_w2 3
// give priority to writing over reading
#define sem_w3 4

int semid;
int shmid;

FILE *f;

static void sem_p(int nr) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = nr;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = 0;

  int change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_p(nr);
      errno = 0;
    } else {
      perror("Semop");
      printf("[READER] Could not close semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}

static void sem_v(int nr) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = nr;
  bufor_sem.sem_op = 1;
  bufor_sem.sem_flg = 0;

  int change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_v(nr);
      errno = 0;
    } else {
      perror("Semop");
      printf("[READER] Could not open semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}
