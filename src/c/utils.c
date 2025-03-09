/** @file utils.c

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"


/* Free `dst` and return `src`. */
void *free_and_assign(void *const dst, void *const src) {
  ASSERT(dst);
  free(dst);
  return src;
}

/* Fork a bin and return status, or -1 on error.  Assign the output from the child to `output`. */
int fork_bin(const char *const __restrict path, char *const argv[], char *const envp[], char **const output) {
  ASSERT(path);
  ASSERT(argv);
  ASSERT(envp);
  char buffer[4096], *readret;
  long bytes_read, total_bytes_read = 0;
  int fdpipe[2], status, statusret = 0;
  pid_t pid;
  ALWAYS_ASSERT(pipe(fdpipe) != -1);
  ALWAYS_ASSERT((pid = fork()) != -1);
  /* Child process. */
  if (pid == 0) {
    /* Close child read fd. */
    close(fdpipe[0]);
    /* Redirect stdout and stderr to write fd of child . */
    dup2(fdpipe[1], STDOUT_FILENO);
    dup2(fdpipe[1], STDERR_FILENO);
    close(fdpipe[1]);
    /* Exec the binary with args and env. */
    ALWAYS_ASSERT(execve(path, argv, envp) != -1);
  }
  /* Parent process. */
  else {
    readret = xmalloc(sizeof(buffer));
    /* Close parent write fd. */
    close(fdpipe[1]);
    /* Read the output from the child. */
    while ((bytes_read = read(fdpipe[0], buffer, sizeof(buffer))) > 0) {
      readret = xrealloc(readret, (total_bytes_read + bytes_read + 1));
      memcpy((readret + total_bytes_read), buffer, bytes_read);
      total_bytes_read += bytes_read;
    }
    /* NULL-TERMINATE the data read, and close the parent read fd. */
    readret[total_bytes_read] = '\0';
    close(fdpipe[0]);
    /* If output ptr is not null, then assign the output from
     * the child to it.  Otherwise, we free the read data. */
    ASSIGN_IF_VALID_ELSE_FREE(output, readret);
    ALWAYS_ASSERT(waitpid(pid, &status, 0) != -1);
    if (WIFEXITED(status)) {
      statusret = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
      statusret = WTERMSIG(status);
    }
    else {
      statusret = -1;
    }
  }
  return statusret;
}

/* Create arguments array from a string. */
void construct_argv(char ***arguments, const char *command) {
  ASSERT(arguments);
  ASSERT(command);
  char *copy_of_command = copy_of(command);
  char *element = strtok(copy_of_command, " ");
  int count = 1;
  while (element) {
    (*arguments) = xrealloc(*arguments, (++count * sizeof(void *)));
    (*arguments)[count - 2] = copy_of(element);
    element = strtok(NULL, " ");
  }
  (*arguments)[count - 1] = NULL;
}

/* Free a NULL-TERMINATED char array where the `char *` inside the array are allocated as well. */
void free_nullterm_carray(char **array) {
  for (char **a = array; *a; ++a) {
    free(*a);
  }
  free(array);
}
