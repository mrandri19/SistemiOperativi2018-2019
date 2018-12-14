#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void recurse(int depth, int max_depth, int time) {
  if (depth == max_depth) {
    sleep(time);
    printf("bye\n");
    return;
  }
  // Fork and create the first child
  pid_t pid1 = fork();

  if (pid1 == 0) {
    // Child 1
    printf("Hello from child1, depth: %d\n", depth);
    printf("my pid is: %d\n", getpid());
    return recurse(depth + 1, max_depth, time);
  } else {
    // Father
    pid_t pid2 = fork();
    if (pid2 == 0) {
      // Child 2
      printf("Hello from child2, depth: %d\n", depth);
      printf("my pid is: %d\n", getpid());
      return recurse(depth + 1, max_depth, time);
    } else {
      // Father
      return;
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s n t \n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  int t = atoi(argv[2]);

  printf("max depth: %d, sleep time: %d\n", n, t);

  fflush(stdout);
  setbuf(stdout, 0);

  recurse(0, n, t);

  // Padre esegue 2 figli e termina

  // ogni figlio a sua volta esegue 2 figli e termina

  // farlo tante volte in modo da ottenere 2^n processi

  // ognuno di questi processi aspetta t secondi e poi termina

  return 0;
}