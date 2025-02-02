#include "../include/prototypes.h"

#include <dirent.h>
#include <Mlib/Debug.h>

void run(const char *bin, const char *const *argv, const char *const *envv) {
  int pid, status;
  if ((pid = fork()) == -1) {
    return;
  }
  else if (pid == 0) {
    if (envv == NULL) {
      execvp(bin, (char **)argv);
    }
    else {
      execvpe(bin, (char **)argv, (char **)envv);
    }
    exit(1);
  }
  else {
    if (waitpid(pid, &status, 0); WIFEXITED(status)) {
      if (WEXITSTATUS(status) != 0) {
        fprintf(stderr, "%s: Exited with status: %i", bin, status);
      }
    }
    else {
      fprintf(stderr, "%s: Exited with unexpexted status %i", bin, status);
    }
  }
}

bool file_size(Ulong *size, FILE *file) {
  int prev = ftell(file);
  if (prev == -1) {
    return false;
  }
  fseek(file, 0L, SEEK_END);
  *size = ftell(file);
  if (*size == -1) {
    return false;
  }
  fseek(file, prev, SEEK_SET);
  return true;
}

void copy_stack_nstr(char *stack_dst, const char *stack_src, Uint n) noexcept {
  for (Uint i = 0; i < n; ++i) {
    stack_dst[i] = stack_src[i];
  }
}

void extract_name_and_ext(DirEntry *e) noexcept {
  for (Uint i = e->file_len; i > 0; --i) {
    if (e->file[i] == '.') {
      e->ext_len = e->file_len - i;
      Uint ext = 0;
      for (; ext < (e->file_len - i); ++ext) {
        e->ext[ext] = e->file[ext + i];
      }
      e->ext[ext] = '\0';
      e->name_len = i;
      Uint name = 0;
      for (; name < i; ++name) {
        e->name[name] = e->file[name];
      }
      e->name[name] = '\0';
      break;
    }
  }
}

bool dir_exists(const char *path) {
  struct stat st;
  return (stat(path, &st) != -1 && S_ISDIR(st.st_mode));
}

/* Return a malloc`ed 'char **' with malloc`ed 'char *' strings containing all paths in the env var 'PATH'.  Return`s NULL on failure. */
char **get_env_paths(Ulong *npaths) {
  /* Get the PATH env var. */
  const char *path_env = getenv("PATH");
  /* If this fails, return NULL. */
  if (!path_env) {
    return NULL;
  }
  /* Set up the array size and cap.  Start with a cap of 10. */
  Ulong size = 0, cap = 10;
  char **paths = (char **)amalloc(sizeof(char *) * cap);
  const char *start = path_env, *end = NULL;
  for (Ulong i = 0; path_env[i]; ++i) {
    if (path_env[i] == ':' || !path_env[i + 1]) {
      end = (!path_env[i + 1] ? &path_env[i + 1] : &path_env[i]);
      /* Realloc the array when needed. */
      if (size == cap) {
        cap *= 2;
        paths = (char **)arealloc(paths, (sizeof(char *) * cap));
      }
      paths[size++] = measured_copy(start, (end - start));
      start = (end + 1);
    }
  }
  /* Resize the array to the exact number of paths, to save memory. */
  paths = (char **)arealloc(paths, (sizeof(char *) * size));
  *npaths = size;
  return paths;
}

bool exec_exists(const char *name, char **fullpath_ret) {
  Ulong npaths;
  char **env_paths = get_env_paths(&npaths);
  if (!env_paths) {
    return FALSE;
  }
  struct stat st;
  char *fullpath = NULL;
  bool ret = FALSE;
  for (Ulong i = 0; i < npaths; ++i) {
    fullpath = concatenate_path(env_paths[i], name);
    if (stat(fullpath, &st) != -1 && st.st_mode & S_IXUSR) {
      ret = TRUE;
      *fullpath_ret = fullpath;
      break;
    }
    free(fullpath);
  }
  while (npaths) {
    free(env_paths[--npaths]);
  }
  free(env_paths);
  return ret;
}

