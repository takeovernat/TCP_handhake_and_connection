#include "alias.h"

void display_help_alias()
{

  printf("\tAvailable alias commands\n");
  printf("+ To display aliases: alias\n");
  printf("+ To create an alias: alias alias_name='command'\n");
  printf("+ To delete an alias: alias -r alias_name\n");
  printf("+ To delete all aliases: alias -c\n");

}

int search_alias(Alias* alias, char* w_alias)
{

  /*
    Function that performs the search for an alias in all
    aliases, if it finds it then returns the position in
    the list of aliases, otherwise it returns -1.
  */
  for (int i = 0; i < alias->n_list; i++) {
    for (int j = 0; j < alias->list[i].n_alias; j++) {
       if(!strcmp(alias->list[i].list_alias[j], w_alias))
        return i;
    }
  }
  return -1;
}

int delete_alias(Alias* alias, char* del_alias)
{
  /*
   Function that removes an alias if it exists in the alias list.
   returns 1 if the alias could be removed successfully; otherwise it returns 0
  */

  int ans, flag, k, i;
  ans = search_alias(alias, del_alias);

  if (ans != -1) {

    flag = 0, k = -1;
    for (i = 0; i < alias->list[ans].n_alias; i++) {

      if (k == -1 && !strcmp(alias->list[ans].list_alias[i], del_alias))
        k = i;

      else if (k != -1)
        strcpy(alias->list[ans].list_alias[k++], alias->list[ans].list_alias[i]);
    }

    alias->list[ans].n_alias--;
    if (alias->list[ans].n_alias == 0) {

      for (i = ans; i < alias->n_list - 1; i++) {
        alias->list[i] = alias->list[i + 1];
      }
      alias->n_list--;
    }
    return 1;
  }
  return 0;
}

int add_new_alias(Alias* alias, char* command, char* new_alias)
{

  /*
    Function that adds a new alias to the list of the given
    command, but with the condition that this new alias does
    not exist, and is not an alias of another command.
  */

  int ans,pos, n;
  ans = search_alias(alias, new_alias);
  if (ans == -1) {

    pos = -1;
    for (int i = 0; i < alias->n_list; i++) {
      if (!strcmp(alias->list[i].command, command)) {
          pos = i;
          break;
      }
    }

    if (pos != -1) {
      n = alias->list[pos].n_alias;
      if (n < MAX_ALIAS) {
        strcpy(alias->list[pos].list_alias[n], new_alias);
        alias->list[pos].n_alias++;
      }
    }
    else {
      n = alias->n_list;
      strcpy(alias->list[n].command, command);
      strcpy(alias->list[n].list_alias[0], new_alias);
      alias->list[n].n_alias = 1;
      alias->n_list++;
    }
    return 1;
  }

  return 0;
}

void delete_list_alias(Alias* alias)
{

  /* Function that removes the entire list of aliases */
  alias->n_list = 0;
  memset(alias, 0, sizeof(alias));

}

void display_list_alias(Alias* alias)
{

  /*
    Function that dislay all aliases of a command
  */

  if (alias->n_list > 0) {

    printf("Command list with aliases:\n");
    for (int i = 0; i < alias->n_list; i++) {
      printf("%s:\n", alias->list[i].command);
      for (int j = 0; j < alias->list[i].n_alias; j++) {
        printf("  %d. %s\n", j + 1, alias->list[i].list_alias[j]);
      }
    }
  }
  else
    printf("No aliases.\n");
}

void delete_special_character(char* string, int* has_equal)
{
    for (int i = 0; i < strlen(string); i++) {
      if (string[i] == '\'')
        string[i] = ' ';
      else if (string[i] == '=') {
        string[i] = ' ';
        *has_equal = 1;
      }
    }
}

int parse_alias_commands(Alias* alias, char* input)
{

  /*

    Main function of the alias, this function is in charge of
    analyzing a command and verifying if it is a correct
    command, if it is a correct command then it proceeds to
    execute it, otherwise it will show an error message.
  */

  int res, i, n = 0, has_equal = 0;
  char command[MAX_COMMAND_INPUT], command_alias[MAX_COMMAND_INPUT];
  char split_input[1000][MAX_COMMAND_INPUT];
  char *temp;

  strcpy(command, input);
  delete_special_character(command, &has_equal);

  temp  = strtok(command, " ");

  while (temp != NULL && n < 1000) {

    strcpy(split_input[n], temp);
    temp  = strtok(NULL, " ");
    n++;

  }
  if (n == 1 && !strcmp(split_input[0], "alias") ) {

    display_list_alias(alias);
    return 1;

  }
  else if (n == 2 && !strcmp(split_input[0], "alias") && !strcmp(split_input[1], "-c")) {

    delete_list_alias(alias);
    printf("Aliases deleted successfully.\n");

    return 1;

  }
  else if (n == 3 && !strcmp(split_input[0], "alias") && !strcmp(split_input[1], "-r")) {
    if (delete_alias(alias, split_input[2]))
        printf("%s: Alias successfully removed.\n",  split_input[2]);

    else
      printf("%s: alias no found.\n",  split_input[2]);

    return 1;

  }
  else if (n == 2 && !strcmp(split_input[0], "man") && !strcmp(split_input[1], "alias")) {

    display_help_alias();
    return 1;

  }
  else {

    if (has_equal && n >= 3) {

      strcpy(command_alias, split_input[1]);
      strcpy(command, "");
      for (i = 2; i < n; i++) {
        strcat(command, split_input[i]);
        if (i + 1 < n)
          strcat(command, " ");
      }
      res = add_new_alias(alias, command, command_alias);

      if (res)
        printf("Alias \"%s\" correctly assigning the command \"%s\".\n", command_alias, command);
      else
        printf("Error, the alias \"%s\" is already assigned.\n", command_alias);

      return 1;
    }

    else {

      int flag = 0;

      for (i = 0; i < n; i++) {
        if (!strcmp(split_input[i], "alias")) {
          flag = 1;
          break;
        }
      }

      if (flag) {
        printf("%s: command no found.\n", input);
        printf("Use \"man alias\" for help.\n");
        return 1;
      }
    }

  }

  return 0;

}

int is_alias(Alias* alias, char* input, char* sp)
{

  /*
    Function that checks if it is a command it is an
    alias, if it is an alias it returns 1, otherwise 0
  */
  int flag = 1, has = 0;
  char *split_input, *temp, *aux, *result;
  temp = (char*) malloc(sizeof(char) * strlen(input));
  aux = (char*) malloc(sizeof(char) * strlen(input));
  result = (char*) malloc(sizeof(char) * strlen(input));

  strcpy(result, "");
  strcpy(aux, input);
  split_input = strtok(aux, sp);

  while( split_input != NULL ) {
    strcpy(temp, split_input);

    if (flag == 1) {

      for (int i = 0; i < alias->n_list; i++) {
        for (int j = 0; j < alias->list[i].n_alias; j++) {
          if (!strcmp(alias->list[i].list_alias[j], temp)) {
            strcpy(temp, alias->list[i].command);
            has = 1;
            break;
          }
        }
      }
    }
    if (!strcmp(temp, "-r") && flag == 1) {
      flag = 0;
    }
    strcat(result, temp);
    split_input = strtok(NULL, sp);

    if (split_input != NULL)
     strcat(result, " ");
  }

  if(has)
    strcpy(input, result);


  return has;
}
