/** @file args.c

  @author  Melwin Svensson.
  @date    7-2-2025.

 */
#include "../include/cproto.h"


typedef struct {
  const char   *short_name;  /* The short `name` of this cmd option argument line `-h`. */
  const char   *long_name;   /* The thing that reprecents this cmd option argument like `--help`. */
  int           argno;       /* The number of arguments this opt has, or -1 for dynamic number. */
} cmdopt_entry_t;

typedef struct {
  /* Only needed when `args_action` is used. */
  int   argc;
  /* Will be split for use later. */
  char *args;
  union {
    void (*void_action)(void);
    void (*args_action)(int, char **);
  };
} cmdopt_action_t;


/* `INTERNAL`  The map that holds the configuration for the cmd opts. */
static cmdopt_entry_t cmdopt[] = {
  {  "-h",      "--help",  0 },
  { "-cf", "--configure",  0 },
  {  "-v",   "--version",  0 },
  {  "-b",     "--build", -1 },
  { "-cl",     "--clean",  0 },
  {  "-i",   "--install", -1 },
  { "-lb",       "--lib",  1 },
  {  "-t",      "--test",  0 },
  {  "-l",      "--link", -1 },
  { "-ch",     "--check",  0 }
};


/* Return `TRUE` if `arg` is a cmd option argument. */
bool is_cmdopt(const char *arg, int *opt) {
  ASSERT(arg);
  for (Ulong i = 0; i < ARRAY_SIZE(cmdopt); ++i) {
    if (strcmp(arg, cmdopt[i].short_name) == 0
     || strcmp(arg, cmdopt[i].long_name)  == 0) {
      ASSIGN_IF_VALID(opt, i);
      return TRUE;
    }
  }
  return FALSE;
}

/* Return the number of args that does not match any of our opts begind `*iter` in argv. */
static int count_args(int argc, char **argv, int *iter) {
  ASSERT(argc);
  ASSERT(argv);
  ASSERT(iter);
  int count = 0;
  for (int i = (*iter + 1); i < argc; ++i) {
    if (is_cmdopt(argv[i], NULL)) {
      break;
    }
    else {
      ++count;
    }
  }
  return count;
}

/* Return a string of `count` args in `argv` after `*iter`. */
static char *get_args_string(int argc, char **argv, int *iter, int count) {
  char *ret = copy_of("");
  for (int i = (*iter + 1); i < (*iter + 1 + count); ++i) {
    ret = astrcat(ret, argv[i]);
    ret = astrcat(ret, " ");
  }
  return ret;
}

void test_args(int argc, char **argv) {
  int opt   = -1;
  int argno = -1;
  for (int i = 0; i < argc; ++i) {
    if (is_cmdopt(argv[i], &opt)) {
      argno = count_args(argc, argv, &i);
      if (cmdopt[opt].argno != -1 && cmdopt[opt].argno != argno) {
        printf("Error: Command: %s: Should have %d number of arguments.\n", cmdopt[opt].long_name, cmdopt[opt].argno);
        return;
      }
      switch (opt) {
        case AMAKE_CLEAN: {
          Amake_do_shallow_clean();
          exit(0);
          break;
        }
      }
      char *args = get_args_string(argc, argv, &i, argno);
      printf("Found cmd opt: %s: argno: %d: args: %s\n", argv[i], argno, args);
      free(args);
    }
  }
}
