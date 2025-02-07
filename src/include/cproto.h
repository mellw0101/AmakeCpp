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
void Amake_make_data_dirs(void);

/* fd.c */
bool lock_fd(int fd, short type);
bool unlock_fd(int fd);

/* utils.c */
const char *tail(const char *const path);
const char *ext(const char *const path);
void *amalloc(Ulong howmush) __THROW _RETURNS_NONNULL;
void *arealloc(void *ptr, Ulong howmush) __THROW _RETURNS_NONNULL _NONNULL(1);
char *measured_copy(const char *const __restrict string, Ulong count) __THROW _RETURNS_NONNULL _NONNULL(1);
char *copy_of(const char *const __restrict string) __THROW _RETURNS_NONNULL _NONNULL(1);
char *fmtstr(const char *const __restrict format, ...) __THROW _RETURNS_NONNULL _NONNULL(1) _PRINTFLIKE(1, 2);
char *concatpath(const char *const __restrict s1, const char *const __restrict s2) __THROW _RETURNS_NONNULL _NONNULL(1, 2);
char *free_and_assign(char *const dst, char *const src);
int   fork_bin(const char *const __restrict path, char *const argv[], char *const envp[], char **const output) __THROW _NONNULL(1, 2, 3);
void  free_chararray(char **const array, Ulong len);
int   entries_in_dir(const char *path, char ***files, Ulong *nfiles, char ***dirs, Ulong *ndirs);
void  stdout_printf(const char *format, ...) _PRINTFLIKE(1, 2);
void  construct_argv(char ***arguments, const char *command);
bool  parse_num(const char *string, long *result);
char **split_string(const char *const string, const char delim);
void free_nullterm_carray(char **array);

/* dirs.c */
char *getpwd(void) __THROW _RETURNS_NONNULL;
char *getsrcdir(void) __THROW _RETURNS_NONNULL;
char *getcdir(void) __THROW _RETURNS_NONNULL;
char *getcppdir(void) __THROW _RETURNS_NONNULL;
char *getbuilddir(void) __THROW _RETURNS_NONNULL;
char *getoutdir(void) __THROW _RETURNS_NONNULL;
char *getamakedir(void) __THROW _RETURNS_NONNULL;
char *getamakecompdir(void) __THROW _RETURNS_NONNULL;
void  freepwd(void) __THROW;
void  freesrcdir(void) __THROW;
void  freecdir(void) __THROW;
void  freecppdir(void) __THROW;
void  freebuilddir(void) __THROW;
void  freeoutdir(void) __THROW;
void  freeamakedir(void) __THROW;
void  freeamakecompdir(void) __THROW;
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
void print_args(int argc, char **argv);

_END_C_LINKAGE
