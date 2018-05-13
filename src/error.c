
#include "c.h"
#include "error.h"

void fatal_error( char * p_error )
{
  perror(p_error);
  fputs(p_error, stderr);
  exit(EXIT_FAILURE);
}

void warning (char * p_error)
{
  perror(p_error);
}
