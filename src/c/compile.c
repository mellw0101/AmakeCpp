/** @file compile.c

  @author  Melwin Svensson.
  @date    3-2-2025.

 */
#include "../include/cproto.h"

/* Create a new blank allocated `compile_data_entry_t` structure. */
compile_data_entry_t *compile_data_entry_make(void) {
  compile_data_entry_t *entry = amalloc(sizeof(*entry));
  entry->srcpath        = NULL;
  entry->outpath        = NULL;
  entry->compiler       = NULL;
  entry->flags          = NULL;
  entry->direntry       = NULL;
  entry->compile_needed = TRUE;
  return entry;
}

/* Free a `compile_data_entry_t` structure. */
void compile_data_entry_free(compile_data_entry_t *const entry) {
  ASSERT(entry);
  free(entry->srcpath);
  free(entry->outpath);
  free(entry->compiler);
  free(entry->flags);
  if (entry->direntry) {
    directory_entry_free(entry->direntry);
  }
  free(entry);
}

/* Init the internal data of a compile_data_t structure. */
void compile_data_data_init(compile_data_t *const output) {
  ASSERT(output);
  output->len  = 0;
  output->cap  = 10;
  output->data = amalloc(sizeof(void *) * output->cap);
}

/* Free the internal data of a `compile_data_t` structure. */
void compile_data_data_free(compile_data_t *const data) {
  ASSERT(data);
  while (data->len) {
    compile_data_entry_free(data->data[--data->len]);
  }
}

/* Base function to get entries in a Amake source folder. */
static void compile_data_get(compile_data_t *const output, const char *const __restrict path, const char *const fileext, const char *const compiler, const char *const flags) {
  /* Assert all parameters.  We must ensure correctness above all. */
  ASSERT(output);
  ASSERT(output->data);
  ASSERT(output->cap);
  ASSERT(path);
  ASSERT(compiler);
  ASSERT(flags);
  compile_data_entry_t *compdata;
  directory_t dir;
  directory_data_init(&dir);
  /* Always assert that this does not return an error, this is because Amake
   * should never fail to get the entries in a source folder it uses. */
  ALWAYS_ASSERT(directory_get_recurse(path, &dir) != -1);
  for (Ulong i = 0; i < dir.len; ++i) {
    directory_entry_t *entry = dir.entries[i];
    /* Only add .c files to output. */
    if (entry->ext && strcmp(entry->ext, fileext) == 0) {
      /* Create the compile_data_entry_t structure. */
      compdata = compile_data_entry_make();
      /* Populate the fields. */
      compdata->outpath  = fmtstr("%s/%s.o", get_outdir(), entry->name);
      compdata->srcpath  = copy_of(entry->path);
      compdata->compiler = copy_of(compiler);
      compdata->flags    = copy_of(flags);
      /* Steal the entire directory_entry_t structure. */
      compdata->direntry = directory_entry_extract(&dir, i--);
      /* Resize the output array if needed. */
      ENSURE_PTR_ARRAY_SIZE(output->data, output->cap, output->len);
      /* Insert the data into output->data. */
      output->data[output->len++] = compdata;
    }
  }
  /* NULL-TERMINATE the array and trim it, this saves memory as well
   * as ensures correct iteration even when output->len is not used. . */
  TRIM_PTR_ARRAY(output->data, output->cap, output->len);
  output->data[output->len] = NULL;
  directory_data_free(&dir);
}

/* Get the compile data for all files in the `c` source dir. */
void compile_data_getc(compile_data_t *const output) {
  ASSERT(output);
  ASSERT(output->data);
  ASSERT(output->cap);
  compile_data_get(output, get_cdir(), "c", DEFAULT_C_COMPILER, C_DEFAULT_ARGS);
}

/* Get the compile data for all files in the `c` source dir. */
void compile_data_getcpp(compile_data_t *const output) {
  ASSERT(output);
  ASSERT(output->data);
  ASSERT(output->cap);
  compile_data_get(output, get_cppdir(), "cpp", DEFAULT_CPP_COMPILER, CC_DEFAULT_ARGS);
}

