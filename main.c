#include "dz_shell.h"

// shell loop
// input parsing
// builtin commands execution
// execute external commands
// manage environmental variables
// manage path
// error handling


void shell_loop(char** env)
{
    char* input = NULL;
    size_t input_size = 0;

    char** args;

    while(1) {
        printf("<dz_shell> ");

        if(getline(&input, &input_size, stdin) == -1) {
            perror("getline");
            break; // EOF
        }

        // printf("Input: %s", input);

        args = parse_input(input);

        for(size_t i = 0; args[i] != NULL; i++) {
            printf("Args: %s", args[i]);
            printf("\n");
        }
    }
}


int main(int argc, char** argv, char** env)
{
    // dont need argc and argv
    (void)argc;
    (void)argv;

    shell_loop(env);

    return 0;

}