#include <signal.h>
#include <string.h>
#include "strix.h"
#include "error.h"

void handle_fatal( char * p_error )
{
  fputs(p_error, stderr);
  raise(SIGABRT);
}

void handle_warning (char * p_error)
{
  perror(p_error);
}
