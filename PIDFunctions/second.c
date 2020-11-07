#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * The purpose of this program is to display information about child processes
 * created by the fork function. Then it display the process family tree.
 */

void printProcessInfo() {
  printf("PID: %d ", getpid());
  printf("PPID: %d ", getppid());
  printf("UID: %d ", getuid());
  printf("GID: %d\n", getgid());
}

int main() {
  printProcessInfo();

  pid_t pid = getpid();
  char cmd[30];
  sprintf(cmd, "pstree -p %d", pid);

  for (int i = 0; i < 3; i++) {
    switch (fork()) {
      case 0:
        printProcessInfo();
        break;
      case -1:
        printf("Error!\n");
        break;
      default:
        break;
    }
  }

  if (pid == getpid()) {
    system(cmd);
  }
  sleep(1);
  return 0;
}