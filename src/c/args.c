/** @file args.c

  @author  Melwin Svensson.
  @date    7-2-2025.

 */
#include "../include/cproto.h"


typedef struct {
  const char *short_name;  /* The short `name` of this cmd option argument line `-h`. */
  const char *long_name;   /* The thing that reprecents this cmd option argument like `--help`. */
  int argno;               /* The number of arguments this opt has, or -1 for dynamic number. */
  union {
    void (*voidfunc)(void);
    void (*argsfunc)(int argc, char **argv);
  } action;
} cmdopt_entry_t;

typedef struct {
  cmdopt_type_t type;
  int argc;     /* Only needed when `argsfunc` is used. */
  char **argv;  /* Will be split for use later. */
  union {
    void (*voidfunc)(void);
    void (*argsfunc)(int argc, char **argv);
  } action;
} cmdopt_action_t;


/* `INTERNAL`  The map that holds the configuration for the cmd opts. */
static cmdopt_entry_t cmdopt[] = {
  {  "-h",      "--help",  0, NULL },
  { "-cf", "--configure",  0, NULL },
  {  "-v",   "--version",  0, NULL },
  {  "-b",     "--build", -1, NULL },
  { "-cl",     "--clean",  0, { Amake_do_shallow_clean } },
  {  "-i",   "--install", -1, NULL },
  { "-lb",       "--lib",  1, NULL },
  {  "-t",      "--test",  0, NULL },
  {  "-l",      "--link", -1, NULL },
  { "-ch",     "--check",  0, NULL }
};


_UNUSED static cmdopt_action_t *cmdopt_action_create(void) {
  cmdopt_action_t *act = xmalloc(sizeof(*act));
  act->argc            = 0;
  act->argv            = NULL;
  act->action.voidfunc = NULL;
  return act;
}

_UNUSED static void cmdopt_action_free(void *arg) {
  cmdopt_action_t *act = arg;
  ASSERT(act);
  free(act->argv);
}

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
  ASSERT(argv);
  ASSERT(iter);
  char *ret = COPY_OF("");
  for (int i=(*iter + 1); i<(*iter + 1 + count) && i<argc; ++i) {
    ret = xstrcat(ret, argv[i]);
    ret = xstrncat(ret, S__LEN(" "));
  }
  return ret;
}

void test_args(int argc, char **argv) {
  char *args;
  int opt = -1;
  Ulong argno;
  for (int i=0; i<argc; ++i) {
    if (is_cmdopt(argv[i], &opt)) {
      argno = count_args(argc, argv, &i);
      if (cmdopt[opt].argno != -1 && cmdopt[opt].argno != (int)argno) {
        printf("Error: Argument: %s: Should have %d number of arguments.\n", argv[i], cmdopt[opt].argno);
        return;
      }
      if (argno) {
        args = get_args_string(argc, argv, &i, argno);
        printf("Found cmd opt: %s: argno: %d: args: %s\n", argv[i], (int)argno, args);
      }
      switch (opt) {
        case AMAKE_CLEAN: {
          Amake_do_shallow_clean();
          exit(0);
        }
        // case AMAKE_LINK: {
        //   if (argno) {
        //     array = split_string_len(args, ' ', &argno);
        //     free(args);
        //     Amake_do_link(argno, array);
        //     free(array);
        //   }
        //   exit(0);
        // }
      }
      if (argno) {
        free(args);
      }
    }
  }
}
