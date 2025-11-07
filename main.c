#include "dz_shell.h"

// shell loop
// input parsing
// builtin commands execution
// execute external commands
// manage environmental variables
// manage path
// error handling

int shell_builts(char** args, char** env, char* initial_directory) {

}


void shell_loop(char** env)
{
    char* input = NULL;
    size_t input_size = 0;

    char** args;

    char* initial_directory = getcwd(NULL, 0); // get current working directory

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

        // if(args[0] != NULL) {
        //     shell_builts(args, env, initial_directory);
        // }
        free_tokens(args);
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