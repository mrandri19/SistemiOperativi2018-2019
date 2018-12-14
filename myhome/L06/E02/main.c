#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 256
static char message[] = "something";

void child(int read_fd_a, int write_fd_a, int read_fd_b, int write_fd_b) {
  while (1) {
    char buf[BUF_SIZE] = {0};

    // Block on while reading
    read(read_fd_a, buf, BUF_SIZE);

    printf("I'm the child  (PID = %d)\n", getpid());
    sleep(1);
    write(write_fd_b, message, sizeof(message));
  }
}

int main() {
  int pipes_a[2];
  pipe(pipes_a);
  int read_fd_a = pipes_a[0];
  int write_fd_a = pipes_a[1];

  int pipes_b[2];
  pipe(pipes_b);
  int read_fd_b = pipes_b[0];
  int write_fd_b = pipes_b[1];

  pid_t child_pid = fork();
  if (child_pid == 0) {
    child(read_fd_a, write_fd_a, read_fd_b, write_fd_b);
    exit(EXIT_SUCCESS);
  }

  while (1) {
    printf("I'm the father (PID = %d)\n", getpid());
    sleep(1);
    write(write_fd_a, message, sizeof(message));
    char buf[BUF_SIZE] = {0};

    // Block on while reading
    read(read_fd_b, buf, BUF_SIZE);
  }

  waitpid(child_pid, NULL, 0);

  close(read_fd_a);
  close(write_fd_a);

  close(read_fd_b);
  close(write_fd_b);
  return 0;
}