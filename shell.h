#ifndef __SHELL__
#define __SHELL__

#include "alias.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


char* directory;
int stack_instructions(Alias*, char*);
void execute(Alias*, char*, int);
void execute_exit();
int is_own_commmand(char*);
int start_shell();
void print_address();

#endif