/* Write compile entry data to `amakefile`. */
static void write_compile_data(const char *amakefile, compile_data_entry_t *const entry) {
  ASSERT(amakefile);
  ASSERT(entry);
  char *wrdata;
  int   fd;
  /* Open the path as a write only fd, creating it if it does not exist. */
  ALWAYS_ASSERT((fd = open(amakefile, (O_WRONLY | O_CREAT), 0755)) != -1);
  wrdata = fmtstr(
    "mtime:%ld\n"
    "size:%ld\n"
    "outpath:%s\n"
    "srcpath:%s\n",
    entry->direntry->stat->st_mtime,
    entry->direntry->stat->st_size,
    entry->outpath,
    entry->srcpath
  );
  /* Write the data to the file. */
  lock_fd(fd, F_WRLCK);
  ALWAYS_ASSERT(write(fd, wrdata, strlen(wrdata)) != -1);
  unlock_fd(fd);
  free(wrdata);
  close(fd);
}

/* Read compile entry data. */
static char *read_compile_data(const char *amakefile) {
  ASSERT(amakefile);
  int   fd;
  char  buffer[4096];
  char *ret = amalloc(1);
  long  bytes_read, total_read = 0;
  /* Open the fd in read only mode. */
  ALWAYS_ASSERT((fd = open(amakefile, O_RDONLY)) != -1);
  /* Read the .amake compile data file in 4096 byte chunks. */
  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    ret = arealloc(ret, (total_read + bytes_read + 1));
    memcpy((ret + total_read), buffer, bytes_read);
    total_read += bytes_read;
  }
  ret[total_read] = '\0';
  close(fd);
  return ret;
}

/* Check if this entry needs to be compiled. */
static void check_compile_data(const char *amakefile, compile_data_entry_t *const entry) {
  ASSERT(amakefile);
  ASSERT(entry);
  char  *read_data;
  char **lines;
  char  *outpath = NULL;
  long   mtime;
  read_data = read_compile_data(amakefile);
  lines = split_string(read_data, '\n');
  for (char **line = lines; *line; ++line) {
    /* Get last modification time. */
    if (strncmp(*line, S__LEN("mtime:")) == 0) {
      ALWAYS_ASSERT(parse_num((*line) + strlen("mtime:"), &mtime));
    }
    /* Get output path. */
    else if (strncmp(*line, S__LEN("outpath:")) == 0) {
      outpath = copy_of((*line) + strlen("outpath:"));
    }
    free(*line);
  }
  free(lines);
  /* Free the data after we have extracted what we need. 
   * Now check if this entry needs compalation. */
  free(read_data);
  /* If the output file does not exist.  We always need to recompile. */
  if (!file_exists(outpath)) {
    entry->compile_needed = TRUE;
  }
  /* Also if the modify time is diffrent from the one on file, we recompile. */
  else if (entry->direntry->stat->st_mtime != mtime) {
    entry->compile_needed = TRUE;
  }
  /* Otherwise, this entry does not need recompalation. */
  else {
    entry->compile_needed = FALSE;
  }
  free(outpath);
}

/* Simple compile command using system, for now. */
void *compile_data_task(void *arg) {
  /* Ensure the data is correct. */
  compile_data_entry_t *data = arg;
  ASSERT(data);
  ASSERT(data->srcpath);
  ASSERT(data->outpath);
  ASSERT(data->compiler);
  ASSERT(data->flags);
  char *amakefile;
  char *command;
  char **argv;
  char *execout;
  bool  existed;
  /* Make the path to the compile data for this file. */
  amakefile = fmtstr("%s/%s.amake", get_amakecompdir(), data->direntry->name);
  /* Check if the file already exists. */
  existed = file_exists(amakefile);
  /* If the compile data file does not exist. */
  if (!existed) {
    write_compile_data(amakefile, data);
  }
  /* Otherwise, when the compile data file exists. */
  else {
    check_compile_data(amakefile, data);
  }
  /* If there is no need to compile, just return. */
  if (!data->compile_needed) {
    free(amakefile);
    return NULL;
  }
  /* Otherwise, compile. */
  else {
    /* Create the command as one string. */
    command = fmtstr("%s -c %s %s -o %s", data->compiler, data->srcpath, data->flags, data->outpath);
    stdout_printf("%s\n", command);
    /* Then split it into args. */
    argv = split_string(command, ' ');
    free(command);
    /* Execute the compalation. */
    fork_bin(argv[0], argv, (char *[]){ NULL }, &execout);
    /* Free argv. */
    free_nullterm_carray(argv);
    stdout_printf("%s", execout);
    free(execout);
    /* Delete the amake compile data file if it exists. */
    if (file_exists(amakefile)) {
      ALWAYS_ASSERT(unlink(amakefile) != -1);
    }
    /* Then write the fresh data. */
    write_compile_data(amakefile, data);
  }
  free(amakefile);
  return NULL;
}
