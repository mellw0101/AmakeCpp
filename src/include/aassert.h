/** @file aassert.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

#define ASSERT_DEBUG

#define ASSERT_DO_WHILE(...)  do {__VA_ARGS__} while (0)

#define ASSERT_VOID_CAST       ((void)0)
#define ASSERT_DIE_CAST(expr)  die("%s: LINE: %lu: FILE: %s: Assertion failed: %s\n", __func__, __LINE__, __FILE__, #expr)

#define DO_ASSERT(expr)          \
  ASSERT_DO_WHILE(               \
    ((expr)                      \
      ? ASSERT_VOID_CAST         \
      : ASSERT_DIE_CAST(expr));  \
  )

#if defined(ASSERT_DEBUG)
# define ASSERT(expr)  DO_ASSERT(expr)
#else
# define ASSERT(expr)  ASSERT_VOID_CAST
#endif

#define ALWAYS_ASSERT(expr)  DO_ASSERT(expr)
