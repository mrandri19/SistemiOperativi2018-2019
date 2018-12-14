#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define S() usleep(750 * 1000)

int main() {
  printf("P1\n");

  pid_t pid3 = fork();
  if (pid3 == 0) {
    printf("P3\n");
    pid_t pid6 = fork();
    if (pid6 == 0) {
      printf("P6\n");
    } else {
      waitpid(pid6, NULL, 0);
      printf("P8\n");
    }
  } else {
    printf("P2\n");
    pid_t pid5 = fork();
    if (pid5 == 0) {
      printf("P5\n");
    } else {
      printf("P4\n");

      waitpid(pid5, NULL, 0);
      printf("P7\n");

      waitpid(pid3, NULL, 0);
      printf("P9\n");
    }
  }

  return 0;
}