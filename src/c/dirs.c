/** @file dirs.c

  @author  Melwin Svensson.
  @date    3-2-2025.

 */
#include "../include/cproto.h"

/* A copy of the `PWD` env variable. */
static char *envpwd = NULL;
/* The source directory path Amake uses. */
static char *srcdir = NULL;
/* The `c` source directory path Amake uses. */
static char *cdir = NULL;
/* The `cpp` source directory path Amake uses. */
static char *cppdir = NULL;
/* The `build` directory path Amake uses. */
static char *builddir = NULL;
/* The `.o` output directory path Amake uses. */
static char *outdir = NULL;
/* The Amake config directory path Amake uses. */
static char *amakedir = NULL;
/* The compile data directory path Amake uses to store a file for every compiled file, so we can skip compalation when its not needed. */
static char *amakecompdir = NULL;

/* Static mutexes to protect usage of these ptrs across threads. */
static mutex envpwd_mutex       = static_mutex_init;
static mutex srcdir_mutex       = static_mutex_init;
static mutex cdir_mutex         = static_mutex_init;
static mutex cppdir_mutex       = static_mutex_init;
static mutex builddir_mutex     = static_mutex_init;
static mutex outdir_mutex       = static_mutex_init;
static mutex amakedir_mutex     = static_mutex_init;
static mutex amakecompdir_mutex = static_mutex_init;

/* Get the pwd env variable.  This function cannot return `NULL`.  The returned ptr
 * should never be freed during runtime, only when exiting.  And only using `freepwd()`. */
char *getpwd(void) {
  const char *cenvpwd = NULL;
  mutex_lock(&envpwd_mutex);
  /* If the static ptr to pwd is NULL, fetch it. */
  if (!envpwd) {
    ALWAYS_ASSERT((cenvpwd = getenv("PWD")));
    envpwd = copy_of(cenvpwd);
  }
  mutex_unlock(&envpwd_mutex);
  return envpwd;
}

/* Get the path to the src dir Amake uses.  Should be freed using `freesrcdir()` only. */
char *get_srcdir(void) {
  mutex_lock(&srcdir_mutex);
  if (!srcdir) {
    srcdir = concatpath(getpwd(), "/src");
  }
  mutex_unlock(&srcdir_mutex);
  return srcdir;
}

/* Get the path to the `c` source dir Amake uses.  Should be freed using `freecdir()` only. */
char *get_cdir(void) {
  mutex_lock(&cdir_mutex);
  if (!cdir) {
    cdir = concatpath(get_srcdir(), "/c");
  }
  mutex_unlock(&cdir_mutex);
  return cdir;
}

/* Get the path to the `cpp` source dir Amake uses.  Should be freed using `freecppdir()` only. */
char *get_cppdir(void) {
  mutex_lock(&cppdir_mutex);
  if (!cppdir) {
    cppdir = concatpath(get_srcdir(), "/cpp");
  }
  mutex_unlock(&cppdir_mutex);
  return cppdir;
}

/* Get the path to the `build` directory Amake uses.  Should be freed using `freebuilddir()` only. */
char *get_builddir(void) {
  mutex_lock(&builddir_mutex);
  if (!builddir) {
    builddir = concatpath(getpwd(), "/build");
  }
  mutex_unlock(&builddir_mutex);
  return builddir;
}

/* Get the path to the `output` directory Amake uses.  Should be freed using `freeoutdir()` only. */
char *get_outdir(void) {
  mutex_lock(&outdir_mutex);
  if (!outdir) {
    outdir = concatpath(get_builddir(), "/obj");
  }
  mutex_unlock(&outdir_mutex);
  return outdir;
}

/* Get the configuration directory amake uses. */
char *get_amakedir(void) {
  mutex_lock(&amakedir_mutex);
  if (!amakedir) {
    amakedir = concatpath(getpwd(), "/.amake");
  }
  mutex_unlock(&amakedir_mutex);
  return amakedir;
}

