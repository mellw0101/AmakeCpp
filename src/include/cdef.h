/** @file cdef.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

/* Amake */
#include "aassert.h"

/* stdlib */
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>

/* Linux */
#include <sys/stat.h>
#include <sys/wait.h>

/* fcio */
#include <fcio/proto.h>

/* Define c linkage macros. */
#ifdef __cplusplus
# define _BEGIN_C_LINKAGE  extern "C" {
# define _END_C_LINKAGE    }
#else
# define _BEGIN_C_LINKAGE
# define _END_C_LINKAGE
#endif

/* Define boolian macros. */
// #ifdef __cplusplus
// # define TRUE   true
// # define FALSE  false
// #else
// # define TRUE   1
// # define FALSE  0
// #endif

/* Integer define */
#undef Ulong
#undef Uint
#undef Ushort
#undef Uchar
#define Ulong   unsigned long int
#define Uint    unsigned int
#define Ushort  unsigned short int
#define Uchar   unsigned char

#ifndef _NORETURN
# define _NORETURN  __attribute__((__noreturn__))
#endif

#ifndef _RETURNS_NONNULL
# define _RETURNS_NONNULL  __attribute__((__returns_nonnull__))
#endif

#ifndef _NONNULL
# define _NONNULL(...)  __attribute__((__nonnull__(__VA_ARGS__)))
#endif

#ifndef _PRINTFLIKE
# define _PRINTFLIKE(...)  __attribute__((__format__(printf, __VA_ARGS__)))
#endif

#ifndef _NODISCARD
# define _NODISCARD __attribute__((__warn_unused_result__))
#endif

#ifndef _UNUSED
# define _UNUSED __attribute__((__unused__))
#endif

#define DO_WHILE(...)  do {__VA_ARGS__} while (0)

// #define ASSIGN_IF_VALID(ptr, value)            DO_WHILE( ((ptr) ? (*(ptr) = value) : ((int)0)); )
#define ASSIGN_IF_VALID_ELSE_FREE(ptr, value)  DO_WHILE( ((ptr) ? (void)(*(ptr) = value) : (free(value))); )

#define DEFAULT_C_COMPILER    "/usr/bin/clang"
#define DEFAULT_CPP_COMPILER  "/usr/bin/clang++"

#if defined(__x86_64__)
# define ASM_DEFAULT_ARGS "-f elf64"
# define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla"
# define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla -mavx"
#elif defined(__aarch64__)
# define ASM_DEFAULT_ARGS ""
# define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
# define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
#endif

// #define ENSURE_PTR_ARRAY_SIZE(array, cap, size)         \
//   DO_WHILE(                                             \
//     if (size == cap) {                                  \
//       cap *= 2;                                         \
//       array = arealloc(array, (sizeof(void *) * cap));  \
//     }                                                   \
//   )

// #define TRIM_PTR_ARRAY(array, cap, size)                    \
//   DO_WHILE(                                                 \
//     cap = (size + 1);                                       \
//     array = arealloc(array, (sizeof(void *) * cap));  \
//   )

/* Some defines for threading. */
#if !defined __cplusplus
/* Variables. */
# define thread  pthread_t
# define mutex   pthread_mutex_t
# define cond    pthread_cond_t

/* Callables. */
# define mutex_lock         pthread_mutex_lock
# define mutex_unlock       pthread_mutex_unlock
# define static_mutex_init  PTHREAD_MUTEX_INITIALIZER

/* Macros. */
# define under_mutex(mutex, action) \
  DO_WHILE(\
    mutex_lock(mutex); \
    DO_WHILE(action);\
    mutex_unlock(mutex); \
  )
#endif

#define S__LEN(str)  str, (sizeof(str) - 1)

#define ARRAY_SIZE(array)  (sizeof(array) / sizeof(array[0]))

typedef enum {
  AMAKE_HELP,
  #define AMAKE_HELP  AMAKE_HELP
  AMAKE_CONFIGURE,
  #define AMAKE_CONFIGURE  AMAKE_CONFIGURE
  AMAKE_VERSION,
  #define AMAKE_VERSION  AMAKE_VERSION
  AMAKE_BUILD,
  #define AMAKE_BUILD  AMAKE_BUILD
  AMAKE_CLEAN,
  #define AMAKE_CLEAN  AMAKE_CLEAN
  AMAKE_INSTALL,
  #define AMAKE_INSTALL  AMAKE_INSTALL
  AMAKE_LIB,
  #define AMAKE_LIB  AMAKE_LIB
  AMAKE_TEST,
  #define AMAKE_TEST  AMAKE_TEST
  AMAKE_LINK,
  #define AMAKE_LINK  AMAKE_LINK
  AMAKE_CHECK,
  #define AMAKE_CHECK  AMAKE_CHECK
} cmdopt_type_t;

/* Some structures. */
// typedef struct {
//   Uchar type;         /* The type of entry this is.  Uses `dirent->d_type`. */
//   char *name;         /* Name of the entry. */
//   char *path;         /* The full path of the entry. */
//   char *ext;          /* The extention, if any. */
//   char *clean_name;   /* When `name` has a extention, this is `name` without that extention, otherwise this is `NULL`. */
//   struct stat *stat;  /* Stat data for the entry. */
// } directory_entry_t;

// typedef struct {
//   directory_entry_t **entries;
//   Ulong cap;
//   Ulong len;
// } directory_t;

typedef struct {
  char *srcpath;                /* The full path to the source file of this entry. */
  char *outpath;                /* The full path to the output file of this entry. */
  char *compiler;               /* The compiler this entry will use to compile. */
  char *flags;                  /* Args this entry uses when compiling. */
  directory_entry_t *direntry;  /* The `directory_entry_t` that holds all data related to the entry. */
  bool compile_needed;          /* This is set to `TRUE` when this entry needs to be recompiled, otherwise `FALSE`. */
} compile_data_entry_t;

typedef struct {
  compile_data_entry_t **data;
  Ulong cap;
  Ulong len;
} compile_data_t;


