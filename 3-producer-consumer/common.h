#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define ERROR -1
#define SEM_WRITE 0
#define SEM_READ 1
#define SEM_FINISH 2

key_t get_key() {
  key_t key = ftok(".", 3);
  if (key == ERROR) {
    perror("Key getting error!\n");
    exit(EXIT_FAILURE);
  }
  return key;
}

int get_semid(key_t key) {
  int semid = semget(key, 3, IPC_CREAT | IPC_EXCL | 0200);

  if (semid == ERROR) {
    semid = semget(key, 3, IPC_CREAT | 0200);
    if (semid == ERROR) {
      perror("Semaphore error!\n");
      exit(EXIT_FAILURE);
    }
    return semid;
  }

  if (semctl(semid, SEM_WRITE, SETVAL, 0) == ERROR ||
      semctl(semid, SEM_READ, SETVAL, 0) == ERROR ||
      semctl(semid, SEM_FINISH, SETVAL, 0) == ERROR) {
    perror("Semaphore error!\n");
    exit(EXIT_FAILURE);
  }
  printf("Initialized semaphores!\n");

  return semid;
}

void sem_p(int semid, int sem_num) {
  struct sembuf bufor_sem;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = 0;
  bufor_sem.sem_num = sem_num;
  int op;

  do {
    op = semop(semid, &bufor_sem, 1);
  } while (op == ERROR && errno == EINTR);

  if (op == ERROR) {
    perror("Semaphore wait error!\n");
    exit(EXIT_FAILURE);
  }
}

int sem_p_nowait(int semid, int sem_num) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = sem_num;
  bufor_sem.sem_op = -1;
  bufor_sem.sem_flg = IPC_NOWAIT;
  int op = semop(semid, &bufor_sem, 1);
  if (op == ERROR && errno != EAGAIN) {
    perror("Semaphore IPC_NOWAIT error!\n");
    exit(EXIT_FAILURE);
  }
  return op;
}

void sem_v(int semid, int sem_num) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num = sem_num;
  bufor_sem.sem_op = 1;
  bufor_sem.sem_flg = 0;

  int op = semop(semid, &bufor_sem, 1);
  if (op == ERROR) {
    perror("Semaphore post error!\n");
    exit(EXIT_FAILURE);
  }
}

char *attach_shmemory(int shmid) {
  void *ptr = shmat(shmid, NULL, 0);
  if (ptr == (void *)ERROR) {
    perror("Shmemory error\n");
    exit(EXIT_FAILURE);
  }

  return (char *)ptr;
}

void detach_shmemory(char *ptr) {
  if (shmdt((void *)ptr) == ERROR) {
    perror("Shmemory detach error\n");
    exit(EXIT_FAILURE);
  }
  printf("Odlaczono pamiec\n");
}

char *get_shmemory_ptr(key_t key, int *id) {
  *id = shmget(key, sizeof(char), IPC_CREAT | 0600);
  if (*id == ERROR) {
    perror("Shmemory error");
    exit(EXIT_FAILURE);
  }

  return attach_shmemory(*id);
}

typedef struct {
  int semid;
  int shmid;
  char *ptr;
} mem_t;

void innit(mem_t *mem) {
  key_t key = get_key();
  mem->semid = get_semid(key);
  mem->ptr = get_shmemory_ptr(key, &(mem->shmid));
}

int get_random_number() { return rand() % 2 + 1; }