/* Get the directory Amake uses to store data on files that have been compiled so we know if compalation is required. */
char *get_amakecompdir(void) {
  mutex_lock(&amakecompdir_mutex);
  if (!amakecompdir) {
    amakecompdir = concatpath(get_amakedir(), "/compile_data");
  }
  mutex_unlock(&amakecompdir_mutex);
  return amakecompdir;
}

/* Frees the `pwd` ptr and sets it to `NULL`. */
void free_pwd(void) {
  if (envpwd) {
    free(envpwd);
    envpwd = NULL;
  }
}

/* Frees the `srcdir` ptr and sets it to `NULL`. */
void free_srcdir(void) {
  if (srcdir) {
    free(srcdir);
    srcdir = NULL;
  }
}

/* Frees the `cdir` ptr and sets it to `NULL`. */
void free_cdir(void) {
  if (cdir) {
    free(cdir);
    cdir = NULL;
  }
}

/* Frees the `cppdir` ptr and sets it to `NULL`. */
void free_cppdir(void) {
  if (cppdir) {
    free(cppdir);
    cppdir = NULL;
  }
}

/* Frees the `builddir` ptr and sets it to `NULL`. */
void free_builddir(void) {
  if (builddir) {
    free(builddir);
    builddir = NULL;
  }
}

/* Frees the `outdir` ptr and sets it to `NULL`. */
void free_outdir(void) {
  if (outdir) {
    free(outdir);
    outdir = NULL;
  }
}

/* Frees the `amakedir` ptr and sets it to `NULL`. */
void free_amakedir(void) {
  if (amakedir) {
    free(amakedir);
    amakedir = NULL;
  }
}

/* Frees the `compile data directory` that Amake uses to compile only what is needed. */
void free_amakecompdir(void) {
  if (amakecompdir) {
    free(amakecompdir);
    amakecompdir = NULL;
  }
}

/* Free all dir ptr's that `Amake` uses. */
void free_dirptrs(void) {
  free_pwd();
  free_srcdir();
  free_cdir();
  free_cppdir();
  free_builddir();
  free_outdir();
  free_amakedir();
  free_amakecompdir();
}

/* Return `TRUE` when path exists and is a directory and when we have access to the directory. */
bool dir_exists(const char *const __restrict path) {
  ASSERT(path);
  struct stat st;
  /* If the path exists, but we do not have permissions to access it return false. */
  if (access(path, R_OK) != 0) {
    return FALSE;
  }
  return (stat(path, &st) != -1 && S_ISDIR(st.st_mode));
}

/* Create a new blank allocated directory entry. */
directory_entry_t *directory_entry_make(void) {
  directory_entry_t *entry = amalloc(sizeof(*entry));
  entry->type       = 0;
  entry->name       = NULL;
  entry->path       = NULL;
  entry->ext        = NULL;
  entry->clean_name = NULL;
  entry->stat       = NULL;
  return entry;
}

/* Retrieve a `directory_entry_t *` from a `directory_t` structure, note that this removes the ptr in `dir` to this entry. */
directory_entry_t *directory_entry_extract(directory_t *const dir, Ulong idx) {
  ASSERT(dir);
  ASSERT_MSG(dir->entries,
    "The passed directory_t structure has not been initilized, if using a stack "
    "based directory_t structure initilize it with 'directory_data_init(&dir).'"
  );
  /* Ensure this is a valid index before anything else. */
  ALWAYS_ASSERT(idx < dir->len);
  directory_entry_t *retentry = dir->entries[idx];
  /* Move the entries over by one including the NULL-TERMINATOR. */
  memmove((dir->entries + idx), (dir->entries + idx + 1), ((dir->len - idx) * sizeof(void *)));
  --dir->len;
  return retentry;
}

/* Free the data of an `directory_entry_t`, then free the entry itself. */
void directory_entry_free(directory_entry_t *const entry) {
  ASSERT(entry);
  ASSERT(entry->name);
  ASSERT(entry->path);
  /* Free the data assosiated with entry. */
  free(entry->name);
  free(entry->path);
  free(entry->ext);
  free(entry->clean_name);
  free(entry->stat);
  /* Then free entry itself. */
  free(entry);
}

