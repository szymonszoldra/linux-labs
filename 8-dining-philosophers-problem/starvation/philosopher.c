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
int i;

static void sem_v(int nr1, int nr2) {
  int change_sem;
  struct sembuf bufor_sem[2];
  bufor_sem[0].sem_num = nr1;
  bufor_sem[0].sem_op = 1;
  bufor_sem[0].sem_flg = SEM_UNDO;
  bufor_sem[1].sem_num = nr2;
  bufor_sem[1].sem_op = 1;
  bufor_sem[1].sem_flg = SEM_UNDO;
  change_sem = semop(semid, bufor_sem, 2);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_v(nr1, nr2);
      errno = 0;
    } else {
      perror("Semop");
      printf("[Philosopher] Could not open semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}

static void sem_p(int nr1, int nr2) {
  int change_sem;
  struct sembuf bufor_sem[2];
  bufor_sem[0].sem_num = nr1;
  bufor_sem[0].sem_op = -1;
  bufor_sem[0].sem_flg = SEM_UNDO;
  bufor_sem[1].sem_num = nr2;
  bufor_sem[1].sem_op = -1;
  bufor_sem[1].sem_flg = SEM_UNDO;
  change_sem = semop(semid, bufor_sem, 2);
  if (change_sem == -1) {
    if (errno == EINTR) {
      sem_v(nr1, nr2);
      errno = 0;
    } else {
      perror("Semop");
      printf("[Philosopher] Could not close semaphore. \n");
      exit(EXIT_FAILURE);
    }
  }
}

void think(int id) {
  printf("[Philosopher %d PID %d] Thinking \n", id, getpid());
}

void eat(int id) {
  printf("[Philosopher %d PID %d] Eating \n", id, getpid());
  printf("[Philosopher %d PID %d] I ate : %d times\n", id, getpid(), i);
}

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
    sem_p(id, (id + 1) % 5);
    i++;
    eat(id);
    if (id == 1 || id == 3) {
      sleep(rand() % 2 + 1);
    }
    sem_v(id, (id + 1) % 5);
  }
}
