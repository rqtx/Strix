
#include "c.h"
#include "error.h"

void fatal_error( char * p_error )
{
  perror(p_error);
  exit(EXIT_FAILURE);
}
