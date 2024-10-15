#include "../include/prototypes.h"

static Uint        tsize = 0;
static Uint        tcap  = 10;
static ThreadData *td    = nullptr;
static pthread_t  *t     = nullptr;

void init_threads(void) {
  t  = (pthread_t *)malloc(sizeof(pthread_t) * tcap);
  td = (ThreadData *)malloc(sizeof(ThreadData) * tcap);
}

void add_thread_data(const char *input, const char *output) {
  (tsize == tcap) ? ((tcap *= 2), (td = (ThreadData *)realloc(td, sizeof(ThreadData) * tcap)),
                     (t = (pthread_t *)realloc(t, sizeof(pthread_t) * tcap)))
                  : 0;
  ThreadData *data    = &td[tsize];
  Uint in_len  = strlen(input);
  Uint out_len = strlen(output);
  copy_stack_nstr(data->input, input, in_len);
  copy_stack_nstr(data->output, output, out_len);
  data->input[in_len]   = '\0';
  data->output[out_len] = '\0';
  ++tsize;
}

static void *thread_cc_work(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  char           cmd[PATH_MAX];
  snprintf(cmd, PATH_MAX, CC_COMPILER " -c %s " CC_DEFAULT_ARGS " -I%s -o %s", data->input, INCLUDE_DIR.c_str(),
           data->output);
  printf("%s\n", cmd);
  system(cmd);
  return nullptr;
}

static void *thread_c_work(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  char           cmd[PATH_MAX];
  snprintf(
      cmd, PATH_MAX, C_COMPILER " -c %s " C_DEFAULT_ARGS " -I%s -o %s", data->input, INCLUDE_DIR.c_str(), data->output);
  printf("%s\n", cmd);
  system(cmd);
  return nullptr;
}

void do_c(void) {
  init_threads();
  Ulong          n;
  DirEntry *files = files_in_dir(C_DIR.c_str(), &n);
  for (Ulong i = 0; i < n; ++i) {
    DirEntry *e = &files[i];
    if (e->type == 8) {
      extract_name_and_ext(e);
      char in[PATH_MAX];
      char out[PATH_MAX];
      snprintf(in, PATH_MAX, "%s/%s%s", C_DIR.c_str(), e->name, e->ext);
      snprintf(out, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, ".o");
      add_thread_data(in, out);
    }
  }
  free(files);
  for (Uint i = 0; i < tsize; ++i) {
    pthread_create(&t[i], nullptr, thread_c_work, &td[i]);
  }
  for (Uint i = 0; i < tsize; ++i) {
    pthread_join(t[i], nullptr);
  }
  free(t);
  free(td);
}

void do_compile(void) {
  init_threads();
  Ulong          n;
  DirEntry *files = files_in_dir(CPP_DIR.c_str(), &n);
  for (Ulong i = 0; i < n; ++i) {
    DirEntry *e = &files[i];
    /* File. */
    if (e->type == 8) {
      extract_name_and_ext(e);
      char input[PATH_MAX];
      char output[PATH_MAX];
      snprintf(input, PATH_MAX, "%s/%s%s", CPP_DIR.c_str(), e->name, e->ext);
      snprintf(output, PATH_MAX, "%s/%s%s", OBJ_DIR.c_str(), e->name, ".o");
      add_thread_data(input, output);
    }
    /* Dir. */
    else if (e->type == 4) {
      if (strcmp(e->file, ".") == 0 || strcmp(e->file, "..") == 0) {
        continue;
      }
      Ulong          sn;
      DirEntry *sub_files = files_in_dir(string(CPP_DIR + "/" + e->file).c_str(), &sn);
      for (Ulong si = 0; si < sn; ++si) {
        DirEntry *se = &sub_files[si];
        if (se->type == 8) {
          printf("file: %s\n", se->file);
          extract_name_and_ext(se);
          char input[PATH_MAX];
          char output[PATH_MAX];
          snprintf(input, PATH_MAX, "%s/%s/%s%s", CPP_DIR.c_str(), e->file, se->name, se->ext);
          snprintf(output, PATH_MAX, "%s/%s/%s%s", OBJ_DIR.c_str(), e->file, se->name, ".o");
          if (!dir_exists(string(OBJ_DIR + "/" + e->file).c_str())) {
            mkdir(string(OBJ_DIR + "/" + e->file).c_str(), 0777);
          }
          add_thread_data(input, output);
        }
      }
    }
  }
  free(files);
  for (Uint i = 0; i < tsize; ++i) {
    pthread_create(&t[i], nullptr, thread_cc_work, &td[i]);
  }
  for (Uint i = 0; i < tsize; ++i) {
    pthread_join(t[i], nullptr);
  }
  free(t);
  free(td);
  printf("Total threads: %u\n", tsize);
  tsize = 0;
  do_c();
}
