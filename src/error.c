
#include "c.h"
#include "error.h"

void handle_fatal( char * p_error )
{
  perror(p_error);
  fputs(p_error, stderr);
  exit(EXIT_FAILURE);
}

void handle_warning (char * p_error)
{
  perror(p_error);
}
