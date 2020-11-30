#include "parser.h"

pid_t  new_proccess;
pid_t process_1;
pid_t process_2;

void signalHandler_child(int p){

	while (waitpid(-1, NULL, WNOHANG) > 0);

}

void signalHandler_int(int p){

	if (kill(pid,SIGTERM) == 0){
		printf("\nProcess %d received a SIGINT signal\n",pid);
		prompt = 1;
	}
}

void signalhandlerC(int sig_num){

  signal(SIGINT, signalhandlerC);
  printf("\n");
  fflush(stdin);


	//printf("\n[%d][%d][%d]\n", SIGTSTP, sig_num, n);

}

/* Function to cancel a process when ctrl + z is pressed */
void signalhandlerZ(int sig_num){

	signal(SIGTSTP, signalhandlerZ);
	if (strlen(correct_command) > 0)
		printf("\n[%d]+  Stopped                 %s\n", n_stop, correct_command);
  fflush(stdin);

	kill(new_proccess, SIGKILL);

}
void initialize_shell() {

  __GBSHPID = getpid();
  _GBS_INT = isatty(STDIN_FILENO);

  if (_GBS_INT) {
    while (tcgetpgrp(STDIN_FILENO) != (__GBSHPGID = getpgrp()))
        kill(__GBSHPID, SIGTTIN);

    // Set the signal handlers for SIGCHILD and SIGINT
    //act_child.sa_handler = signalHandler_child;
    //act_int.sa_handler = signalHandler_int;

    sigaction(SIGCHLD, &act_child, 0);
    sigaction(SIGINT, &act_int, 0);

    setpgid(__GBSHPID, __GBSHPID);
    __GBSHPGID = getpgrp();
    if (__GBSHPID != __GBSHPGID) {
        printf("Error, the shell is not process group.");
        exit(EXIT_FAILURE);
    }

    tcsetpgrp(STDIN_FILENO, __GBSHPGID);
    tcgetattr(STDIN_FILENO, &_GBSHTOMODES);

  }
  else {

    printf("Failed to start shell.\n");
    exit(EXIT_FAILURE);
  }


}


void execute_command_typeB(char** command_parsed, char** command_parsed_encap)
{

  //pid_t process_1;
  //pid_t process_2;
  int processes[2];

  if (pipe(processes) < 0) return;

  process_1 = fork();

  if (process_1 < 0) return;

  if (process_1 == 0)
  {
		//signal(SIGINT, signalhandlerC);
    //signal(SIGTSTP, signalhandlerZ);
    close(processes[0]);
    dup2(processes[1], STDIN_FILENO);
    close(processes[1]);

    if (execvp(command_parsed[0], command_parsed) < 0)
    {
        printf("%s: command not found\n", command_parsed[0]);
        //kill(getpid(), SIGTERM);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

  }

  else
  {
      process_2 = fork();
      if (process_2 < 0) return;

      if (process_2 == 0)
      {
				//signal(SIGINT, signalhandlerC);
		    //signal(SIGTSTP, signalhandlerZ);
        close(processes[1]);
        dup2(processes[0], STDIN_FILENO);
        close(processes[0]);

        if (execvp(command_parsed_encap[0], command_parsed_encap) < 0)
        {
          printf("%s: command not found\n", command_parsed_encap[0]);
          //kill(getpid(), SIGTERM);
          exit(EXIT_FAILURE);
        }

        else
        {

         wait(NULL);
        }
        wait(NULL);
     }

  }

}

void execute_command_typeA(char** command_parsed)
{

  //pid_t
	 new_proccess = fork();

  if (new_proccess == 1)
  {
    return;
  }
  else if (new_proccess == 0)
  {
		//signal(SIGINT, signalhandlerC);
    //signal(SIGTSTP, signalhandlerZ);
      if ( execvp(command_parsed[0], command_parsed))
      {
          printf("%s: command not found\n", command_parsed[0]);
          //kill(getpid(), SIGTERM);
      }

      exit(EXIT_SUCCESS);
  }

  else {

      wait(NULL);
      return;
  }
}

void find_commands(char** command_parsed, char* sub_input)
{

  int i = 0;
  char sep[] = " ";

  do {

    command_parsed[i] = strsep( & sub_input, " ");
    if (command_parsed[i] == NULL) break;
    else if ( !strlen(command_parsed[i]) ) i--;

    i++;

  } while ( 1 );

}

int split_input(char* input, char** input_pipe,  int n_pipe)
{

  for (int i = 0; i < n_pipe; i++)
  {
      input_pipe[i] == strsep( & input, "|");
      if (input_pipe[i] == NULL)
      {
        break;
      }
  }

  return !(input_pipe[1] == NULL);

}

int analize_input(char* input, char** command_parsed, char**  command_parsed_encap )
{

  int res, i, n_pipe = 2;
  char* input_pipe[n_pipe];


  res =  split_input(input, input_pipe, n_pipe);
  if ( res == 1 )
  {
    find_commands(command_parsed, input_pipe[0]);
    find_commands(command_parsed_encap, input_pipe[1]);

  }

  else
  {
      find_commands(command_parsed, input);
  }

  return (res + 1);

}

void process_input(char* cinput)
{

  int arguments;
  char input[MAX_COMMAND_INPUT];
  char* command_parsed[MAX_COMMAND_INPUT];
  char* command_parsed_encap[MAX_COMMAND_INPUT];

  strcpy(input, cinput);

  arguments = analize_input(input, command_parsed,  command_parsed_encap);


  if (arguments == 1)
  {

      execute_command_typeA(command_parsed);
  }

  else if (arguments == 2)
  {
      execute_command_typeB(command_parsed, command_parsed_encap);
  }

  else
  {
    printf("Error....\n");
  }

}
