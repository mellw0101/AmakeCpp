/** @file signal.c

  @author  Melwin Svensson.
  @date    7-2-2025.

 */
#include "../include/cproto.h"

/* Storage containers for the original and the temporaty handler for `SIGINT`. */
__thread struct sigaction old_SIGINT_action, new_SIGINT_action;
/* Flag that is set by `install_SIGINT_handler`, and reset by `restore_SIGINT_handler`. */
__thread bool SIGINT_default = TRUE;

/* Set a new handler for `SIGINT`. */
void install_SIGINT_handler(void (*handler)(int)) {
  if (!SIGINT_default) {
    restore_SIGINT_handler();
  }
  new_SIGINT_action.sa_handler = handler;
  new_SIGINT_action.sa_flags   = 0;
  sigaction(SIGINT, &new_SIGINT_action, &old_SIGINT_action);
  SIGINT_default = FALSE;
}

/* Restore the original handler for `SIGINT`. */
void restore_SIGINT_handler(void) {
  sigaction(SIGINT, &old_SIGINT_action, NULL);
  SIGINT_default = TRUE;
}

