#include "common.h"

int main(int argc, char* argv[]) {
  pid_t pid = getpid();
  FILE* f;

  char random_char;
  char filename[20];
  sprintf(filename, "wy/wy_%d.txt", pid);

  if ((f = fopen(filename, "w")) == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  while (read(0, (void*)&random_char, sizeof(char)) > 0) {
    fputc(random_char, f);
  }

  fclose(f);

  return EXIT_SUCCESS;
}
