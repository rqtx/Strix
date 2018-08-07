#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include "strix.h"
#include "netbasic.c"
#include "injector/injector.h"

#define MAX_INJECTOR_THREADS 3
#define ID_Injector 0
#define ID_Monitor 1
#define ID_TrafficShaping 2
#define SAMPLING 5
#define MEGABYTE 1000000
#define DEFAULT_SIZE 1

InjectionArray injectArray = {
  .array = NULL,
  .free = NULL,
  .size = 0
};

static struct timespec onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static volatile sig_atomic_t _control = true;


static void sigint_handler( int signum )
{
  fprintf(stdout, "\nShutdown signal received\n");
}

static void *_trafficshapingHandler( void *arg )
{
  int *id = (int *)arg; 

  while(1){
    injectArray.array[*id]->bucketSize = (injectArray.array[*id]->throughputExpected * MEGABYTE) / injectArray.array[*id]->pkt->size;
    nanosleep(&onesec, NULL);
  }

  return NULL; //kepp compiler quiet
}

static void *_throughputMonitorHandler( void *arg )
{
  int *id = (int *)arg; 
  float sample = 0; 
  
  while(1){
    for(int i = 0; i < SAMPLING; i++){ 
      injectArray.array[*id]->pktCounter = 0;
      nanosleep(&onesec, NULL);
      sample += injectArray.array[*id]->pkt->size * injectArray.array[*id]->pktCounter;
    }
 
    injectArray.array[*id]->throughputCurrent = (sample/SAMPLING)/MEGABYTE;
    sample = 0;
  };

  return NULL; //Keep compiler quiet
}

static void *_injectorHandler( void *arg )
{  
  
  int *id = (int *)arg; 

  while(1){
    while( injectArray.array[*id]->bucketSize || ( injectArray.array[*id]->throughputExpected <= 0 ) ){
      send_packet(injectArray.array[*id]->socket, injectArray.array[*id]->pkt->packet_ptr, injectArray.array[*id]->pkt->size, (struct sockaddr *)injectArray.array[*id]->pkt->saddr);
      injectArray.array[*id]->bucketSize--;
      injectArray.array[*id]->pktCounter++;
    }
  };

  return NULL; //Keep compiler quiet
}

int CreateInjection(Packet *pkt, float thp)
{

  Injection *newInject;
  int newInjectId = -1;

  if( NULL == injectArray.array){
    memalloc( (void *)&injectArray.array, sizeof(Injection) * DEFAULT_SIZE, __func__);
    memalloc( (void *)&injectArray.free, sizeof(int) * DEFAULT_SIZE, __func__);
    injectArray.size = DEFAULT_SIZE;
  }

  for( int i = 0; i < injectArray.size; i++){
    if(injectArray.free[i] == 0){
      newInjectId = i;
      break;
    }
  }

  if( newInjectId == -1){
    //realloc
  }

  memalloc( (void *)&newInject,  sizeof( Injection ), __func__); 
  memalloc( (void *)&(newInject->threads_id), sizeof(int) * MAX_INJECTOR_THREADS, __func__);

  newInject->throughputExpected = thp;
  newInject->throughputCurrent = 0;
  newInject->bucketSize = 0;
  newInject->pkt = pkt;
  newInject->socket = create_socket();
  newInject->pktCounter = 0;

  injectArray.array[newInjectId] = newInject;
  injectArray.free[newInjectId] = 1; 
  return newInjectId;

}

void InjectionStart(int id)
{
  
  if(id < 0 && id > injectArray.size && injectArray.free[id] != 1){
    return;
  }

  if( 0 != pthread_create(&(injectArray.array[id]->threads_id[ID_Injector]), NULL, _injectorHandler, &id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }

  if( 0 != pthread_create(&(injectArray.array[id]->threads_id[ID_Monitor]), NULL, _throughputMonitorHandler, &id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }

  if( 0 != pthread_create(&(injectArray.array[id]->threads_id[ID_TrafficShaping]), NULL, _trafficshapingHandler, &id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }
}
