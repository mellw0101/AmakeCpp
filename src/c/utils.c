/** @file utils.c

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"


/* The static mutex that `stdout_printf` uses. */
static mutex stdout_printf_mutex = static_mutex_init;


/* Allocate a block of memory with size `howmush`. */
void *amalloc(Ulong howmush) {
  void *block = malloc(howmush);
  ALWAYS_ASSERT(block);
  return block;
}

/* Reallocate `ptr` with size `howmush`. */
void *arealloc(void *ptr, Ulong howmush) {
  ptr = realloc(ptr, howmush);
  ALWAYS_ASSERT(ptr);
  return ptr;
}

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
    readret = amalloc(sizeof(buffer));
    /* Close parent write fd. */
    close(fdpipe[1]);
    /* Read the output from the child. */
    while ((bytes_read = read(fdpipe[0], buffer, sizeof(buffer))) > 0) {
      readret = arealloc(readret, (total_bytes_read + bytes_read + 1));
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

/* Free the memory of the given array, which should contain len elements. */
void free_chararray(char **const array, Ulong len) {
  if (!array) {
    return;
  }
  while (len > 0) {
    free(array[--len]);
  }
  free(array);
}

/* Print something to stdout, in a thread safe manner. */
void stdout_printf(const char *format, ...) {
  ASSERT(format);
  char   *ret;
  int     len;
  va_list ap, dummy;
  /* First get the length we need to allocate. */
  va_start(dummy, format);
  ALWAYS_ASSERT((len = vsnprintf(NULL, 0, format, dummy)) != -1);
  va_end(dummy);
  /* Allocate the return ptr. */
  ret = amalloc(len + 1);
  /* Then format the string into ret. */
  va_start(ap, format);
  ALWAYS_ASSERT(vsnprintf(ret, (len + 1), format, ap) != -1);
  va_end(ap);
  mutex_lock(&stdout_printf_mutex);
  {
    lock_fd(STDOUT_FILENO, F_WRLCK);
    write(STDOUT_FILENO, ret, len);
    unlock_fd(STDOUT_FILENO);
  }
  mutex_unlock(&stdout_printf_mutex);
  free(ret);
}

/* Create arguments array from a string. */
void construct_argv(char ***arguments, const char *command) {
  ASSERT(arguments);
  ASSERT(command);
  char *copy_of_command = copy_of(command);
  char *element = strtok(copy_of_command, " ");
  int count = 1;
  while (element) {
    (*arguments) = arealloc(*arguments, (++count * sizeof(void *)));
    (*arguments)[count - 2] = copy_of(element);
    element = strtok(NULL, " ");
  }
  (*arguments)[count - 1] = NULL;
}

/* Parse a number from a string. */
bool parse_num(const char *string, long *result) {
  long value;
  char *excess;
  /* Clear errno so we can check it after. */
  errno = 0; 
  value = strtol(string, &excess, 10);
  if (errno == ERANGE || !*string || *excess) {
    return FALSE;
  }
  *result = value;
  return TRUE;
}

/* Free a NULL-TERMINATED char array where the `char *` inside the array are allocated as well. */
void free_nullterm_carray(char **array) {
  for (char **a = array; *a; ++a) {
    free(*a);
  }
  free(array);
}