DirEntry *files_in_dir(const char *path, Ulong *n) {
  DIR *dir = opendir(path);
  if (!dir) {
    return nullptr;
  }
  dirent   *entry;
  Uint      size = 0, cap = 10;
  DirEntry *buf = (DirEntry *)malloc(sizeof(DirEntry) * cap);
  for (; (entry = readdir(dir)); ++size) {
    (size == cap) ? ((cap *= 2), (buf = (DirEntry *)realloc(buf, sizeof(DirEntry) * cap))) : 0;
    DirEntry *e   = &buf[size];
    Uint      len = strlen(entry->d_name);
    copy_stack_nstr(e->file, entry->d_name, len);
    e->file[len] = '\0';
    e->type      = entry->d_type;
    e->file_len  = len;
  }
  *n = size;
  closedir(dir);
  return buf;
}

void free_files(DirEntry *files, Ulong n) {
  free(files);
}

void *amalloc(Ulong howmush) {
  void *ptr = malloc(howmush);
  if (!ptr) {
    fprintf(stderr, "AmakeCpp is out of memory\n");
    exit(1);
  }
  return ptr;
}

void *arealloc(void *ptr, Ulong howmush) {
  ptr = realloc(ptr, howmush);
  if (!ptr) {
    fprintf(stderr, "AmakeCpp is out of memory\n");
    exit(1);
  }
  return ptr;
}

char *measured_copy(const char *string, Ulong count) {
  char *ret = (char *)amalloc(count + 1);
  memcpy(ret, string, count);
  ret[count] = '\0';
  return ret;
}

char *copy_of(const char *string) {
  ASSERT(string);
  return measured_copy(string, strlen(string));
}

char *get_pwd(void) {
  if (!pwd) {
    const char *env_pwd = getenv("PWD");
    if (!env_pwd) {
      fprintf(stderr, "Could not get pwd env var.\n");
      exit(1);
    }
    pwd = copy_of(env_pwd);
  }
  return pwd;
}

char *get_src_dir(void) {
  if (!src_dir) {
    src_dir = concatenate_path(get_pwd(), "/src");
  }
  return src_dir;
}

char *get_lib_src_dir(void) {
  if (!lib_src_dir) {
    lib_src_dir = concatenate_path(get_src_dir(), "/lib");
  }
  return lib_src_dir;
}

char *concatenate_path(const char *s1, const char *s2) {
  Ulong s1len = strlen(s1);
  char *ret = NULL;
  if ((s1[s1len - 1] == '/' && *s2 != '/') || (s1[s1len - 1] != '/' && *s2 == '/')) {
    ret = (char *)amalloc(s1len + strlen(s2) + 1);
    sprintf(ret, "%s%s", s1, s2);
  }
  else {
    ret = (char *)amalloc(s1len + strlen(s2) + 2);
    sprintf(ret, "%s%s%s", s1, "/", s2);
  }
  return ret;
}

int make_directory(const char *fullpath) {
  return mkdir(fullpath, 0755);
}

extern char **environ;

int launch_bin(const char *binary, const char *const argv[], const char *const envp[]) {
  char *fullbinpath;
  if (strncmp(binary, "find:", 5) == 0) {
    if (!exec_exists((binary + 5), &fullbinpath)) {
      return -2;
    }
  }
  else {
    fullbinpath = NULL;
  }
  int pipefd[2];
  /* Create the pipe for reading the output from the exec. */
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(1);
  }
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }
  /* Child process. */
  else if (pid == 0) {
    close(pipefd[0]); /* Close unused read fd. */
    dup2(pipefd[1], STDOUT_FILENO); /* Redirect strout. */
    dup2(pipefd[1], STDERR_FILENO); /* Redirect stderr. */
    close(pipefd[1]);
    if (execve(binary, (char **)argv, (*envp ? ((strcmp(*envp, "__parent_env") == 0) ? environ : (char **)envp) : (char **)envp)) == -1) {
      perror("execve");
      exit(1);
    }
  }
  /* Parent process. */
  else {
    close(pipefd[1]); /* Close unused write fd. */
    char buffer[4096];
    long count;
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      buffer[count] = '\0';
      printf("%s", buffer);
    }
    close(pipefd[0]);
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("waitpid");
      exit(1);
    }
    if (WIFEXITED(status)) {
      printf("%s exited with status %d.\n", binary, WEXITSTATUS(status));
      return WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
      printf("%s killed by signal %d.\n", binary, WTERMSIG(status));
      return WTERMSIG(status);
    }
    else {
      printf("%s terminated abnormally.\n", binary);
    }
  }
  return -1;
}

