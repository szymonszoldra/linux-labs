#include <pthread.h>

#include "common.h"

pid_t client_pid;
int queue_id;
char temp[MAX];

pthread_t send;
pthread_t receive;

void* send_handler(void* args) {
  struct statement msg;
  msg.mtype = SERVER;
  msg.mtype1 = client_pid;

  while (1) {
    sleep(1);
    printf("[CLIENT %d] Enter your communicate: ", client_pid);
    scanf("%s", temp);

    if (strlen(temp) > MAX) {
      printf("Maximum size exceeded. The message has been cut\n");
    }

    for (int i = 0; i < MAX; i++) {
      msg.mtext[i] = temp[i];
    }

    // Queue overflow control
    struct msqid_ds control;
    msgctl(queue_id, IPC_STAT, &control);

    if (MAX + sizeof(long) + control.__msg_cbytes >= control.msg_qbytes) {
      printf(
          "[CLIENT %d] Sending this message would fill the queue! "
          "The message was not sent\n",
          client_pid);
      continue;
    }
    // ----------------------------------

    printf("[CLIENT %d] Sent: %s \n to %ld\n", client_pid, msg.mtext,
           msg.mtype);
    if (msgsnd(queue_id, &msg, strlen(msg.mtext) + 1 + sizeof(long),
               IPC_NOWAIT) == -1) {
      if (errno == EIDRM) {
        exit(EXIT_FAILURE);
      }
      perror("[CLIENT] Error adding a message to the queue \n");
    }
  }
  pthread_exit(EXIT_SUCCESS);
}

void* receive_handler(void* args) {
  struct statement msg;
  while (1) {
    sleep(1);

    if (msgrcv(queue_id, &msg, MAX + sizeof(long), client_pid, MSG_NOERROR) ==
        -1) {
      if (errno == EIDRM) {
        exit(EXIT_FAILURE);
      }
      perror("[CLIENT] Failed to get the message from the queue ");
      exit(EXIT_FAILURE);
    }

    printf("[CLIENT %d] Received: %s\n from %ld\n", client_pid, msg.mtext,
           msg.mtype1);
  }
  pthread_exit(EXIT_SUCCESS);
}

void make_queue() {
  key_t key = ftok(".", FTOK_KEY);
  queue_id = msgget(key, IPC_CREAT);

  if (queue_id == -1) {
    perror("[CLIENT] Error creating queue");
    exit(EXIT_FAILURE);
  } else {
    printf("[CLIENT] Joined queue number: %d\n", queue_id);
  }
}

int main() {
  make_queue();
  struct statement msg;
  client_pid = getpid();

  if (pthread_create(&send, NULL, send_handler, &msg) == -1) {
    printf("Error creating sending thread\n ");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&receive, NULL, receive_handler, &msg) == -1) {
    printf("Error creating receiving thread\n ");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(send, NULL) == -1) {
    printf("Error joining the sending thread\n ");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(receive, NULL) == -1) {
    printf("Error joining the receiving thread\n ");
    exit(EXIT_FAILURE);
  }

  if (pthread_detach(send) == -1) {
    printf("Error detaching the sending thread\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_detach(receive) == -1) {
    printf("Error detaching the receiving thread\n");
    exit(EXIT_FAILURE);
  }

  printf("\n");
  exit(EXIT_SUCCESS);
}
