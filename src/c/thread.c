/** @file thread.c

  @author  Melwin Svensson.
  @date    3-2-2025.

 */
#include "../include/cproto.h"

/* Returns a allocated array of `pthread_t` with size `howmeny`. */
thread *get_nthreads(Ulong howmeny) {
  return xmalloc(sizeof(thread) * howmeny);
}
