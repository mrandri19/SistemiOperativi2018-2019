#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void sigUsr(int signo) { return; }

int main(int argc, char const *argv[]) {
  if (signal(SIGUSR1, sigUsr) == SIG_ERR) {
    perror("dwa");
  }
  pid_t pid = fork();

  if (pid != 0) {
    // padre
    while (true) {
      pause();
      printf("Hello from father\n");
      sleep(1);

      kill(pid, SIGUSR1);
    }
  } else {
    // figlio
    pid = getppid();
    while (true) {
      sleep(1);
      kill(pid, SIGUSR1);

      pause();
      printf("Hello from child\n");
    }
  }

  return 0;
}