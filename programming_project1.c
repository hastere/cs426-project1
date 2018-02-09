#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

char **history;
int historycount = 0;
int should_run;
int andFlag;

char *readInput(void);
char **breakingUpIsHardToDo(char *);
void addToHistory(char *);
char **determineHistory(char **);
void display(void);
char *putUTogether(char **);
char **clearAmpersand(char **);
//gcc -Wall -g -std=c99 programming_project1.c -o programming_project1
int main(void) {
  char **args; /* command line arguments */
  char *input;
  andFlag = 0;

  //Allocate/Initialize History
  history = malloc(sizeof(char *) * 10);
  for (int i = 0; i < 10; ++i) {
    history[i] = "\0";
  }
  should_run = 1; /* flag to determine when to exit program */

  while (should_run) {
    printf("osh>");
    fflush(stdout);
    //Read in from commandline
    input = readInput();
    //break into arguments
    args = breakingUpIsHardToDo(input);
    //determine if they are special arguements
    args = determineHistory(args);
    //clear the ampersand
    args = clearAmpersand(args);
    if (should_run == 0)
      return 1;
    //Begin forking logic
    pid_t pid;
    pid = fork();
    if (pid < 0) {
      fprintf(stderr,"\nCannot Fork Child\n");
      exit(-1);
    }
    else if (pid == 0) {
      execvp(args[0], args);
      return 0;
      }
    else {
       if (andFlag == 0)
        wait(NULL);
    }
    //reset andFlag
  }
  return 0;
}

char **clearAmpersand(char **args) {
  int i;
  //looks for &, and removes it from args
  for (i = 0; args[i] != NULL; ++i) {
    if (strcmp(args[i], "&") == 0) {
      andFlag = i;
    }
  }
  if (andFlag != 0) {
    i = andFlag;
    while (args[i] != NULL && i < (MAX_LINE - 1)) {
      args[i] = args[i + 1];
      ++i;
    }
    args[i] = NULL;
  }

  return args;
}

void addToHistory(char *input) {
  //for the initial value, just place in first spot
  if (history[0] == '\0')
      history[0] = input;
  //else, move values down array until first spot is freed
  else {
    for (int i = 8; i > -1; --i) {
      history[i+1] = history[i];
    }
    history[0] = input;
  }
  //update count of commands
  ++historycount;
}

char **determineHistory(char **args) {
  int historyNo;
  char *command;

  //if history, display history
  if (strcmp(args[0], "history") == 0) {
    display();
  }
  //if !!, replace with last command if it exists
  else if (args[0][0] == '!' && args[0][1] =='!') {
    if (history[0] == '\0') {
      fprintf(stderr,"\nNo Commands in History\n");
      exit(-1);
    }
    args = breakingUpIsHardToDo(history[0]);
    addToHistory(history[0]);
  }
  //if !, check for validity and then replace args with args from history
  else if (args[0][0] == '!') {
    if (!isdigit(args[0][1])) {
      fprintf(stderr,"\nInvalid Digit. Check that an Integer is used.\n");
      exit(-1);
    }
    historyNo = args[0][1] - '0';
    --historyNo;
    if (historyNo > 9) {
      fprintf(stderr,"\nInvalid Digit. Check that Integer is <= 10\n");
      exit(-1);
    }
    else if (history[historyNo][0] == '\0') {
      fprintf(stderr,"\nInvalid Digit. Not Enough Commands in History\n");
      exit(-1);
    }
    else {
    args = breakingUpIsHardToDo(history[historyNo]);
    addToHistory(history[historyNo]);
    }
  }
  //if exit, exit
  else if(strcmp(args[0], "exit") == 0) {
    printf("should exit....\n");
    should_run = 0;
  }
  //else, put args together and add to history
  else {
    command = putUTogether(args);
    addToHistory(command);
  }
  return args;
}

char *putUTogether(char **args) {
  //puts args into a single line
  char *togetherAgain = malloc(sizeof(char) * MAX_LINE);
  strcat(togetherAgain, args[0]);
  for(int i = 1; args[i] != NULL; ++i) {
    strcat(togetherAgain, " ");
    strcat(togetherAgain, args[i]);

  }
  strcat(togetherAgain, "\0");
  return togetherAgain;
}

char *readInput(void) {
  //reads input, one character at a time.
  char *input = malloc(sizeof(char) * MAX_LINE);
  int i = 0;
  andFlag = 0;
  i = 0;
  char readChar = getchar();
  while (readChar != '\n' && i < MAX_LINE) {
    input[i] = readChar;
    readChar = getchar();
    ++i;
  }

  input[i] = '\0';
  return input;
}

char **breakingUpIsHardToDo(char *input) {
  //breaks line of args into individual arguments, places into 2d array;
  int i;
  char **daddy = malloc(sizeof(char *) * MAX_LINE);
  //intiialize
  for (i = 0; i < MAX_LINE; ++i) {
    daddy[i] = malloc(sizeof(char) * MAX_LINE);
  }

  //fill
  i = 0;
  char *token = strtok(input," \t\n");
  while (token != NULL) {
    strcpy(daddy[i],token);
    token = strtok (NULL, " \t\n");
    ++i;
  }
  daddy[i] = NULL;

   return daddy;
}

void display() {
  //walk history array, print commands
  int count = historycount;
  int i = 9;
  while (i > -1) {
      if (strcmp(history[i], "\0") == 0) {
        --i;
      }
      else {
        printf("%d.\t", count);
        printf("%s\n", history[i]);
        --i;
        --count;
      }
  }
}
