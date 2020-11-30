
#include "shell.h"

/********************************************************************/

void print_address()
{

    char add[1024];
    getcwd(add, sizeof(add));
    printf("%s", add);
}

void get_directory() {
  printf("%s: ", directory);
}
void command_cd(char* path) {

  if( chdir(path) == -1)
    printf("cd: %s: No such file or directory\n", path);

}

/********************************************************************/

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


  if (flag == 0) {

    strcpy(temp, input);
    split = strtok(temp, " ");
    if (split != NULL) {

      if (!strcmp(split, "cd")) {
        split =  strtok(NULL, " ");
        if (split == NULL) {
          char*  home_user;
          home_user = (char*) malloc(sizeof(char) * 1024);
          strcpy(home_user, "/home/");
          strcat(home_user, getenv("USER"));
          command_cd(home_user);
        }

        else
          command_cd(split);
        flag = 2;
      }

      else if(!strcmp(split, "pwd")) {

        print_address();
        printf("\n");
        flag = 2;
      }
    }

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
  signal(SIGINT, signalhandlerC);
  signal(SIGTSTP, signalhandlerZ);

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
  prompt = 0, cant = 0;
  /*********************************************************/
  initialize_shell();
  printf("Do you want to customize your shell prompt for extra credit (Type y for yes): ");
  scanf("%s", input);

  directory = (char*) malloc(sizeof(char) * 1024);

  if (!strcmp(input, "y")) {

    getchar();
    printf("Type your customized shell prompt: ");
    fgets(directory, 1024, stdin);
    directory[strlen(directory) - 1] = '\0';

  }
  else {

    getchar();
    strcpy(directory, "prompt");
  }


  while (1) {

    get_directory();
    fgets(input, MAX_COMMAND_INPUT, stdin);
    input[strlen(input) - 1] = '\0';
    n_stop = 1;
    strcpy(correct_command, input);
    if (strcmp(input, "cd") == 0)
      chdir(input);


    ans = stack_instructions(alias, input);
    if (ans == 0)
      execute(alias, input, 1);

    else {


    }
  }

  start_shell();
  return 0;
}
