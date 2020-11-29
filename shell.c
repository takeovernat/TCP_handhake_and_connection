
#include "shell.h"

int stack_instructions(Alias* alias, char* input)
{

  /*
    This function checks if the input has several commands, if
    so then it proceeds to execute them one by one, with the
    exception of "exit", this command is the last one to be executed.
  */

  int ans, n = 0, i, flag = 0, stack = 0, pending_exit;
  char* split, *temp;
  char* commands[MAX_COMMAND_INPUT];
  temp = (char*) malloc(sizeof(char) * strlen(input));
  strcpy(temp, input);

  split = strtok(temp, ";");

  while (split != NULL) {

    commands[n]  =(char*) malloc(sizeof(char) *  strlen(split));
    strcpy(commands[n], split);
    split = strtok(NULL, ";");
    n++;
  }

  if (n > 0) {

    pending_exit = stack = 0;
    for (i = 0; i < n; i++) {

      strcpy(temp, commands[i]);
      if (!strcmp(strtok(temp," "), "exit"))
        pending_exit = 1;
      else
        execute(alias, commands[i], stack);
    }

    if (pending_exit == 1) {
      stack = 1;
      execute(alias, "exit", stack);
    }
    flag = 1;

  }
  return flag;
}

int is_own_commmand(char* input)
{

  /*
    Function that checks if it is the exit command, if
    so then it returns 1 of it against 0.
  */

  int flag = 0;
  char *temp, *split;

  temp = (char*) malloc(sizeof(char) * strlen(input));
  strcpy(temp, input);
  split = strtok(temp, ";");

  while (split != NULL) {

    if (!strcmp(split, "exit") ) {
      flag = 1;
      break;
    }
    split = strtok(NULL, ";");
  }
  return flag;
}

void execute_exit()
{

  /*
    Function to finish the execution of the program
  */
  exit(EXIT_SUCCESS);
}

void execute(Alias* alias, char* input, int stack)
{

  /*
    Function to execute a command
  */
  
  int ans;

  ans = is_alias(alias, input, " ");
  ans = parse_alias_commands(alias, input);
  if (ans == 0) {

    ans = is_own_commmand(input);
    if (ans == 0)
      process_input(input);

    else if (ans == 1 && stack == 1)
      execute_exit();

    else {

    }
  }

  else {
    /*
      other proccess
    */
  }
}

int start_shell()
{

  /*********************************************************/
  char* input;
  input = (char*) malloc(sizeof(char) *  MAX_COMMAND_INPUT);
  Alias* alias;
  alias = (Alias*) malloc(sizeof(Alias));
  alias->n_list = 0;
  int ans;
  /*********************************************************/

  while (1) {

    printf("\n$ ");
    fgets(input, MAX_COMMAND_INPUT, stdin);
    input[strlen(input) - 1] = '\0';

    ans = stack_instructions(alias, input);
    if (ans == 0)
      execute(alias, input, 1);

    else {

    }

  }

  return 0;
}
