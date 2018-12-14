#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

typedef struct {
  int n;
  char* in;
  char* out;
} args_t;

void* sayHello(void* args_) {
  args_t* args = (args_t*)args_;

  char* in_filename = calloc(strlen(args->in) + 1, sizeof(char));
  char* out_filename = calloc(strlen(args->out) + 1, sizeof(char));

  sprintf(in_filename, "%s%d.txt", args->in, args->n);
  sprintf(out_filename, "%s%d.txt", args->out, args->n);

  printf("%s\n", in_filename);
  printf("%s\n", out_filename);

  FILE* fp_in = fopen(in_filename, "r");
  if (fp_in == NULL) {
    perror("cannot open input file");
    return NULL;
  }
  FILE* fp_out = fopen(out_filename, "w+");
  if (fp_in == NULL) {
    perror("cannot open output file");
    return NULL;
  }

  int len = 0;
  fscanf(fp_in, "%d ", &len);

  int* v = calloc(len, sizeof(int));
  for (int i = 0; i < len; ++i) {
    fscanf(fp_in, "%d ", &v[i]);
  }

  for (int i = 0; i < len - 1; ++i) {
    for (int j = 0; j < len - i - 1; ++j) {
      if (v[j] > v[j + 1]) {
        int tmp = v[j];
        v[j] = v[j + 1];
        v[j + 1] = tmp;
      }
    }
  }
  for (int i = 0; i < len; ++i) {
    fprintf(fp_out, "%d\n", v[i]);
  }

  free(v);
  fclose(fp_in);
  fclose(fp_out);

  free(out_filename);
  free(in_filename);

  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Usage: %s n A B\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int N = atoi(argv[1]);

  pthread_t* tids = calloc(N, sizeof(pthread_t));

  // Separate data for separate threads
  args_t args[2] = {{.n = -1, .in = argv[2], .out = argv[3]},
                    {.n = -1, .in = argv[2], .out = argv[3]}};

  for (int i = 0; i < N; ++i) {
    // Only change a thread's own data
    args[i].n = i + 1;
    pthread_create(&(tids[i]), NULL, sayHello, (void*)&args[i]);
  }

  for (int i = 0; i < N; ++i) {
    pthread_join(tids[i], NULL);
  }

  free(tids);

  return 0;
}