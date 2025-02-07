#include <Mlib/FileSys.h>
#include <Mlib/Vector.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <filesystem>
#include <linux/limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

/* Amake */
#include "cproto.h"

/* clang-format off */
#ifdef Ulong
#    undef Ulong
#endif
#ifdef Uint
#    undef Uint
#endif
#ifdef Ushort
#    undef Ushort
#endif
#ifdef Uchar
#    undef Uchar
#endif
#define Ulong  unsigned long
#define Uint   unsigned int
#define Ushort unsigned short
#define Uchar  unsigned char

#define RW_FOR_ALL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#define __NOT_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))

#define ARG_MAX 256
#define ASM_COMPILER "nasm"
#define C_COMPILER  "clang"
#define CC_COMPILER "clang++"
#if defined(__x86_64__)
  #define ASM_DEFAULT_ARGS "-f elf64"
  #define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla"
  #define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla -mavx"
#elif defined(__aarch64__)
  #define ASM_DEFAULT_ARGS ""
  #define C_DEFAULT_ARGS "-m64 -funroll-loops -O3 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
  #define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -Rpass=loop-vectorize -flto -Wno-vla"
#endif
#define STATIC_STD "-static-libc++ -static-libgcc"

#define ARRAY_SIZE(array) (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))
#define ARGV(...) (const char *[]) { __VA_ARGS__ ,NULL }
#define PARENT_ENV ARGV("__parent_env")
#define FIND_EXEC(name) "find:" name

// [](void) noexcept -> const char ** { \
//   const char *array[ARRAY_SIZE(__VA_ARGS__) + 1]; \
//   Uint i = 0; \
//   for (; i < ARRAY_SIZE(array); ++i) {\
//     array[i] == __VA_ARGS__[i]; \
//   } \
//   array[i] = NULL; \
//   return array; \
// }()

#define ENSURE_CHARARRAY_CAPACITY(array, cap, size) \
  do {\
    if (size == cap) {\
      cap *= 2;\
      array = (char **)arealloc(array, (sizeof(char *) * cap)); \
    } \
  } while(0)

#define fs std::filesystem
using std::vector;

enum compile_type {
  C,
  CC
};

typedef struct {
  Uchar type;
  char file[ARG_MAX];
  char name[ARG_MAX];
  char ext[ARG_MAX];
  Uint file_len;
  Uint name_len;
  Uint ext_len;
} DirEntry;

typedef struct {
  char input[PATH_MAX];
  char output[PATH_MAX];
} ThreadData;

enum getArgsMode : Uchar {
  BUILDARGS = (1 << 0),
  LINKARGS  = (1 << 1),
};

typedef enum {
  CONFIG_HAVE___THREAD
  #define CONFIG_HAVE___THREAD CONFIG_HAVE___THREAD
} config_check_type;
