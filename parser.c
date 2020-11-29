#include "parser.h"

void execute_command_typeB(char** command_parsed, char** command_parsed_encap)
{

  pid_t process_1;
  pid_t process_2;
  int processes[2];

  if (pipe(processes) < 0) return;

  process_1 = fork();

  if (process_1 < 0) return;

  if (process_1 == 0)
  {

    close(processes[0]);
    dup2(processes[1], STDIN_FILENO);
    close(processes[1]);

    if (execvp(command_parsed[0], command_parsed) < 0)
    {
        printf("%s: command no found\n", command_parsed[0]);
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

        close(processes[1]);
        dup2(processes[0], STDIN_FILENO);
        close(processes[0]);

        if (execvp(command_parsed_encap[0], command_parsed_encap) < 0)
        {
          printf("%s: command no found\n", command_parsed_encap[0]);
          exit(EXIT_FAILURE);

        }

        else
        {

         wait(NULL);
         wait(NULL);

        }
        wait(NULL);
        wait(NULL);
     }

  }

}

void execute_command_typeA(char** command_parsed)
{

  pid_t new_proccess = fork();

  if (new_proccess == 1)
  {
    return;
  }
  else if (new_proccess == 0)
  {
      if ( execvp(command_parsed[0], command_parsed) < 0)
      {
          printf("%s: command no found\n", command_parsed[0]);
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
