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
  long    cores    = sysconf(_SC_NPROCESSORS_ONLN);
  Ulong   threadno = 0;
  thread *threads;
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
  directory_entry_t *entry;
  directory_data_init(&dir);
  ALWAYS_ASSERT(directory_get_recurse(get_outdir(), &dir) != -1);
  for (Ulong i = 0; i < dir.len; ++i) {
    entry = dir.entries[i];
    ALWAYS_ASSERT(file_exists(entry->path));
    ALWAYS_ASSERT(unlink(entry->path) != -1);
  }
}
