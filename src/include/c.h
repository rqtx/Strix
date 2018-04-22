/*
 *  c.h
 *          Fundamental C definitions.
 */

#ifndef C_H
  #define C_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "error.h"
#include "memutils.h"

/*
  * Pointer
  *      Variable holding address of any memory resident object.
  *
  *      XXX Pointer arithmetic is done with this, so it can't be void *
  *      under "true" ANSI compilers.
  */
 typedef char *Pointer;

 /*
 * lengthof
 *      Number of elements in an array.
 */
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))

#endif              /* C_H */
