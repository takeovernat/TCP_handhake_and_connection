#ifndef __SHELL__
#define __SHELL__

#include "alias.h"
#include "parser.h"

int stack_instructions(Alias*, char*);
void execute(Alias*, char*, int);
void execute_exit();
int is_own_commmand(char*);
int start_shell();

#endif
