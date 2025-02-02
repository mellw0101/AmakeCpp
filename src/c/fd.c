/** @file

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"

bool lock_fd(int fd, short type) {
  ALWAYS_ASSERT(fd >= 0);
  struct flock lock = {
    type,
    SEEK_SET,
    0,
    0,
    getpid()
  };
  if (fcntl(fd, F_SETLKW, &lock) == -1) {
    return FALSE;
  }
  return TRUE;
}

bool unlock_fd(int fd) {
  ALWAYS_ASSERT(fd >= 0);
  struct flock lock = {
    F_UNLCK,
    SEEK_SET,
    0,
    0,
    getpid()
  };
  if (fcntl(fd, F_SETLK, &lock) == -1) {
    return FALSE;
  }
  return TRUE;
}
