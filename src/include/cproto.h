/** @file cproto.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

/* Amake */
#include "cdef.h"

_BEGIN_C_LINKAGE

/* Amake.c */
void die(const char *format, ...) _NORETURN _NONNULL(1);
void Amake_do_compile(void);
void Amake_make_build_dirs(void);
void Amake_make_data_dirs(void);
void Amake_do_shallow_clean(void);

/* fd.c */
bool lock_fd(int fd, short type);
bool unlock_fd(int fd);

/* utils.c */
const char *tail(const char *const path);
const char *ext(const char *const path);
void *amalloc(Ulong howmush) __THROW _RETURNS_NONNULL;
void *arealloc(void *ptr, Ulong howmush) __THROW _RETURNS_NONNULL _NONNULL(1);
void *free_and_assign(void *const dst, void *const src);
int   fork_bin(const char *const __restrict path, char *const argv[], char *const envp[], char **const output) __THROW _NONNULL(1, 2, 3);
void  free_chararray(char **const array, Ulong len);
void  stdout_printf(const char *format, ...) _PRINTFLIKE(1, 2);
void  construct_argv(char ***arguments, const char *command);
bool  parse_num(const char *string, long *result);
void  free_nullterm_carray(char **array);

/* dirs.c */
char *getpwd(void) __THROW _RETURNS_NONNULL;
char *get_srcdir(void) __THROW _RETURNS_NONNULL;
char *get_cdir(void) __THROW _RETURNS_NONNULL;
char *get_cppdir(void) __THROW _RETURNS_NONNULL;
char *get_builddir(void) __THROW _RETURNS_NONNULL;
char *get_bindir(void) __THROW _RETURNS_NONNULL;
char *get_outdir(void) __THROW _RETURNS_NONNULL;
char *get_amakedir(void) __THROW _RETURNS_NONNULL;
char *get_amakecompdir(void) __THROW _RETURNS_NONNULL;
void  free_pwd(void) __THROW;
void  free_srcdir(void) __THROW;
void  free_cdir(void) __THROW;
void  free_cppdir(void) __THROW;
void  free_builddir(void) __THROW;
void  free_bindir(void);
void  free_outdir(void) __THROW;
void  free_amakedir(void) __THROW;
void  free_amakecompdir(void) __THROW;
void  free_dirptrs(void) __THROW;
bool  dir_exists(const char *const __restrict path) __THROW _NODISCARD _NONNULL(1);
directory_entry_t *directory_entry_make(void) __THROW _NODISCARD _RETURNS_NONNULL;
directory_entry_t *directory_entry_extract(directory_t *const dir, Ulong idx) __THROW _NODISCARD _RETURNS_NONNULL _NONNULL(1);
void  directory_entry_free(directory_entry_t *const entry) __THROW _NONNULL(1);
void  directory_data_init(directory_t *const dir) __THROW _NONNULL(1);
void  directory_data_free(directory_t *const dir) __THROW _NONNULL(1);
int   directory_get(const char *const __restrict path, directory_t *const output);
int   directory_get_recurse(const char *const __restrict path, directory_t *const output);
void  amkdir(const char *const __restrict path) __THROW _NONNULL(1);

/* files.c */
bool file_exists(const char *const __restrict path) __THROW _NODISCARD _NONNULL(1);
void statalloc(const char *const __restrict path, struct stat **ptr) __THROW _NONNULL(1, 2);

/* compile.c */
compile_data_entry_t *compile_data_entry_make(void);
void  compile_data_entry_free(compile_data_entry_t *entry);
void  compile_data_data_init(compile_data_t *const output);
void  compile_data_data_free(compile_data_t *const data);
void  compile_data_getc(compile_data_t *const output);
void  compile_data_getcpp(compile_data_t *const output);
void *compile_data_task(void *arg);

/* thread.c */
pthread_t *get_nthreads(Ulong howmeny);

/* args.c */
bool is_cmdopt(const char *arg, int *opt);
void test_args(int argc, char **argv);

/* signal.c */
void install_SIGINT_handler(void (*handler)(int));
void restore_SIGINT_handler(void);

/* astring.c */
char  *measured_copy(const char *const __restrict string, Ulong count) __THROW _RETURNS_NONNULL _NONNULL(1);
char  *copy_of(const char *const __restrict string) __THROW _RETURNS_NONNULL _NONNULL(1);
char **split_string(const char *const string, const char delim);
char  *fmtstr(const char *const __restrict format, ...) __THROW _RETURNS_NONNULL _NONNULL(1) _PRINTFLIKE(1, 2);
char  *concatpath(const char *const __restrict s1, const char *const __restrict s2) __THROW _RETURNS_NONNULL _NONNULL(1, 2);
char  *astrcat(char *__restrict dst, const char *const __restrict src) __THROW _RETURNS_NONNULL _NONNULL(1, 2);
char  *astrinj(char *__restrict dst, const char *const __restrict src, Ulong idx) __THROW _RETURNS_NONNULL _NONNULL(1, 2);

_END_C_LINKAGE
