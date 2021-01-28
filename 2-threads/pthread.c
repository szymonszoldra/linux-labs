#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int arr[2][10];
int sum1 = 0;
int sum2 = 0;

void* sum_first_line(void* args) {
  for (int i = 0; i < 10; i++) {
    sum1 += arr[0][i];
  }
  printf("Thread 1 tid = %lu\n", pthread_self());
}

void* sum_second_line(void* args) {
  for (int i = 0; i < 10; i++) {
    sum2 += arr[1][i];
  }
  printf("Thread 2 tid = %lu\n", pthread_self());
}

int main() {
  pthread_t first_thread;
  pthread_t second_thread;

  srand(time(NULL));

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 10; j++) {
      arr[i][j] = rand() % 60;
    }
  }

  if (pthread_create(&first_thread, NULL, sum_first_line, NULL)) {
    printf("Error creating thread!\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&second_thread, NULL, sum_second_line, NULL)) {
    printf("Error creating thread!\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(first_thread, NULL)) {
    printf("Error creating thread!\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(second_thread, NULL)) {
    printf("Error creating thread!\n");
    exit(EXIT_FAILURE);
  }

  printf("First line sum: %d\n", sum1);
  printf("Second line sum: %d\n", sum2);

  return 0;
}
