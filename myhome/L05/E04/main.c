#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handler(int _) { return; }

int consumer(FILE *fp) {
  signal(SIGUSR1, handler);

  if (fp == NULL) {
    perror("cannot open file.txt");
    return 1;
  }

  while (true) {
    pause();

    char input[256];
    int out = fscanf(fp, "%s", input);

    printf("read: %s, out: %d", input, out);
  }

  fclose(fp);
  return 0;
}

int producer(FILE *fp) {
  signal(SIGUSR1, handler);

  if (fp == NULL) {
    perror("cannot open file.txt");
    return 1;
  }

  while (true) {
    char input[256];
    printf("> ");
    if (fscanf(stdin, "%s", input) == EOF) {
      printf("exiting\n");
      exit(1);
    }
    fprintf(fp, "%s", input);

    kill(0, SIGUSR1);

    printf("written %s to file\n", input);
  }

  fclose(fp);
  return 0;
}

int main(int argc, char const *argv[]) {
  signal(SIGUSR1, handler);
  FILE *fp = fopen("file.txt", "rw");
  if (fp == NULL) {
    perror("cannot open file");
    exit(1);
  }

  pid_t producer_pid = fork();
  if (producer_pid != 0) {
    // father
    pid_t consumer_pid = fork();
    if (consumer_pid != 0) {
      // father
      int exit_code;
      waitpid(producer_pid, &exit_code, 0);
      if (exit_code == 1) exit(1);
      waitpid(consumer_pid, &exit_code, 0);
      if (exit_code == 1) exit(1);
      return 0;
    } else {
      consumer(fp);
    }
  } else {
    producer(fp);
  }
}