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

// ensuring that the writer process is mutually exclusive for all
// other processes
#define sp 0
// ensuring the mutual exclusion of the reading process in moments
// start and stop using the resource
#define w 1

int semid;
int shmid;
FILE *f;

void sem_p(int sem_num) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = sem_num;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = SEM_UNDO;

  int change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_p(sem_num);
      errno = 0;
    } else {
      perror("Semop");
      printf("[WRITER %d] Could not close semaphore. \n", getpid());
      exit(EXIT_FAILURE);
    }
  }
}

void sem_v(int sem_num) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = sem_num;
  bufor_sem.sem_op = 1;
  bufor_sem.sem_flg = SEM_UNDO;

  int change_sem = semop(semid, &bufor_sem, 1);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_v(sem_num);
      errno = 0;
    } else {
      perror("Semop");
      printf("[WRITER %d] Could not open semaphore. \n", getpid());
      exit(EXIT_FAILURE);
    }
  }
}