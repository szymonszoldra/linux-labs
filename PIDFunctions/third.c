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
        break;
      case -1:
        perror("fork error");
        exit(1);
      default:
        break;
    }
  }

  int statusCode;
  int processId;

  for (int i = 0; i < 3; i++) {
    processId = wait(&statusCode);
    printf("Proces %d zakonczyl sie z kodem %d\n", processId, statusCode);
  }

  return 0;
}