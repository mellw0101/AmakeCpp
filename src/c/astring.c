/** @file astring.c

  @author  Melwin Svensson.
  @date    9-2-2025.

 */
#include "../include/cproto.h"


/* Append `src` to the end of `dst`. */
char *astrcat(char *restrict dst, const char *const restrict src) {
  ASSERT(dst);
  ASSERT(src);
  /* Get the length of both dst and src. */
  Ulong dstlen = strlen(dst);
  Ulong srclen = strlen(src);
  /* Reallocate dst to fit all of the text plus a NULL-TERMINATOR. */
  dst = xrealloc(dst, (dstlen + srclen + 1));
  /* Append src to dst, and NULL-TERMINATE dst. */
  memcpy((dst + dstlen), src, srclen);
  dst[dstlen + srclen] = '\0';
  /* Then return dst. */
  return dst;
}

/* Inject `src` into `dst` at index `idx`. */
char *astrinj(char *restrict dst, const char *const restrict src, Ulong idx) {
  ASSERT(dst);
  ASSERT(src);
  /* Get length of both dst and src. */
  Ulong dstlen = strlen(dst);
  Ulong srclen = strlen(src);
  /* Always assert that idx is valid. */
  ALWAYS_ASSERT(idx < dstlen);
  /* Reallocate dst to fit src and a NULL-TERMINATOR. */
  dst = xrealloc(dst, (dstlen + srclen + 1));
  /* First move the data at idx by srclen, then copy src into dst at idx. */
  memmove((dst + idx + srclen), (dst + idx), (dstlen - idx));
  memcpy((dst + idx), src, srclen);
  return dst;
}
