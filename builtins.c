#include "dz_shell.h"


// cd, cd [path], cd - (prev dir), cd ~, cd .., handling non existing dirs and permission issues
int command_cd(char** args, char* initial_directory) {
    (void) initial_directory; // not needed yet
    if(args[1] == NULL) {
        printf("cd expects path as an argument\n");
    } else if(chdir(args[1]) == 0) {
        printf("CD worked\n");
    } else {
        perror("CD failed");
    }
    return 0;
}


int command_pwd();
int command_echo(char** args, char** env);
int command_env(char** env);
int command_which(char** args, char** env);

char** command_setenv(char** args, char** env);
char** command_unsetenv(char** args, char** env);