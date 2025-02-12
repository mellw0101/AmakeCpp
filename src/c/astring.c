/** @file astring.c

  @author  Melwin Svensson.
  @date    9-2-2025.

 */
#include "../include/cproto.h"

/* Return a allocated copy of `string`. */
char *measured_copy(const char *const __restrict string, Ulong count) {
  ASSERT(string);
  char *ret = amalloc(count + 1);
  memcpy(ret, string, count);
  ret[count] = '\0';
  return ret;
}

/* Return a allocated copy of `string`. */
char *copy_of(const char *const __restrict string) {
  ASSERT(string);
  return measured_copy(string, strlen(string));
}

/* Split a string by demiliter. */
char **split_string(const char *const string, const char delim) {
  ASSERT(string);
  ASSERT(delim);
  /* Initilaze both start and end to string. */
  const char *start = string;
  const char *end   = string;
  /* Set up the return array. */
  Ulong  cap = 10;
  Ulong  len = 0;
  char **result = amalloc(sizeof(void *) * cap);
  /* Iterate until the end of the string. */
  while (*end) {
    /* Advance end until we se the delimiter. */
    while (*end && *end != delim) {
      ++end;
    }
    ENSURE_PTR_ARRAY_SIZE(result, cap, len);
    result[len++] = measured_copy(start, (end - start));
    /* Break if we reached the end of the string. */
    if (!*end) {
      break;
    }
    /* Advance end past all delim, for instance if delim
     * is ' ' then double space is just advanced past. */
    while (*end && *end == delim) {
      ++end;
    }
    /* Set start to the first char that is not delim. */
    start = end;
  }
  /* Trim the array before returning it, saving memory where we can. */
  TRIM_PTR_ARRAY(result, cap, len);
  result[len] = NULL;
  return result;
}

/* Return a allocated formatted string. */
char *fmtstr(const char *const __restrict format, ...) {
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
  return ret;
}

/* Concatate a path, taking into account trailing and leading '/' for a proper path. */
char *concatpath(const char *const __restrict s1, const char *const __restrict s2) {
  Ulong s1len = strlen(s1);
  /* If either s1 end with '/' or s2 starts with '/'. */
  if ((s1[s1len - 1] == '/' && *s2 != '/') || (s1[s1len - 1] != '/' && *s2 == '/')) {
    return fmtstr("%s%s", s1, s2);
  }
  /* When both s1 and s2 starts with '/'. */
  else if (s1[s1len - 1] == '/' && *s2 == '/') {
    return fmtstr("%s%s", s1, (s2 + 1));
  }
  /* And when niether s1 end with '/' or s2 starts with '/'. */
  else {
    return fmtstr("%s/%s", s1, s2);
  }
}

/* Append `src` to the end of `dst`. */
char *astrcat(char *__restrict dst, const char *const __restrict src) {
  ASSERT(dst);
  ASSERT(src);
  /* Get the length of both dst and src. */
  Ulong dstlen = strlen(dst);
  Ulong srclen = strlen(src);
  /* Reallocate dst to fit all of the text plus a NULL-TERMINATOR. */
  dst = arealloc(dst, (dstlen + srclen + 1));
  /* Append src to dst, and NULL-TERMINATE dst. */
  memcpy((dst + dstlen), src, srclen);
  dst[dstlen + srclen] = '\0';
  /* Then return dst. */
  return dst;
}

/* Inject `src` into `dst` at index `idx`. */
char *astrinj(char *__restrict dst, const char *const __restrict src, Ulong idx) {
  ASSERT(dst);
  ASSERT(src);
  /* Get length of both dst and src. */
  Ulong dstlen = strlen(dst);
  Ulong srclen = strlen(src);
  /* Always assert that idx is valid. */
  ALWAYS_ASSERT(idx < dstlen);
  /* Reallocate dst to fit src and a NULL-TERMINATOR. */
  dst = arealloc(dst, (dstlen + srclen + 1));
  /* First move the data at idx by srclen, then copy src into dst at idx. */
  memmove((dst + idx + srclen), (dst + idx), (dstlen - idx));
  memcpy((dst + idx), src, srclen);
  return dst;
}
