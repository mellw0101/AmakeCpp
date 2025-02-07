/** @file signal.c

  @author  Melwin Svensson.
  @date    7-2-2025.

 */
#include "../include/cproto.h"

/* Storage containers for the original and the temporaty handler for `SIGINT`. */
static struct sigaction old_SIGINT_action, new_SIGINT_action;

void install_SIGINT_handler(void (*handler)(int)) {
  new_SIGINT_action.sa_handler = handler;
  new_SIGINT_action.sa_flags   = 0;
  sigaction(SIGINT, &new_SIGINT_action, &old_SIGINT_action);
}

void restore_SIGINT_handler(void) {
  sigaction(SIGINT, &old_SIGINT_action, NULL);
}

