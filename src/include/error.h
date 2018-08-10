/*
 *  error.h
 *
 */

#ifndef ERROR_H
  #define ERROR_H

#include <errno.h>

void handle_fatal( char * p_error );
void handle_warning( char * p_error );

#endif
