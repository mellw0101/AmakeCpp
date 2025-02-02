/** @file Amake.c

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"

/* When something fatal happens we call this to terminate. */
void die(const char *format, ...) {
  static int stabs = 0;
  /* When we die multiple times.  Just abandon the ship. */
  if (stabs++ != 0) {
    exit(11);
  }
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  exit(1);
}
