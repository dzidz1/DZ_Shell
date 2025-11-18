#include "builtins.h"
#include "common.h"
#include "my_helpers.h"

char *capture_stdout_for_echo(char **args, char **env) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  int stdout_fd = dup(fileno(stdout));
  if (stdout_fd == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  if (dup2(pipefd[1], fileno(stdout)) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }
  close(pipefd[1]);

  command_echo(args, env);
  fflush(stdout);

  if (dup2(stdout_fd, fileno(stdout)) == -1) {
    perror("dup2 restore");
    exit(EXIT_FAILURE);
  }
  close(stdout_fd);

  char buffer[4096];
  ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
  if (n < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  close(pipefd[0]);

  buffer[n] = '\0';
  return my_strdup(buffer); // caller frees
}

int test_command_echo(char **env) {
  int total = 0;
  int passed = 0;

  // Test 1: echo hello world
  total++;
  {
    char *args[] = {"echo", "hello", "world", NULL};
    char *out = capture_stdout_for_echo(args, env);
    if (my_strcmp(out, "hello world\n") == 0) {
      printf("Test 1 passed: echo hello world\n");
      passed++;
    } else {
      printf("Test 1 FAILED: echo hello world, got: '%s'\n", out);
    }
    free(out);
  }

  // Test 2: echo -n hello
  total++;
  {
    char *args[] = {"echo", "-n", "hello", NULL};
    char *out = capture_stdout_for_echo(args, env);
    if (my_strcmp(out, "hello") == 0) {
      printf("Test 2 passed: echo -n hello\n");
      passed++;
    } else {
      printf("Test 2 FAILED: echo -n hello, got: '%s'\n", out);
    }
    free(out);
  }

  // Test 3: echo $PATH (i just check if it ends with newline)
  total++;
  {
    char *args[] = {"echo", "$PATH", NULL};
    char *out = capture_stdout_for_echo(args, env);
    int ok = 0;
    int len = my_strlen(out);
    if (len > 0 && out[len - 1] == '\n')
      ok = 1;
    if (ok) {
      printf("Test 3 passed: echo $PATH (ends with newline)\n");
      passed++;
    } else {
      printf("Test 3 FAILED: echo $PATH (no newline at the end), got: '%s'\n",
             out);
    }
    free(out);
  }

  printf("\ncommand_echo: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

char *capture_stdout_void(void (*fn)(void)) {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  int stdout_fd = dup(fileno(stdout));
  if (stdout_fd == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  if (dup2(pipefd[1], fileno(stdout)) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }
  close(pipefd[1]);

  fn();
  fflush(stdout);

  if (dup2(stdout_fd, fileno(stdout)) == -1) {
    perror("dup2 restore");
    exit(EXIT_FAILURE);
  }
  close(stdout_fd);

  char buffer[4096];
  ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
  if (n < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  close(pipefd[0]);

  buffer[n] = '\0';
  return my_strdup(buffer); // caller frees
}

// wrapping in order to use capture_stdout_void with command_pwd()
static void pwd_wrapper(void) { command_pwd(); }

int test_command_pwd(void) {
  int total = 0;
  int passed = 0;

  // saving original cwd so we can restore it at the end
  char *original_cwd = getcwd(NULL, 0);
  if (original_cwd == NULL) {
    perror("getcwd");
    return 0; // tests failed
  }

  // Test 1: pwd prints the current directory
  total++;
  {
    char *expected = getcwd(NULL, 0);
    char *output = capture_stdout_void(pwd_wrapper);

    // strip trailing newline from output
    int len = my_strlen(output);
    if (len > 0 && output[len - 1] == '\n') {
      output[len - 1] = '\0';
    }

    if (expected != NULL && my_strcmp(expected, output) == 0) {
      printf("Test 1 passed: pwd prints current directory\n");
      passed++;
    } else {
      printf("Test 1 FAILED: pwd\nExpected: '%s'\nGot:     '%s'\n",
             expected ? expected : "(null)", output ? output : "(null)");
    }

    free(expected);
    free(output);
  }

  // Test 2: after chdir to "/", pwd should print "/"
  total++;
  {
    int ch = chdir("/");
    if (ch != 0) {
      perror("chdir to /");
      printf("Test 2 FAILED: could not chdir to '/'\n");
    } else {
      char *output = capture_stdout_void(pwd_wrapper);

      int len = my_strlen(output);
      if (len > 0 && output[len - 1] == '\n') {
        output[len - 1] = '\0';
      }

      if (my_strcmp(output, "/") == 0) {
        printf("Test 2 passed: pwd after chdir to '/'\n");
        passed++;
      } else {
        printf("Test 2 FAILED: pwd after chdir to '/'\nExpected: '/'\nGot:     "
               "'%s'\n",
               output);
      }

      free(output);
    }
  }

  // restore original cwd
  if (chdir(original_cwd) != 0) {
    perror("restore chdir");
  }
  free(original_cwd);

  printf("\ncommand_pwd: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int main(int argc, char **argv, char **env) {
  (void)argc;
  (void)argv;

  int passed = 0;
  int total = 0;

  printf("Running tests...\n\n");

  total++;
  passed += test_command_echo(env);
  printf("\n=========================\n");

  total++;
  passed += test_command_pwd();
  printf("\n=========================\n");

  printf("Overall Builtins: %d/%d test functions passed\n", passed, total);
  return (passed == total) ? 0 : 1;
}