#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT 1024

char** parse_input(char* input);
void free_tokens(char** tokens);

int shell_builts(char** args, char** env, char* initial_directory);