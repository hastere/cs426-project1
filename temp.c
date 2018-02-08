
void displayHistory() {
  int i;
  int j = 0;
  int histCount = count;

  //loop for iterating through commands
  for (i = 0; i<10;i++) {
      //command index
      printf("%d.  ", histCount);
      while (history[i][j] != '\n' && history[i][j] != '\0') {
	//printing command
          printf("%c", history[i][j]);
          j++;
      }
      printf("\n");
      j = 0;
      histCount--;
      if (histCount ==  0)
        break;
  }
  printf("\n");
}

//Fuction to get the command from shell, tokenize it and set the args parameter

int formatCommand(char inputBuffer[], char *args[],int *flag) {
  int length; // # of chars in command line
  int i;     // loop index for inputBuffer
  int start;  // index of beginning of next command
  int ct = 0; // index of where to place the next parameter into args[]
  int hist;
  //read user input on command line and checking whether the command is !! or !n

 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
  start = -1;
  if (length == 0)
    exit(0);   //end of command
  if (length < 0) {
    printf("Command not read\n");
    exit(-1);  //terminate
  }

  //examine each character
  for (i=0;i<length;i++) {
    switch (inputBuffer[i])  {
      case ' ':
      case '\t' :               // to seperate arguments
        if(start != -1) {
          args[ct] = &inputBuffer[start];
          ct++;
        }
        inputBuffer[i] = '\0'; // add a null char at the end
        start = -1;
        break;
      case '\n':                 //final char
        if (start != -1) {
          args[ct] = &inputBuffer[start];
          ct++;
        }
        inputBuffer[i] = '\0';
        args[ct] = NULL; // no more args
        break;
      default :
        if (start == -1)
          start = i;
        if (inputBuffer[i] == '&') {
          *flag  = 1; //this flag is the differentiate whether the child process is invoked in background
          inputBuffer[i] = '\0';
        }
    }
  }

  args[ct] = NULL; //if the input line was > 80

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
  for (i = 9;i>0; i--) //Moving the history elements one step higher
   	strcpy(history[i], history[i-1]);

    strcpy(history[0],inputBuffer); //Updating the history array with input buffer
    count++;
	if(count>10)
	  count=10;
}
