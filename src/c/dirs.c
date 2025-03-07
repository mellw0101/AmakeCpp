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
/* The `binary` directory path `Amake` uses. */
static char *bindir = NULL;
/* The `.o` output directory path Amake uses. */
static char *outdir = NULL;
/* The Amake config directory path Amake uses. */
static char *amakedir = NULL;
/* The compile data directory path Amake uses to store a file for every compiled file, so we can skip compalation when its not needed. */
static char *amakecompdir = NULL;

/* Static mutexes to protect usage of these ptrs across threads. */
static mutex_t envpwd_mutex       = mutex_init_static;
static mutex_t srcdir_mutex       = mutex_init_static;
static mutex_t cdir_mutex         = mutex_init_static;
static mutex_t cppdir_mutex       = mutex_init_static;
static mutex_t builddir_mutex     = mutex_init_static;
static mutex_t bindir_mutex       = mutex_init_static;
static mutex_t outdir_mutex       = mutex_init_static;
static mutex_t amakedir_mutex     = mutex_init_static;
static mutex_t amakecompdir_mutex = mutex_init_static;


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

/* Get the path to the `binary` build directory `Amake` uses.  Should be freed using `free_bindir()` only. */
char *get_bindir(void) {
  mutex_lock(&bindir_mutex);
  if (!bindir) {
    bindir = concatpath(get_builddir(), "/bin");
  }
  mutex_unlock(&bindir_mutex);
  return bindir;
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

/* Free's the `bindir` ptr and sets it to `NULL`. */
void free_bindir(void) {
  if (bindir) {
    free(bindir);
    bindir = NULL;
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
  free_bindir();
  free_outdir();
  free_amakedir();
  free_amakecompdir();
}

/* Create a dir for this project. */
void amkdir(const char *const restrict path) {
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

