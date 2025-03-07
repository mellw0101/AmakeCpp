/** @file cdef.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

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

#define ASSIGN_IF_VALID_ELSE_FREE(ptr, value)  DO_WHILE( ((ptr) ? (void)(*(ptr) = value) : (free(value))); )

#define DEFAULT_C_COMPILER    "/usr/bin/clang"
#define DEFAULT_CPP_COMPILER  "/usr/bin/clang++"

#if defined(__x86_64__)
# define ASM_DEFAULT_ARGS "-f elf64"
# define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla " \
                        "-std=gnu99 -flto=auto -fno-fat-lto-objects -Wextra -pedantic -Wno-unused-parameter -Wstrict-prototypes " \
                        "-Wshadow -Wconversion -Wvla -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute " \
                        "-Wmissing-prototypes -fsigned-char -fstack-protector-strong -Wno-conversion -fno-common -Wno-unused-result " \
                        "-Wimplicit-fallthrough -fdiagnostics-color=always -mavx -Wno-vla"
# define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla -mavx"
#elif defined(__aarch64__)
# define ASM_DEFAULT_ARGS ""
# define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
# define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
#endif

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


