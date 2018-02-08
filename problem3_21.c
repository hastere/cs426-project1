#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool processOptions(int, char **);
bool isNumber(char *);
void collatz(int);

//compiled using gcc -Wall -g -std=c99 problem3_21.c -o problem3_21

int main(int argc,char **argv) {
    //Process Options from input, if true, execute collatz
  if (argc > 2 || argv[1][0] == '-') {
    printf("Error Processing Arguments. Please check to make sure you are");
    printf("inputting a valid positive integer\n");
  }

  for (int i; argv[1][i] != 0; i++) {
    if (!isdigit(argv[1][i])) {
      printf("Error Processing Arguments. Please check to make sure you are");
      printf("inputting a valid positive integer\n");
      return 1;
    }
  }

  int x = atoi(argv[1]);
  collatz(x);
  //print error if invalid input
  return 0;
}

//collatz sequence
void collatz(int x) {
  pid_t pid;
  int y = x;
  //fork process
  pid = fork();
  //error checking
  if (pid < 0) {
    printf("Error: Cannot Fork Child\n");
    return;
  }
  //run collatz
  else if (pid == 0) {
    printf("%d", y);
    if (y != 1) {
      while (y != 1) {
        if (y % 2 == 0)
          y = y / 2;
        else
          y = 3 * y + 1;
        printf(", %d", y);
      }
    }
    printf("\n");
  }
  //wait for child to finish
  else
    wait(NULL);
  return;
}
