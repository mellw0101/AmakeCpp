/** @file

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"

// /* Lock a file descriptor, note that this blocks until the lock can be accuried. */
// bool lock_fd(int fd, short type) {
//   ALWAYS_ASSERT(fd >= 0);
//   struct flock lock;
//   lock.l_type   = type;
//   lock.l_whence = SEEK_SET;
//   lock.l_start  = 0;
//   lock.l_len    = 0;
//   if (fcntl(fd, F_SETLKW, &lock) == -1) {
//     return FALSE;
//   }
//   return TRUE;
// }

// /* Unlock a file descriptor, this assumes you own the lock when called. */
// bool unlock_fd(int fd) {
//   ALWAYS_ASSERT(fd >= 0);
//   struct flock lock;
//   lock.l_type   = F_UNLCK;
//   lock.l_whence = SEEK_SET;
//   lock.l_start  = 0;
//   lock.l_len    = 0;
//   if (fcntl(fd, F_SETLK, &lock) == -1) {
//     return FALSE;
//   }
//   return TRUE;
// }
