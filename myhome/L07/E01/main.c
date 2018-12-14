#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// void gen_binary_numbers(int pos, int n, int *vec) {
//   if (pos == n) {
//     for (int i = 0; i < n; ++i) {
//       printf("%d", vec[i]);
//     }
//     printf("\n");
//     return;
//   }

//   vec[pos] = 0;
//   gen_binary_numbers(pos + 1, n, vec);
//   vec[pos] = 1;
//   gen_binary_numbers(pos + 1, n, vec);
// }

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage %s n\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int n = atoi(argv[1]);
  int *vec = calloc(n, sizeof(int));

  for (int i = 0; i < n; ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      vec[i] = 0;
    } else {
      vec[i] = 1;
    }
  }

  for (int i = 0; i < n; ++i) {
    printf("%d", vec[i]);
  }
  printf("\n");

  return 0;
}