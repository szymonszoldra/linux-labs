#include <ctype.h>

#include "common.h"

int queue_id;
pid_t server_pid;

void make_queue() {
  key_t key = ftok(".", FTOK_KEY);
  queue_id = msgget(key, IPC_CREAT | 0600);

  if (queue_id == -1) {
    perror("[SERVER] Error creating queue");
    exit(EXIT_FAILURE);
  } else {
    printf("[SERVER %d] Created queue number: %d\n", server_pid, queue_id);
  }
}

void delete_queue() {
  if (msgctl(queue_id, IPC_RMID, 0) == -1) {
    perror("[SERVER] Error removing message queue ");
  } else {
    printf("[SERVER %d] Deleted queue number: %d\n", server_pid, queue_id);
  }
}

void close_all(int sig_num) {
  signal(SIGINT, close_all);
  delete_queue(sig_num);
}

int main() {
  make_queue();
  signal(SIGINT, close_all);
  struct statement msg;
  server_pid = getpid();

  while (1) {
    if (msgrcv(queue_id, &msg, MAX + sizeof(long), SERVER, MSG_NOERROR) == -1) {
      if (errno == EIDRM || errno == EINTR) {
        exit(EXIT_FAILURE);
      } else {
        perror("[SERVER] Error reveiving message");
        exit(EXIT_FAILURE);
      }
    }

    printf("[SERVER %d] Received: %s \n from %ld\n", server_pid, msg.mtext,
           msg.mtype1);

    for (int i = 0; i < strlen(msg.mtext); i++) {
      msg.mtext[i] = toupper(msg.mtext[i]);
    }

    msg.mtype = msg.mtype1;
    msg.mtype1 = server_pid;

    printf("[SERVER %d] Sent: %s \n to %ld\n", server_pid, msg.mtext,
           msg.mtype);

    if (msgsnd(queue_id, &msg, strlen(msg.mtext) + 1 + sizeof(long), 0) == -1) {
      perror("[SERVER] Error adding the message!");
      exit(EXIT_FAILURE);
    }

    printf("\n");
  }
  exit(EXIT_SUCCESS);
}
