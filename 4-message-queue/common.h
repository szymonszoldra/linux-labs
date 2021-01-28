#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 8184
#define SERVER 1
#define CLIENT 2
#define FTOK_KEY 'Z'

struct statement {
  long int mtype;
  long int mtype1;
  char mtext[MAX];
};
