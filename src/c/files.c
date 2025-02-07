/** @file files.c

  @author  Melwin Svensson.
  @date    3-2-2025.

 */
#include "../include/cproto.h"

/* Return `TRUE` when path exitst and is file, otherwise, returns `FALSE` when file does not exist or path is not a file. */
bool file_exists(const char *const __restrict path) {
  ASSERT(path);
  struct stat st;
  if (access(path, R_OK) != 0) {
    return FALSE;
  }
  return (stat(path, &st) != -1 && !(S_ISDIR(st.st_mode) || S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)));
}

/* Allocate a given stat struct if not already allocated. */
void statalloc(const char *const __restrict path, struct stat **ptr) {
  ASSERT(path);
  ASSERT(ptr);
  /* If path does not exist, or is something other the a file, free it if its valid and return. */
  if (!file_exists(path)) {
    *ptr ? (free(*ptr), *ptr = NULL) : 0;
    return;
  }
  !*ptr ? (*ptr = amalloc(sizeof(**ptr))) : 0;
  ALWAYS_ASSERT(stat(path, *ptr) != -1);
}
