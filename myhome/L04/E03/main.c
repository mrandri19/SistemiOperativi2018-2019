#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_BUF_LEN (unsigned long)1024
#define COMMANDS_BUF 128

void using_system(char *command) { system(command); }

void using_exec(char *command) {
  char *piece = calloc(1, LINE_BUF_LEN * sizeof(char));

  char **argv = calloc(COMMANDS_BUF, sizeof(char *));
  for (int i = 0; i < COMMANDS_BUF; ++i) {
    argv[i] = calloc(LINE_BUF_LEN, sizeof(char));
  }

  int argc = 0;
  piece = strtok(command, " ");
  strcpy(argv[argc++], piece);  // copy argv[0]

  while ((piece = strtok(NULL, " ")) != NULL) {
    strcpy(argv[argc++], piece);  // copy all the subsequent ones
  }
  argv[argc] = NULL;

  if (execvp(argv[0], argv) == -1) {
    perror("error executing command");
  };

  for (int i = 0; i < COMMANDS_BUF; ++i) {
    free(argv[i]);
  }
  free(argv);
}

int main(int argc, char const *argv[]) {
  FILE *fp = fopen("./input.txt", "r");

  if (fp == NULL) {
    perror("Cannot open input.txt");
    return 1;
  }

  unsigned long len = LINE_BUF_LEN;
  char *line = malloc(len * sizeof(char));
  while (getline(&line, &len, fp) != EOF) {
    // using_system(line);
    if (fork() == 0) {
      using_exec(line);
    }
  }

  fclose(fp);
  return 0;
}