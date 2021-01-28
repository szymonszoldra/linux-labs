#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int semid;
char *forks_arr[5] = {"0", "1", "2", "3", "4"};

static void set_sem(int semaphore) {
  for (int i = 0; i < 5; i++) {
    if (semctl(semaphore, i, SETVAL, 1) == -1) {
      printf("[Main] Could not set semaphore.\n");
      exit(EXIT_FAILURE);
    } else {
      printf("[Main] Set semaphore: %d.\n", i);
    }
  }
}

static void rmv_sem(int k) {
  int sem;
  if ((sem = semctl(k, 0, IPC_RMID)) == -1) {
    printf("[Main] Could not remove semaphore \n");
    exit(EXIT_FAILURE);
  } else {
    printf("[Main] Semaphore removed : %d\n", sem);
  }
}

void sighandler(int signum, siginfo_t *info, void *ptr) {
  if (signum == 2) {
    printf("[Main] SIGINT\n");
    rmv_sem(semid);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  struct sigaction sa;
  sa.sa_sigaction = sighandler;
  sigaction(SIGINT, &sa, NULL);

  key_t key = ftok(".", 3);
  semid = semget(key, 5, IPC_CREAT | 0600);
  if (semid == -1) {
    perror("Semget");
    printf("[Main] Could not create semaphore");
    exit(EXIT_FAILURE);
  }

  set_sem(semid);

  for (int i = 0; i < 5; i++) {
    switch (fork()) {
      case -1: {
        perror("Fork");
        rmv_sem(semid);
        exit(EXIT_FAILURE);
      }
      case 0: {
        execl("./philosopher", "", forks_arr[i], NULL);
        perror("Execl");
        rmv_sem(semid);
        exit(EXIT_FAILURE);
      }
    }
  }

  int x;
  for (int i = 0; i < 5; i++) {
    if (wait(&x) == -1) {
      perror("wait() error ");
      exit(EXIT_FAILURE);
    }
  }
}