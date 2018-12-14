#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print_binary_file(int fd, int binary_file_len) {
  // start at offset 1 because otherwise the length will be read
  lseek(fd, 1 * sizeof(int), SEEK_SET);

  printf("[");
  for (size_t i = 0; i < binary_file_len; ++i) {
    int n;
    ssize_t bytes_read = read(fd, &n, sizeof(int));
    if (bytes_read != sizeof(int)) {
      printf("Read %ld bytes, expected 4\n", bytes_read);
      exit(1);
    }
    printf("%d, ", n);
  }
  printf("\b\b]\n");
}

int main(int argc, char const* argv[]) {
  int fd =
      open(argv[1],
           O_RDWR);  // Open with rw permissions for owner and read for others
  if (fd == 0) {
    perror("cannot open file");
  }

  int binary_file_len;
  ssize_t bytes_read = read(fd, &binary_file_len, sizeof(int));
  if (bytes_read != 4) {
    printf("Read %ld bytes, expected 4\n", bytes_read);
    return 1;
  }

  printf("len: %d\n", binary_file_len);
  printf("Before sorting\n\n");
  print_binary_file(fd, binary_file_len);

  for (int i = 0; i < binary_file_len - 1; ++i) {
    // All of the children pids
    size_t pids_len = binary_file_len - 1 - i;
    pid_t* pids = calloc(pids_len, sizeof(pid_t));

    for (int j = 0; j < (binary_file_len - 1 - i); ++j) {
      pids[j] = fork();

      // If it's a child
      // TODO: ho visto la soluzione, e' una cagata
      if (pids[j] == 0) {
        int v_j;
        if (lseek(fd, (j + 1) * sizeof(int), SEEK_SET) == -1) {
          perror("bad lseek");
          exit(1);
        };
        bytes_read = read(fd, &v_j, sizeof(int));
        if (bytes_read != 4) {
          printf("Read %ld bytes, expected 4\n", bytes_read);
          perror("cannot read v[j]");
          exit(1);
        }

        int v_jplus1;
        bytes_read = read(fd, &v_jplus1, sizeof(int));
        if (bytes_read != 4) {
          printf("Read %ld bytes, expected 4\n", bytes_read);
          perror("cannot read v[j+1]");
          exit(1);
        }

        ssize_t bytes_written = -100;
        if (v_j > v_jplus1) {
          int tmp = v_j;

          // v_j = v_jplus1;
          lseek(fd, (j + 1) * sizeof(int), SEEK_SET);
          bytes_written = write(fd, &v_jplus1, sizeof(int));
          if (bytes_written != 4) {
            printf("Wrote %ld bytes, expected 4\n", bytes_written);
            perror("cannot write to v[j]");
            exit(1);
          }

          // v_jplus1 = tmp;
          lseek(fd, ((j + 1) + 1) * sizeof(int), SEEK_SET);
          bytes_written = write(fd, &tmp, sizeof(int));
          if (bytes_written != 4) {
            printf("Wrote %ld bytes, expected 4\n", bytes_written);
            perror("cannot write to v[j+1]");
            exit(1);
          }
        }

        printf("\tProcess finished with j=%d\n", j);
        return 0;
      }
    }

    for (int j = 0; j < pids_len; j++) {
      waitpid(pids[j], NULL, 0);
    }
    printf("New for iteration with i=%d\n", i);
  }

  printf("\nAfter sorting\n");

  print_binary_file(fd, binary_file_len);

  close(fd);
  return 0;
}