/* Init a directory_t structure. */
void directory_data_init(directory_t *const dir) {
  ASSERT(dir);
  dir->len = 0;
  dir->cap = 10;
  dir->entries = amalloc(sizeof(void *) * dir->cap);
}

/* Free the internal data of a `directory_t` structure. */
void directory_data_free(directory_t *const dir) {
  ASSERT(dir);
  /* Iter until len reatches zero. */
  while (dir->len) {
    directory_entry_free(dir->entries[--dir->len]);
  }
  free(dir->entries);
}

/* Get all entries in `path` and append them onto `output->entries`.  Return `-1` on error.  Otherwise, `0`. */
int directory_get(const char *const __restrict path, directory_t *const output) {
  ASSERT(path);
  ASSERT(output);
  const char *fileext = NULL;
  struct dirent *direntry;
  DIR *dir;
  directory_entry_t *entry;
  /* If the path does not exist, or is not a directory.  Return early. */
  if (!dir_exists(path)) {
    return -1;
  }
  /* Open the directory. */
  ALWAYS_ASSERT(dir = opendir(path));
  /* Iter over all entries in opened dir. */
  while ((direntry = readdir(dir))) {
    /* Skip directory trevarsal entries. */
    if (direntry->d_type == DT_DIR && _D_ALLOC_NAMLEN(direntry) <= 5 && direntry->d_name[0] == '.'
     && (direntry->d_name[1] == '\0' || (direntry->d_name[1] == '.' && direntry->d_name[2] == '\0'))) {
      continue;
    }
    /* Allocate the directory_entry_t structure. */
    entry = directory_entry_make();
    /* Allocate the internal data. */
    entry->type = direntry->d_type;
    entry->name = measured_copy(direntry->d_name, (_D_ALLOC_NAMLEN(direntry) - 1));
    entry->path = concatpath(path, entry->name);
    fileext = ext(entry->name);
    if (fileext) {
      entry->ext = copy_of(fileext + 1);
      entry->clean_name = measured_copy(entry->name, (fileext - entry->name));
    }
    statalloc(entry->path, &entry->stat);
    /* Insert the entry into output. */
    ENSURE_PTR_ARRAY_SIZE(output->entries, output->cap, output->len);
    output->entries[output->len++] = entry;
  }
  /* Trim the entries array and NULL-TERMINATE it to same memory and ensure safe iteration even without a len. */
  TRIM_PTR_ARRAY(output->entries, output->cap, output->len);
  output->entries[output->len] = NULL;
  closedir(dir);
  return 0;
}

/* Recursivly get all entries in `path`. */
int directory_get_recurse(const char *const __restrict path, directory_t *const output) {
  ASSERT(path);
  ASSERT(output);
  ASSERT(output->entries);
  /* The subdir, if any exists. */
  char *subdir;
  /* Used to scope the recursive nature of this function.  As it will modify the same structure. */
  Ulong waslen, newlen;
  /* Set waslen before running directory_get(). */
  waslen = output->len;
  if (directory_get(path, output) == -1) {
    return -1;
  }
  /* Then set newlen after. */
  newlen = output->len;
  /* Now we have a set scope to perform the recursive calls. */
  for (Ulong i = waslen; i < newlen; ++i) {
    if (output->entries[i]->type == DT_DIR) {
      subdir = concatpath(path, output->entries[i]->name);
      directory_get_recurse(subdir, output);
      free(subdir);
    }
  }
  return 0;
}

/* Create a dir for this project. */
void amkdir(const char *const __restrict path) {
  ASSERT(path);
  /* We dont have premissions to make this directory. */
  ALWAYS_ASSERT_MSG((access(path, R_OK) != 0), "We dont have permission.");
  /* The directory already exists. */
  if (dir_exists(path)) {
    return;
  }
  /* Create the directory. */
  ALWAYS_ASSERT(mkdir(path, 0755) != -1);
}