int launch_bin(const char *binary, MVector<const char *> argv, MVector<const char *> envp) {
  /* Find the ececutable if the user wants that. */
  char *fullbinpath;
  if (strncmp(binary, "find:", 5) == 0) {
    if (!exec_exists((binary + 5), &fullbinpath)) {
      return -2;
    }
  }
  else {
    fullbinpath = NULL;
  }
  /* Insert the bin as the first arg. */
  fullbinpath ? argv.insert((Uint)0, fullbinpath) : argv.insert((Uint)0, binary);
  /* Add null terminator to arg and enviroment vector. */
  argv.push_back(NULL);
  envp.push_back(NULL);
  int pipefd[2];
  /* Create the pipe for reading the output from the exec. */
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(1);
  }
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }
  /* Child process. */
  else if (pid == 0) {
    close(pipefd[0]); /* Close unused read fd. */
    dup2(pipefd[1], STDOUT_FILENO); /* Redirect strout. */
    dup2(pipefd[1], STDERR_FILENO); /* Redirect stderr. */
    close(pipefd[1]);
    if (execve((fullbinpath ? fullbinpath : binary), (char **)argv.data(),
     ((envp[0] != NULL) ? ((strcmp(envp[0], "__parent_env") == 0) ? environ : (char **)envp.data()) : (char **)envp.data())) == -1) {
      perror("execve");
      exit(1);
    }
  }
  /* Parent process. */
  else {
    close(pipefd[1]); /* Close unused write fd. */
    char buffer[4096];
    long count;
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      buffer[count] = '\0';
      printf("%s", buffer);
    }
    close(pipefd[0]);
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("waitpid");
      exit(1);
    }
    if (WIFEXITED(status)) {
      printf("%s exited with status %d.\n", binary, WEXITSTATUS(status));
      free(fullbinpath);
      fullbinpath = NULL;
      return WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status)) {
      printf("%s killed by signal %d.\n", binary, WTERMSIG(status));
      free(fullbinpath);
      fullbinpath = NULL;
      return WTERMSIG(status);
    }
    else {
      printf("%s terminated abnormally.\n", binary);
    }
  }
  free(fullbinpath);
  fullbinpath = NULL;
  return -1;
}

void extract_tar_gz(const char *path, const char *output_path) {
  char *bin;
  if (exec_exists("tar", &bin)) {
    launch_bin(bin, ARGV(bin, "-xzf", path, "-C", output_path), ARGV(NULL));
  }
  free(bin);
}

void extract_zip(const char *path, const char *output_path) {
  char *bin;
  if (exec_exists("unzip", &bin)) {
    launch_bin(bin, ARGV(bin, "-o", path, "-d", output_path), ARGV(NULL));
  }
  free(bin);
}

/* Free the memory of the given array, which should contain len elements. */
void free_chararray(char **array, Ulong len) {
  if (!array) {
    return;
  }
  while (len > 0) {
    free(array[--len]);
  }
  free(array);
}

/* Append an array onto 'array'.  Free 'append' but not any elements in it after call. */
void append_chararray(char ***array, Ulong *len, char **append, Ulong append_len) {
  Ulong new_len = ((*len) + append_len);
  *array = (char **)arealloc(*array, (sizeof(char *) * (new_len + 1)));
  for (Ulong i = 0; i < append_len; ++i) {
    (*array)[(*len) + i] = append[i];
  }
  *len = new_len;
  (*array)[*len] = NULL;
}

