/*Code inspired on t50 */
#include <errno.h>
#include "netbasic.h"
#include "strix.h"
int 
create_socket( void )
{
  int fd, flag;
  uint32_t n = 1;

  if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1)
  {
    handle_fatal("Cannot open raw socket\n");
  }

/* Try to change the socket mode to NON BLOCKING. */
  if ((flag = fcntl(fd, F_GETFL)) == -1)
  {
    handle_fatal("Cannot get socket flags\n");
  }

  if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
  {
    handle_fatal("Cannot set socket to non-blocking mode\n");
  }


  /* Setting IP_HDRINCL. */
  /* NOTE: We will provide the IP header, but enabling this option, on linux,
           still makes the kernel calculates the checksum and total_length. */
  if ( setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) == -1 )
  {
    handle_fatal("Cannot set socket options\n");
  }

  return fd;
}

void 
close_socket( int fd)
{
  /* Close only if the descriptor is valid. */
  if (fd > 0)
  {
    close(fd);    // AS_SAFE!

    /* Added to avoid multiple socket closing. */
    fd = -1;
  }
}

bool send_packet(int socket, const void *buf, size_t len, struct sockaddr *saddr)
{
  if( unlikely( -1 == sendto(socket, buf, len, MSG_NOSIGNAL, saddr, sizeof(struct sockaddr_in)) ) ){
    if (errno == EPERM){
      handle_warning("Cannot send packet (Permission!?). Please check your firewall rules (iptables?).\n");
    }

    return false;
  }

  return true;
}

bool send_data(const void *buf, size_t len, struct sockaddr *saddr)
{
  int socket = create_socket();
  bool rtn;

  rtn = send_packet(socket, buf, len, saddr);
  close_socket(socket);
  return rtn;
}
