#include "common.h"

char generate_random_char() {
  const int MIN = 33;
  const int MAX = 125;

  return (char)(rand() % (MAX - MIN + 1) + MIN);
}

int main(int argc, char* argv[]) {
  pid_t pid = getpid();
  srand(time(NULL));

  long chars_number = strtol(argv[1], NULL, 10);

  FILE* f;
  char random_char;

  char generated_filename[20];
  sprintf(generated_filename, "we/we_%d.txt", pid);

  if ((f = fopen(generated_filename, "w")) == NULL) {
    perror("Error opening file \n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < chars_number; i++) {
    random_char = generate_random_char();
    fputc(random_char, f);
    write(1, (void*)&random_char, sizeof(char));
  }

  fclose(f);

  return EXIT_SUCCESS;
}
