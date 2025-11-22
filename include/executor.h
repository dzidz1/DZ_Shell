#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <sys/wait.h>

int executor(char **args, char **env);
int child_process(char **args, char **env);
char *get_path(char **env);
char **split_paths(char *paths, int *count);

#endif