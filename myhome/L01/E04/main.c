#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 256

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: ./copy src dest\n");
    return -1;
  }
  char* src = argv[1];
  char* dest = argv[2];
  printf("Source: %s, Dest: %s\n", src, dest);

  int src_fd = open(src, O_RDONLY);
  if (src_fd == -1) {
    perror("Coudln't open source file");
    return -1;
  }
  int dest_fd =
      open(dest, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (dest_fd == -1) {
    perror("Coudln't open dest file");
    if (close(src_fd) == -1) {
      perror("Coudln't close source file");
      return -1;
    }
    return -1;
  }

  while (1) {
    char buf[BUFSIZE] = {0};
    ssize_t res = read(src_fd, buf, BUFSIZE);
    if (res == -1) {
      perror("Error reading source");
      break;
    }
    if (res == 0) {
      printf("EOF reached\n");
      break;
    }
    res = write(dest_fd, buf, res);
    if (res == -1) {
      perror("Error reading source");
      break;
    }
    if (res == 0) {
      printf("EOF reached\n");
      break;
    }
  }

  if (close(src_fd) == -1) {
    perror("Coudln't close source file");
    return -1;
  }
  if (close(dest_fd) == -1) {
    perror("Coudln't close dest file");
    return -1;
  }
  return 0;
}