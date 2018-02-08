#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

char **history;

char *readInput(void);
char **breakingUpIsHardToDo(char *, int *);
void addToHistory(char *);
void determineHistory(char **, int *);
void display(void);
//gcc -Wall -g -std=c99 programming_project1.c -o programming_project1
int main(void) {
  char **args; /* command line arguments */
  char *input;
  int andFlag = 0;
  history = malloc(sizeof(char *) * 10)
  for (int i = 0; i < 10; ++i) {
    history[i] = "\0";
  }
  int should_run = 1; /* flag to determine when to exit program */
  while (should_run) {
    printf("osh>");
    fflush(stdout);
    input = readInput();
    addToHistory(input);
    args = breakingUpIsHardToDo(input, &andFlag);
    determineHistory(args, &andFlag);

    // After reading user input, the steps are:
    // (1) fork a child process using fork()

    pid_t pid;
    pid = fork();
    if (pid < 0) {
      fprintf(stderr,"\nCannot Fork Child\n");
      exit(-1);
    }
    // (2) the child process will invoke execvp()
    else if (pid == 0)
      execvp(args[0], args);
    // (3) if command included &, parent will invoke wait()
    else {
      if (!andFlag)
        wait(NULL);
    }
  }
  return 0;
}

void addToHistory(char *input) {
  if (history[0] == '\0')
      history[0] == input;
  else {
    for (int i = 8; i > -1; --i) {
      history[i+1] = history[i];
    }
    history[0] = input;
  }
}

void determineHistory(char **args, int *andFlag) {
  int historyNo;
  if (strcmp(args[0], "history") == 0) {
    display();
  }
  if (args[0][0] == '!' && args[0][1] =='!')
    args = breakingUpIsHardToDo(history[0], andFlag);
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
    args = breakingUpIsHardToDo(history[historyNo], andFlag);
    addToHistory(history[historyNo]);
    }

  }
}

char *readInput(void) {
  char *input = malloc(sizeof(char) * MAX_LINE);
  int i = 0;

  i = 0;
  char readChar = getchar();
  while (readChar != '\n' && i < MAX_LINE) {
    input[i] = readChar;
    readChar = getchar();
    ++i;
  }
  //TODO
  /*if (!getchar()) {
    fprintf(stderr,"Command Size Too Long");
    exit(-1);
  }*/
  input[i] = '\0';
  return input;
}

char **breakingUpIsHardToDo(char *input, int *andFlag) {
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
   return daddy;
}

void display() {
  //walk array
  int i = 9;
  while (i > -1) {
      if (strcmp(history[i], "\0") == 0) {
        --i;
      }
      else {
        printf("%d.\t", i);
        printf("%s\n", history[i]);
        --i;
      }
  }
}
/*
void history(char *command) {

  if(strcmp(command,"history")==0) {
    if(count>0)
      display();
    else
        printf("No Commands\n");
  }
  // !! case
  else if (command[0] == '!' && command[1] == '!')
    strcpy(inputBuffer,history[0]);
  // !n case
  else if(command[0] == '!') {
    if (!isdigit(command[1] || !isdigit(command[2]))
      printf("Invalid format.\n");
    else {
      if (command[2] = '\0')
        strcpy(inputBuffer,history[count-x]);
      else
    }
      }
}
int formatCommand(char inputBuffer[], char *args[],int *flag) {
  int hist;
  //read user input on command line and checking whether the command is !! or !n

  int length; // # of chars in command line
 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
  start = -1;
  if (length == 0 || length > 80)
    exit(0);   //end of command

  char **cleanInput = clean(inputBuffer, length, &flag)
  if(strcmp(args[0],"history")==0) {
    if(count>0)
      displayHistory();
    else
		    printf("\nNo Commands in the history\n");
	  return -1;
    }
  else if (args[0][0]-'!' ==0) {
    int x = args[0][1]- '0';
		int z = args[0][2]- '0';

	  if(x>count) {
		  printf("\nNo Such Command in the history\n");
		  strcpy(inputBuffer,"Wrong command");
		}
	  else if (z!=-48) {
		  printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
		  strcpy(inputBuffer,"Wrong command");
	  }
	  else {
		  if(x==-15)
        strcpy(inputBuffer,history[0]);  // this will be your 10 th(last) command
		  else if(x==0) {
        printf("Enter proper command");
			  strcpy(inputBuffer,"Wrong command");
		  }
		  else if(x>=1)
			  strcpy(inputBuffer,history[count-x]);
		  }
	}
  int i;
  for (i = 9;i>0; i--) //Moving the history elements one step higher
   	strcpy(history[i], history[i-1]);

    strcpy(history[0],inputBuffer); //Updating the history array with input buffer
    count++;
	if(count>10)
	  count=10;
}
*/
