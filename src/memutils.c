
#include "memutils.h"
#include "error.h"

void memalloc( void ** p_ptr, size_t p_size, const char * const p_func )
{
  if( NULL != p_ptr && NULL == *p_ptr )
  {
    *p_ptr = malloc( p_size );
        
    if( NULL == *p_ptr )
    {
      char error[50];
      snprintf(error, 50, "[%s] out of memory", p_func);
      fatal_error(error);
    }

    memset(*p_ptr, 0, p_size);
  }
}
