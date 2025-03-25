/** @file cproto.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

/* Amake */
#include "cdef.h"

_BEGIN_C_LINKAGE

/* Amake.c */
void die(const char *format, ...) _NO_RETURN _NONNULL(1);
void Amake_do_compile(void);
void Amake_do_link(int argc, char **argv);
void Amake_make_build_dirs(void);
void Amake_make_data_dirs(void);
void Amake_do_shallow_clean(void);

/* utils.c */
void *free_and_assign(void *const dst, void *const src);
int   fork_bin(const char *const restrict path, char *const argv[], char *const envp[], char **const output) __THROW _NONNULL(1, 2, 3);
void  construct_argv(char ***arguments, const char *command);
// bool  parse_num(const char *string, long *result);
void  free_nullterm_carray(char **array);
char *encode_slash_to_underscore(const char *const restrict string);

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
void  amkdir(const char *const __restrict path) __THROW _NONNULL(1);

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
char  *astrcat(char *__restrict dst, const char *const __restrict src) __THROW _RETURNS_NONNULL _NONNULL(1, 2);
char  *astrinj(char *__restrict dst, const char *const __restrict src, Ulong idx) __THROW _RETURNS_NONNULL _NONNULL(1, 2);

_END_C_LINKAGE
