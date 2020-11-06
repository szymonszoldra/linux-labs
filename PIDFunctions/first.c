#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/** 
  * The purpose of this program is to display information about the process, 
  * It is used by the other program (third.c)
  */

int main()
{
  printf("PID: %d ", getpid());
  printf("PPID: %d ", getppid());
  printf("UID: %d ", getuid());
  printf("GID: %d\n", getgid());
  return 0;
}