#ifndef __PARSER__
#define __PARSER__

#define MAX_COMMAND_INPUT 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command_typeA(char** );
void execute_command_typeB(char**, char** );
int analize_input(char*, char**, char** );
void find_commands(char**, char* );
int split_input(char*, char**,  int);
void process_input(char*);

#endif
