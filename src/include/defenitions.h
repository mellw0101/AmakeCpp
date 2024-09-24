#include <Mlib/FileSys.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

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
#define C_COMPILER  "clang"
#define C_DEFAULT_ARGS "-m64 -stdlib=libc -funroll-loops -O3 -std=c23 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla"
#define CC_COMPILER "clang++"
#define CC_DEFAULT_ARGS "-m64 -stdlib=libc++ -funroll-loops -O3 -std=c++23 -static -Werror -Wall -march=native -Rpass=loop-vectorize -flto -Wno-vla"
#define STATIC_STD "-static-libc++ -static-libgcc"

enum compile_type
{
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
} compile_entry;

typedef struct {
    char input[PATH_MAX];
    char output[PATH_MAX];
} thread_data_t;
/* clang-format on */
