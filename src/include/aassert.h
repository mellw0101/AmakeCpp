/** @file aassert.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

#define ASSERT_DEBUG

#define ASSERT_DO_WHILE(...)  do {__VA_ARGS__} while (0)

#define ASSERT_VOID_CAST                ((void)0)
#define ASSERT_DIE_CAST(expr)           die("%s: LINE: %lu: FILE: %s: Assertion failed: %s\n", __func__, __LINE__, __FILE__, #expr)
#define ASSERT_DIE_MSG_CAST(expr, msg)  die("%s: LINE: %lu: FILE: %s: Assertion failed: %s: %s\n", __func__, __LINE__, __FILE__, #expr, (msg))

#define DO_ASSERT(expr)          \
  ASSERT_DO_WHILE(               \
    ((expr)                      \
      ? ASSERT_VOID_CAST         \
      : ASSERT_DIE_CAST(expr));  \
  )

#define DO_ASSERT_MSG(expr, msg)          \
  ASSERT_DO_WHILE(                        \
    ((expr)                               \
      ? ASSERT_VOID_CAST                  \
      : ASSERT_DIE_MSG_CAST(expr, msg));  \
  )


#if defined(ASSERT_DEBUG)
# define ASSERT(expr)           DO_ASSERT(expr)
# define ASSERT_MSG(expr, msg)  DO_ASSERT_MSG(expr, msg)
#else
# define ASSERT(expr)           ASSERT_VOID_CAST
# define ASSERT_MSG(expr, msg)  ASSERT_VOID_CAST
#endif

#define ALWAYS_ASSERT(expr)           DO_ASSERT(expr)
#define ALWAYS_ASSERT_MSG(expr, msg)  DO_ASSERT_MSG(expr, msg)

