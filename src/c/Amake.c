/** @file Amake.c

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#include "../include/cproto.h"


/* When something fatal happens we call this to terminate. */
void die(const char *format, ...) {
  static int stabs = 0;
  /* When we die multiple times.  Just abandon the ship. */
  if (stabs++ != 0) {
    exit(11);
  }
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  exit(1);
}

/* Compile the project. */
void Amake_do_compile(void) {
  long      cores    = sysconf(_SC_NPROCESSORS_ONLN);
  Ulong     threadno = 0;
  thread_t *threads;
  /* Check if build dirs and the structure exists.  If not, create it. */
  Amake_make_build_dirs();
  /* Check if .amake dir for this project exists.  If not, create it. */
  Amake_make_data_dirs();
  /* Get all the entries we need to check if compalation is needed for. */
  compile_data_t data;
  compile_data_data_init(&data);
  compile_data_getc(&data);
  compile_data_getcpp(&data);
  threads = get_nthreads(data.len);
  /* Iterate over all entries, advancing by number of cores available. */
  for (Ulong i = 0; i < data.len; i += cores) {
    /* Calculate how meny threads to create. */
    threadno = (((i + cores) > data.len) ? (data.len - i) : cores);
    /* Create the threads with the task. */
    for (Ulong t = 0; t < threadno; ++t) {
      pthread_create(&threads[t + i], NULL, compile_data_task, data.data[t + i]);
    }
    /* Wait for all current threads to finish. */
    for (Ulong t = 0; t < threadno; ++t) {
      pthread_join(threads[t + i], NULL);
    }
  }
  free(threads);
  compile_data_data_free(&data);
}

void Amake_do_link(int argc, char **argv) {
  char *out;
  char *cmd = COPY_OF(DEFAULT_CPP_COMPILER " ");
  char **arguments;
  Ulong argslen = argc;
  directory_t dir;
  int i;
  bool bininst = FALSE;
  for (i=0; i<argc; ++i) {
    if (strcmp(argv[i], "--bin") == 0) {
      bininst = TRUE;
      break;
    }
  }
  if (bininst) {
    chararray_erase(argv, &argslen, i);
    argv[argslen] = NULL;
  }
  directory_data_init(&dir);
  ALWAYS_ASSERT(directory_get_recurse(get_outdir(), &dir) != -1);
  DIRECTORY_ITER(dir, dn, entry,
    cmd = fmtstrcat(cmd, "%s ", entry->path);
  );
  directory_data_free(&dir);
  writef("%s\n", cmd);
  arguments = split_string_len(cmd, ' ', &argslen);
  free(cmd);
  chararray_append(&arguments, &argslen, argv, argc);
  fork_bin(arguments[0], arguments, (char *[]){ NULL }, &out);
  writef("%s\n", out);
  free(out);
  chararray_free(arguments, argslen);
}

/* Create the build structure, so this can happen automaticly if user just cleaned the project. */
void Amake_make_build_dirs(void) {
  /* Make the main build dir for the project. */
  if (!dir_exists(get_builddir())) {
    amkdir(get_builddir());
  }
  /* Make the bin dir. */
  if (!dir_exists(get_bindir())) {
    amkdir(get_bindir());
  }
  /* Make the obj dir. */
  if (!dir_exists(get_outdir())) {
    amkdir(get_outdir());
  }
}

/* Create the `.amake` direcory and its subdirs Amake needs. */
void Amake_make_data_dirs(void) {
  if (!dir_exists(get_amakedir())) {
    amkdir(get_amakedir());
  }
  if (!dir_exists(get_amakecompdir())) {
    amkdir(get_amakecompdir());
  }
}

/* Do a simple clean, delete all object files, resulting in the need for recompalation off all source files. */
void Amake_do_shallow_clean(void) {
  /* If the output directory does not exist, just return. */
  if (!dir_exists(get_outdir())) {
    return;
  }
  directory_t dir;
  directory_data_init(&dir);
  ALWAYS_ASSERT(directory_get_recurse(get_outdir(), &dir) != -1);
  DIRECTORY_ITER(dir, i, entry,
    ALWAYS_ASSERT(file_exists(entry->path));
    ALWAYS_ASSERT(unlink(entry->path) != -1);
  );
  directory_data_free(&dir);
}