/* Retrieve files and dirs from path.  Returns -1 apon failure to open directory. */
int entries_in_dir(const char *path, char ***files, Ulong *nfiles, char ***dirs, Ulong *ndirs) {
  /* Open the target directory. */
  dirent *directory_entry = NULL;
  DIR *target_directory = opendir(path);
  /* Return early apon failure to open directory. */
  if (!target_directory) {
    logE("Failed to open dir: %s.\n");
    return -1;
  }
  /* Define our buffers, one for file entries and one for dirs. */
  char **file_buf  = NULL, **dir_buf  = NULL;
  Ulong  file_size = 0,      dir_size = 0;
  Ulong  file_cap  = 40,     dir_cap  = 40;
  /* Malloc our buffers. */
  file_buf = (char **)amalloc(sizeof(char *) * file_cap);
  dir_buf  = (char **)amalloc(sizeof(char *) * dir_cap);
  /* Fetch all entries in the target directory. */
  while ((directory_entry = readdir(target_directory))) {
    switch (directory_entry->d_type) {
      case DT_DIR: /* Directory entry. */ {
        /* Skip directory traversers. */
        if (strcmp(directory_entry->d_name, "..") == 0 || strcmp(directory_entry->d_name, ".") == 0) {
          continue;
        }
        ENSURE_CHARARRAY_CAPACITY(dir_buf, dir_cap, dir_size);
        dir_buf[dir_size++] = measured_copy(directory_entry->d_name, (_D_ALLOC_NAMLEN(directory_entry) - 1));
        break;
      }
      case DT_REG: /* Regular file entry. */ {
        ENSURE_CHARARRAY_CAPACITY(file_buf, file_cap, file_size);
        file_buf[file_size++] = measured_copy(directory_entry->d_name, (_D_ALLOC_NAMLEN(directory_entry) - 1));
        break;
      }
    }
  }
  /* Trim data buffer`s to the size of data. */
  file_buf = (char **)arealloc(file_buf, (sizeof(char *) * (file_size + 1)));
  dir_buf  = (char **)arealloc(dir_buf,  (sizeof(char *) * (dir_size  + 1)));
  /* NULL-terminate buffer`s to ensure NULL-safe operation`s. */
  file_buf[file_size] = NULL;
  dir_buf[dir_size]   = NULL;
  closedir(target_directory);
  /* Assign the data buffers. */
  *files  = file_buf;
  *nfiles = file_size;
  *dirs  = dir_buf;
  *ndirs = dir_size;
  return 0;
}

/* Recursivly get all files and dirs from a starting path. */
int recursive_entries_in_dir(const char *path, char ***files, Ulong *nfiles, char ***dirs, Ulong *ndirs) {
  char **local_files, **local_dirs;
  Ulong  local_nfiles,  local_ndirs;
  if (entries_in_dir(path, &local_files, &local_nfiles, &local_dirs, &local_ndirs) == -1) {
    return -1;
  }
  append_chararray(files, nfiles, local_files, local_nfiles);
  append_chararray(dirs,  ndirs,  local_dirs,  local_ndirs);
  for (Ulong i = 0; i < local_ndirs; ++i) {
    char *subdir = concatenate_path(path, local_dirs[i]);
    recursive_entries_in_dir(subdir, files, nfiles, dirs, ndirs);
    free(subdir);
  }
  free(local_files);
  free(local_dirs);
  return 0;
}

/* Helper to correctly get all entries in a starting path. */
int get_all_entries_in_dir(const char *path, char ***files, Ulong *nfiles, char ***dirs, Ulong *ndirs) {
  /* Init the arrays. */
  char **local_files  = (char **)amalloc(sizeof(char *));
  char **local_dirs   = (char **)amalloc(sizeof(char *));
  Ulong  local_nfiles = 0;
  Ulong  local_ndirs  = 0;
  if (recursive_entries_in_dir(path, &local_files, &local_nfiles, &local_dirs, &local_ndirs) == -1) {
    free_chararray(local_files, local_nfiles);
    free_chararray(local_dirs, local_ndirs);
    *files  = NULL;
    *dirs   = NULL;
    *nfiles = 0;
    *ndirs  = 0;
    return -1;
  }
  *files  = local_files;
  *dirs   = local_dirs;
  *nfiles = local_nfiles;
  *ndirs  = local_ndirs;
  return 0;
}

