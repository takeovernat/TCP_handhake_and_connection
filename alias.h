
#ifndef __ALIAS__
#define __ALIAS__

#define MAX_ALIAS 1000
#define MAX_COMMAND_INPUT 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct command_alias {
  
  char list_alias[MAX_ALIAS][MAX_COMMAND_INPUT];
  char command[MAX_COMMAND_INPUT];
  int n_alias;

};

typedef struct command_alias command_alias;

struct alias {

  command_alias list[MAX_ALIAS];
  int n_list;

};

typedef struct alias Alias;

int search_alias(Alias*, char*);
int delete_alias(Alias*, char*);
int add_new_alias(Alias*, char*, char*);
void delete_list_alias(Alias* );
void display_list_alias(Alias* );
void display_help_alias();
void delete_special_character(char*, int*);
int parse_alias_commands(Alias*, char*);
int is_alias(Alias*, char*, char*);

#endif
