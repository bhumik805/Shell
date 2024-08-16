// shell.h
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **split_input(char *input_line);
void free_tokens(char **tokens);
void signal_handler(int sig);

extern int fore_pgid;
extern int back_num;
extern int back_pgid;

#endif // SHELL_H