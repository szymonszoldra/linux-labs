#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int arr[2][10];
int sumOfTheNumbersInFirstLine = 0;
int sumOfTheNumbersInSecondLine = 0;

void* sumFirstLine(void* args) {
  for (int i = 0; i < 10; i++) {
    sumOfTheNumbersInFirstLine += arr[0][i];
  }
  printf("Watek 1 ma tid = %lu\n", pthread_self());
}

void* sumSecondLine(void* args) {
  for (int i = 0; i < 10; i++) {
    sumOfTheNumbersInSecondLine += arr[1][i];
  }
  printf("Watek 2 ma tid = %lu\n", pthread_self());
}

int main() {
  pthread_t firstLineThread;
  pthread_t secondLineThread;

  srand(time(NULL));

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 10; j++) {
      arr[i][j] = rand() % 60;
    }
  }

  if (pthread_create(&firstLineThread, NULL, sumFirstLine, NULL)) {
    printf("Blad tworzenia watku!\n");
    exit(1);
  }

  if (pthread_create(&secondLineThread, NULL, sumSecondLine, NULL)) {
    printf("Blad tworzenia watku!\n");
    exit(1);
  }

  if (pthread_join(firstLineThread, NULL)) {
    printf("Blad przylaczania watku!\n");
    exit(1);
  }

  if (pthread_join(secondLineThread, NULL)) {
    printf("Blad przylaczania watku!\n");
    exit(1);
  }

  printf("Suma liczb w pierwszej lini: %d\n", sumOfTheNumbersInFirstLine);
  printf("Suma liczb w drugiej lini: %d\n", sumOfTheNumbersInSecondLine);

  return 0;
}
