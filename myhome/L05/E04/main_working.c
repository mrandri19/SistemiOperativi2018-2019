#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void handle_SIGUSR1(int signo) {
  if (signo == SIGUSR1) {
    printf("received\n");
  } else {
    printf("unknown signal %d\n", signo);
  }
  return;
}

pid_t producer_pid, consumer_pid;

void producer() {
  printf("producer started\n");
  while (true) {
    fflush(stdout);
    pause();
    FILE *fp = fopen("scratchpad.txt", "w");
    if (fp == NULL) {
      perror("cannot open file");
      exit(1);
    }

    char str[256];
    printf("Insert a string: ");
    scanf("%s", str);
    fprintf(fp, "%s\n", str);
    printf("Sending message to consumer\n");
    kill(consumer_pid, SIGUSR1);
    fflush(stdout);
    fclose(fp);
  }

  printf("producer: Producer exiting\n");
  exit(0);
}

void consumer() {
  printf("consumer started\n");

  while (true) {
    fflush(stdout);
    pause();
    FILE *fp = fopen("scratchpad.txt", "r");
    if (fp == NULL) {
      perror("cannot open file");
      exit(1);
    }
    char str[256];
    fscanf(fp, "%s ", str);
    printf("consumer: read %s from file\n", str);
    for (int i = 0; i < strlen(str); ++i) {
      printf("%c", toupper(str[i]));
    }
    printf("\n");
    sleep(1);
    kill(producer_pid, SIGUSR1);

    fclose(fp);
  }

  printf("consumer: Consumer exiting\n");
  exit(0);
}

int main() {
  setbuf(stdout, 0);
  if (signal(SIGUSR1, handle_SIGUSR1) == SIG_ERR) {
    perror("cannot setup signal handler");
  }

  producer_pid = fork();
  if (producer_pid == 0) {
    producer();
  } else {
    consumer_pid = fork();
    if (consumer_pid == 0) {
      consumer();
    } else {
      // Father
      printf("Hello, from father\n");

      // sleep necessary for producer and consumer to be spawned before sending
      // signals, and for global variables producer_pid and consumer_pid to be
      // populated
      sleep(1);

      printf("sending kill\n");
      if (kill(producer_pid, SIGUSR1) == -1) {
        perror("cannot kill");
      };
      printf("sent kill\n");

      waitpid(producer_pid, NULL, 0);
      printf("father: Producer exited\n");
      waitpid(consumer_pid, NULL, 0);
      printf("father: Consumer exited\n");
      printf("father: Father exiting\n");
      exit(0);
    }
  }
}