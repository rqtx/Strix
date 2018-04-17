/*
 *  c.h
 *          Fundamental C definitions.
 */

#ifndef C_H
  #define C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

/*
 * lengthof
 *      Number of elements in an array.
 */
#define lengthof(array) (sizeof (array) / sizeof ((array)[0]))

#endif              /* C_H */
