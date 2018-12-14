#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void traverse(char *path, int depth);

void traverse(char *relative_path, int depth) {
  char path[PATH_MAX + 1];
  // realpath(relative_path, path);
  strcpy(path, relative_path);

  // get info about path
  struct stat sb;
  if (stat(path, &sb) == -1) {
    printf("Error, path: %s\n", path);
    perror("Couldn't stat");
  };

  // if it's a file
  if (S_ISREG(sb.st_mode)) {
    printf("%*sf: %s\n", depth * 4, " ", path);
  }
  // if it's a directory
  if (S_ISDIR(sb.st_mode)) {
    printf("%*sd: %s\n", depth * 4, " ", path);

    // open it
    DIR *dh = opendir(path);
    if (dh == NULL) {
      perror("Couldn't opendir");
    }

    // iterate through the files in the directory
    struct dirent *dp;
    while ((dp = readdir(dh)) != NULL) {
      if (strcmp(".", dp->d_name) == 0 || strcmp("..", dp->d_name) == 0) {
        continue;
      } else {
        // Build the new path, the current path + / + the name of the directory
        char new_path[PATH_MAX + 1];
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, dp->d_name);

        traverse(new_path, depth + 1);
      }
    }

    // close it
    if (closedir(dh) == -1) {
      perror("Couldn't closedir");
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    perror("Usage ./r PATH");
    return -1;
  }
  traverse(argv[1], 0);

  return 0;
}
