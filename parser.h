#ifndef __PARSER__
#define __PARSER__

#define MAX_COMMAND_INPUT 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

// Shell pid, pgid, terminal modes
static pid_t __GBSHPID;
static pid_t __GBSHPGID;
static int _GBS_INT;
static struct termios _GBSHTOMODES;
struct sigaction act_child;
struct sigaction act_int;
char correct_command[1024];
int prompt, n_stop, cant;

pid_t pid;

void signalHandler_child(int);
void signalHandler_int(int);
void initialize_shell();

void execute_command_typeA(char** );
void execute_command_typeB(char**, char** );
int analize_input(char*, char**, char** );
void find_commands(char**, char* );
int split_input(char*, char**,  int);
void process_input(char*);
void signalhandlerZ(int );
void signalhandlerC(int );
#endif
