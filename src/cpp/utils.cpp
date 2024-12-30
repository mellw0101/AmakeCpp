#include <dirent.h>
#include "../include/prototypes.h"

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
