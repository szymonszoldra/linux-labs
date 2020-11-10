#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * The purpose of this program is to display information about child processes
 * created by the execl function combined with the wait function. It uses first
 * program to display information about child processes.
 */

int main() {
  for (int i = 0; i < 3; i++) {
    switch (fork()) {
      case 0:
        execl("./first", "first", NULL);
        perror("Error!\n");  // If it reach that line it means that something
        exit(1);             // went wrong.
        break;
      case -1:
        perror("fork error");
        exit(1);
      default:
        break;
    }
  }

  char cmd[30];
  pid_t pid = getpid();
  sprintf(cmd, "pstree -p %d", pid);
  if (pid == getpid()) {
    system(cmd);
  }

  int statusCode;
  int processId;

  for (int i = 0; i < 3; i++) {
    processId = wait(&statusCode);
    if (processId == -1) {
      perror("Error!\n");
      exit(1);
    }
    printf("Proces %d zakonczyl sie z kodem %d\n", processId, statusCode);
  }

  return 0;
}