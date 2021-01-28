#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

FILE *f;

int semid;
int shmid;
int *adress;

static void set_sem(int semafor) {
  int sem;
  for (int i = 0; i < 5; i++) {
    sem = semctl(semafor, i, SETVAL, 1);
    if (sem == -1) {
      printf("[MAIN] Could not set the semaphore.\n");
      exit(EXIT_FAILURE);
    } else {
      printf("[MAIN] Semaphore set: %d.\n", i);
    }
  }
}

static void rmv_sem(int k) {
  int sem;
  sem = semctl(k, 0, IPC_RMID);
  if (sem == -1) {
    printf("[MAIN] Could not delete semaphores \n");
    exit(EXIT_FAILURE);
  } else {
    printf("[MAIN] Semaphores deleted %d\n", sem);
  }
}

void get_memory() {
  key_t key = ftok(".", 6);
  shmid = shmget(key, 3 * sizeof(int), 0600 | IPC_CREAT | IPC_EXCL);
  if (shmid == -1) {
    shmid = shmget(key, 3 * sizeof(int), 0600);
    if (shmid == -1) {
      perror("Shmget");
      printf("[MAIN] Problems with creating shared memory.\n");
      rmv_sem(semid);
      exit(EXIT_FAILURE);
    }
  } else
    printf("[MAIN] Shared memory was created: %d\n", shmid);
}

void att_memory() {
  adress = shmat(shmid, 0, 0);
  if (*adress == -1) {
    perror("Shmat");
    printf("[MAIN] Problem with address allocation.\n");
    rmv_sem(semid);
    exit(EXIT_FAILURE);
  } else
    printf("[MAIN] Address space has been allocated.\n");
}

void det_memory() {
  if (shmctl(shmid, IPC_RMID, 0) == -1 || shmdt(adress) == -1) {
    perror("Shmctl|shmdt");
    printf("[MAIN] Shared memory disconnection problems.\n");
    exit(EXIT_FAILURE);
  } else
    printf("[MAIN] Shared memory has been disconnected\n");
}

void sighandler(int signum, siginfo_t *info, void *ptr) {
  if (signum == 2) {
    printf("[MAIN] SIGINT!\n");
    rmv_sem(semid);
    det_memory();
    exit(0);
  }
}

long handle_strtol(const char *arg) {
  char *endptr;
  errno = 0;

  long temp = strtol(arg, &endptr, 10);

  if (errno == ERANGE) {
    printf("[MAIN] Invalid data range\n");
    exit(EXIT_FAILURE);
  }

  if (endptr == arg) {
    printf("[MAIN] Incorrect variable format specified (other than a number)");
    exit(EXIT_FAILURE);
  }

  return temp;
}

int main(int argc, char *argv[]) {
  struct sigaction sa;
  sa.sa_sigaction = sighandler;
  sigaction(SIGUSR2, &sa, NULL);

  if (argc != 4) {
    printf("[MAIN] Wrong number of arguments, expected 3\n");
    exit(EXIT_FAILURE);
  }

  long writers_n = handle_strtol(argv[1]);
  long readers_n = handle_strtol(argv[2]);
  long M = handle_strtol(argv[3]);

  const int size = 256;
  char buf[size];

  f = popen("ps ux | wc -l", "r");
  fgets(buf, size, f);
  long proc_running = handle_strtol(buf);
  pclose(f);

  f = popen("ulimit -u", "r");
  fgets(buf, size, f);
  long limit = handle_strtol(buf);
  pclose(f);

  if ((limit - proc_running) < (writers_n + readers_n)) {
    printf("[MAIN] Limit error\n");
    printf("[MAIN] Limit: %d\n", limit - proc_running);
    exit(EXIT_FAILURE);
  }

  key_t key = ftok(".", 3);
  semid = semget(key, 5, IPC_CREAT | 0600);
  if (semid == -1) {
    perror("Semget");
    printf("[MAIN] Error creating semaphores");
    exit(EXIT_FAILURE);
  }

  set_sem(semid);
  get_memory();
  att_memory();

  adress[0] = 0;
  adress[1] = M;
  adress[2] = 0;

  int i;
  for (i = 0; i < writers_n; i++) {
    switch (fork()) {
      case -1:
        perror("Fork");
        rmv_sem(semid);
        det_memory();
        exit(EXIT_FAILURE);

      case 0: {
        execl("./writer", "", NULL);
        perror("Execl failed");
        rmv_sem(semid);
        exit(EXIT_FAILURE);
      }
    }
    usleep(rand() % 3);
  }

  for (i = 0; i < readers_n; i++) {
    switch (fork()) {
      case -1:
        perror("Fork");
        rmv_sem(semid);
        det_memory();
        exit(EXIT_FAILURE);

      case 0: {
        execl("./reader", "", NULL);
        perror("Execl failed");
        rmv_sem(semid);
        exit(EXIT_FAILURE);
      }
    }
    usleep(rand() % 3);
  }

  int x;
  for (int i = 0; i < readers_n + writers_n; i++) {
    if (wait(&x) == -1) {
      perror("wait() error ");
      exit(EXIT_FAILURE);
    }
  }
}
