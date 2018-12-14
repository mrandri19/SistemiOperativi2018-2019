#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  while (true) {
    int d;
    if (scanf("%d", &d) == EOF) break;
    if (d == 0) break;

    if (d % 2 == 0)
      fprintf(stdout, "%d\n", d);
    else
      fprintf(stderr, "%d\n", d);
  }
  return 0;
}

// rm -f out.txt err.txt &&clang -Wall -Werror -o main main.c && ./main <
// input.txt > out.txt 2> err.txt
