/** @file cdef.h

  @author  Melwin Svensson.
  @date    2-2-2025.

 */
#pragma once

/* Amake */
#include "aassert.h"

/* stdlib */
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* Define c linkage macros. */
#ifdef __cplusplus
# define _BEGIN_C_LINKAGE  extern "C" {
# define _END_C_LINKAGE    }
#else
# define _BEGIN_C_LINKAGE
# define _END_C_LINKAGE
#endif

/* Define boolian macros. */
#ifdef __cplusplus
# define TRUE   true
# define FALSE  false
#else
# define TRUE   1
# define FALSE  0
#endif

#ifndef _NORETURN
# define _NORETURN  __attribute__((__noreturn__))
#endif
