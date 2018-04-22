

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "c.h"
#include "netio.h"

int create_socket( void )
{
  int fd, flag;
  uint32_t n = 1;

  if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1)
  {
    fatal_error("Cannot open raw socket");
  }

/* Try to change the socket mode to NON BLOCKING. */
  if ((flag = fcntl(fd, F_GETFL)) == -1)
  {
    fatal_error("Cannot get socket flags");
  }

  if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
  {
    fatal_error("Cannot set socket to non-blocking mode");
  }


  /* Setting IP_HDRINCL. */
  /* NOTE: We will provide the IP header, but enabling this option, on linux,
           still makes the kernel calculates the checksum and total_length. */
  if ( setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) == -1 )
  {
    fatal_error("Cannot set socket options");
  }

  return fd;
}


void close_socket( int fd)
{
  /* Close only if the descriptor is valid. */
  if (fd > 0)
  {
    close(fd);    // AS_SAFE!

    /* Added to avoid multiple socket closing. */
    fd = -1;
  }
}
