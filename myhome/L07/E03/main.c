#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

typedef struct {
  int n;
  char* in;
} args_t;

typedef struct {
  size_t len;
  int* data;
} vec_t;

static vec_t m[2] = {{.len = 0, .data = NULL}, {.len = 0, .data = NULL}};

void* sayHello(void* args_) {
  args_t* args = (args_t*)args_;

  char* in_filename = calloc(strlen(args->in) + 1, sizeof(char));
  sprintf(in_filename, "%s%d.txt", args->in, args->n);

  FILE* fp_in = fopen(in_filename, "r");
  if (fp_in == NULL) {
    perror("cannot open input file");
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

  int p = args->n - 1;
  m[p].data = calloc(len, sizeof(int));
  m[p].len = len;

  for (int i = 0; i < len; ++i) {
    m[p].data[i] = v[i];
  }

  free(v);
  fclose(fp_in);
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

  // TODO: use n instead of 2

  // Separate data for separate threads
  args_t args[2] = {{.n = -1, .in = argv[2]}, {.n = -1, .in = argv[2]}};

  for (int i = 0; i < N; ++i) {
    // Only change a thread's own data
    args[i].n = i + 1;
    pthread_create(&(tids[i]), NULL, sayHello, (void*)&args[i]);
  }

  for (int i = 0; i < N; ++i) {
    pthread_join(tids[i], NULL);
  }

  for (int i = 0; i < 2; ++i) {
    size_t len = m[i].len;
    printf("m[%d] = \n", i);
    for (int j = 0; j < len; ++j) {
      printf("    %d\n", m[i].data[j]);
    }
  }

  int final_len = m[0].len + m[1].len;
  int* final = calloc(final_len, sizeof(int));

  // Merge the two
  int i = 0, j = 0, k = 0;

  while (i < m[0].len && j < m[1].len) {
    if (m[0].data[i] < m[1].data[j]) {
      final[k++] = m[0].data[i++];
    } else {
      final[k++] = m[1].data[j++];
    }
  }
  while (i < m[0].len) {
    final[k++] = m[0].data[i++];
  }
  while (j < m[1].len) {
    final[k++] = m[1].data[j++];
  }
  assert(i == m[0].len);
  assert(j == m[1].len);
  assert(k == final_len);

  printf("FINAL\n");
  for (int i = 0; i < final_len; ++i) {
    printf("%d\n", final[i]);
  }

  free(final);
  free(m[0].data);
  free(m[1].data);
  free(tids);

  return 0;
}