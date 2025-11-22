#include "common.h"
#include "executor.h"
#include "my_helpers.h"

int test_get_path() {
  int passed = 0;
  int total = 0;

  // Test 1: PATH exists in env
  total++;
  {
    char *env[] = {"HOME=/home/user", "PATH=/usr/bin:/bin:/usr/local/bin",
                   "USER=testuser", NULL};
    char *result = get_path(env);
    if (result && my_strcmp(result, "/usr/bin:/bin:/usr/local/bin") == 0) {
      printf("Test 1 passed: PATH exists in env\n");
      passed++;
    } else {
      printf("Test 1 FAILED: PATH exists in env\n");
    }
    free(result);
  }

  // Test 2: PATH doesn't exist in env
  total++;
  {
    char *env[] = {"HOME=/home/user", "USER=testuser", NULL};
    char *result = get_path(env);
    if (result == NULL) {
      printf("Test 2 passed: PATH doesn't exist in env\n");
      passed++;
    } else {
      printf("Test 2 FAILED: PATH doesn't exist, expected NULL\n");
      free(result);
    }
  }

  // Test 3: empty PATH value
  total++;
  {
    char *env[] = {"HOME=/home/user", "PATH=", "USER=testuser", NULL};
    char *result = get_path(env);
    if (result && my_strcmp(result, "") == 0) {
      printf("Test 3 passed: empty PATH valu\n");
      passed++;
    } else {
      printf("Test 3 FAILED: empty PATH value\n");
    }
    free(result);
  }

  printf("\nget_path: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_split_paths() {
  int passed = 0;
  int total = 0;

  // Test 1: normal PATH with multiple directories
  total++;
  {
    int count;
    char *paths = "/usr/bin:/bin:/usr/local/bin";
    char **result = split_paths(paths, &count);

    if (count == 3 && result && my_strcmp(result[0], "/usr/bin") == 0 &&
        my_strcmp(result[1], "/bin") == 0 &&
        my_strcmp(result[2], "/usr/local/bin") == 0) {
      printf("Test 1 passed: normal PATH with multiple directories\n");
      passed++;
    } else {
      printf("Test 1 FAILED: normal PATH with multiple directories\n");
    }

    for (int i = 0; i < count; i++) {
      free(result[i]);
    }
    free(result);
  }

  // Test 2: single directory
  total++;
  {
    int count;
    char *paths = "/usr/bin";
    char **result = split_paths(paths, &count);

    if (count == 1 && result && my_strcmp(result[0], "/usr/bin") == 0) {
      printf("Test 2 passed: single directory\n");
      passed++;
    } else {
      printf("Test 2 FAILED: single directory\n");
    }

    free(result[0]);
    free(result);
  }

  // Test 3: NULL input
  total++;
  {
    int count;
    char **result = split_paths(NULL, &count);

    if (count == 0 && result == NULL) {
      printf("Test 3 passed: NULL input\n");
      passed++;
    } else {
      printf("Test 3 FAILED: NULL input\n");
    }
  }

  printf("\nsplit_paths: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int main() {
  int passed = 0;
  int total = 0;

  printf("Running executor tests...\n\n");

  total++;
  passed += test_get_path();
  printf("\n=========================\n");

  total++;
  passed += test_split_paths();
  printf("\n=========================\n");

  printf("Overall Executor: %d/%d test functions passed\n", passed, total);
  return (passed == total) ? 0 : 1;
}