bool lock_fd(int fd, short type) {
  ALWAYS_ASSERT(fd >= 0);
  flock lock {
    type,
    SEEK_SET,
    0,
    0,
    getpid()
  };
  if (fcntl(fd, F_SETLKW, &lock) == -1) {
    return FALSE;
  }
  return TRUE;
}

bool unlock_fd(int fd) {
  ALWAYS_ASSERT(fd >= 0);
  flock lock {
    F_UNLCK,
    SEEK_SET,
    0,
    0,
    getpid()
  };
  if (fcntl(fd, F_SETLK, &lock) == -1) {
    return FALSE;
  }
  return TRUE;
}

static const char *config_check_type_str(config_check_type type) {
  static thread_local char buffer[256];
  switch (type) {
    case CONFIG_HAVE___THREAD: {
      copy_stack_nstr(buffer, S__LEN("__thread") + 1);
      break;
    }
    default: {
      buffer[0] = '\0';
      break;
    }
  }
  return buffer;
}

/* Just testing for now, this will eventualy become the config function where it trys to compile for a specific thing
 * to see if it is available to the current system, and if it is define a `HAVE_THING` in config.h, or when not availiable,
 * comment it out in the config.h, so its clear it was checked. */
void check_config_part(config_check_type type) {
  static thread_local char tmpfile[256];
  static thread_local char outfile[256];
  thread_local const char *data = NULL;
  thread_local Ulong datalen = 0, filelen = 0;
  thread_local int fd, ret;
  switch (type) {
    case CONFIG_HAVE___THREAD: {
      data =
        "#if defined(__GNUC__)\n"
        "void check__thread(void) {\n"
        "  static __thread int x;\n"
        "  x = 0;\n"
        "}\n"
        "#else\n"
        "# error \"No __thread support\"\n"
        "#endif\n"
        "\n"
        "int main(void) {\n"
        "  check__thread();\n"
        "}\n"
      ;
      datalen = (sizeof(
        "#if defined(__GNUC__)\n"
        "void check__thread(void) {\n"
        "  static __thread int x;\n"
        "  x = 0;\n"
        "}\n"
        "#else\n"
        "# error \"No __thread support\"\n"
        "#endif\n"
        "\n"
        "int main(void) {\n"
        "  check__thread();\n"
        "}\n"
        ) - 1
      );
      break;
    }
    default: {
      return;
    }
  }
  copy_stack_nstr(tmpfile, S__LEN("/tmp/Amake_check_"));
  copy_stack_nstr((tmpfile + (sizeof("/tmp/Amake_check_") - 1)), config_check_type_str(type), strlen(config_check_type_str(type)) + 1);
  filelen = strlen(tmpfile);
  copy_stack_nstr(outfile, tmpfile, filelen);
  copy_stack_nstr((tmpfile + filelen), S__LEN(".c") + 1);
  logI("tmpfile: %s", tmpfile);
  logI("outfile: %s", outfile);
  fd = open("/tmp/Amake_check___thread.c", (O_WRONLY | O_CREAT), 0755);
  if (fd < 0) {
    logE("Failed to create tmp file to test '%s'", config_check_type_str(type));
    return;
  }
  lock_fd(fd, F_WRLCK);
  write(fd, data, datalen);
  unlock_fd(fd);
  close(fd);
  ret = launch_bin("/usr/bin/cc", ARGV("/usr/bin/cc", "-o", outfile, tmpfile), PARENT_ENV);
  if (ret != 0) {
    logE("Failed.");
    unlink(tmpfile);
    return;
  }
  unlink(tmpfile);
  unlink(outfile);
}
