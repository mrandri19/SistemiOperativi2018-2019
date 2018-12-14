#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 256

pid_t producer_pid, consumer_pid;

void producer(int write_fd) {
  printf("started producer\n");

  while (true) {
    pause();

    printf("$ ");
    char buf[BUF_SIZE] = {0};
    if (scanf("%s", buf) == EOF) {
      exit(EXIT_SUCCESS);
    }

    ssize_t bytes_written = write(write_fd, buf, strlen(buf));
    if (bytes_written == -1) {
      printf("write failed\n");
      exit(EXIT_FAILURE);
    }

    kill(consumer_pid, SIGUSR1);
  }
}

void consumer(int read_fd) {
  printf("started consumer\n");

  while (true) {
    pause();
    char buf[BUF_SIZE] = {0};
    ssize_t bytes_read = read(read_fd, buf, BUF_SIZE);
    if (bytes_read == -1) {
      exit(EXIT_FAILURE);
    }

    printf("%s\n", buf);
    kill(producer_pid, SIGUSR1);
  }
}

void SIGUSR1_handler(int signo) {
  if (signo != SIGUSR1) {
    printf("Unknown signal: %d received\n", signo);
  }
}

int main() {
  setbuf(stdout, 0);

  if (signal(SIGUSR1, SIGUSR1_handler) == SIG_ERR) {
    perror("cannot register signal handler");
    exit(EXIT_FAILURE);
  }

  int descriptors[2];

  if (pipe(descriptors) != 0) {
    perror("cannot open pipe");
    exit(EXIT_FAILURE);
  }

  int read_fd = descriptors[0];
  int write_fd = descriptors[1];

  producer_pid = fork();
  if (producer_pid == 0) {
    producer(write_fd);
    exit(EXIT_SUCCESS);
  }

  consumer_pid = fork();
  if (consumer_pid == 0) {
    consumer(read_fd);
    exit(EXIT_SUCCESS);
  }

  sleep(1);
  printf("started father\n");
  printf("sending kill\n");
  kill(producer_pid, SIGUSR1);

  waitpid(producer_pid, NULL, 0);
  waitpid(consumer_pid, NULL, 0);

  close(descriptors[0]);
  close(descriptors[1]);

  exit(EXIT_SUCCESS);
}