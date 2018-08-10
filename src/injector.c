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

#define SPACE_OCCUP 0
#define SPACE_FREE 1

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

static void *_trafficshapingHandler( void *arg )
{
  int *aux = (int *) (arg);
  int id = *aux;

  while(1){
    injectArray.array[id]->bucketSize = (injectArray.array[id]->throughputExpected * MEGABYTE) / injectArray.array[id]->pkt->size;
    nanosleep(&onesec, NULL);
  }

  return NULL; //kepp compiler quiet
}

static void *_throughputMonitorHandler( void *arg )
{
  int *aux = (int *) (arg);
  int id = *aux;

  float sample = 0; 
  
  while(1){
    for(int i = 0; i < SAMPLING; i++){ 
      injectArray.array[id]->pktCounter = 0;
      nanosleep(&onesec, NULL);
      sample += injectArray.array[id]->pkt->size * injectArray.array[id]->pktCounter;
    }
 
    injectArray.array[id]->throughputCurrent = (sample/SAMPLING)/MEGABYTE;
    sample = 0;
  };

  return NULL; //Keep compiler quiet
}

static void *_injectorHandler( void *arg )
{  
  int *aux = (int *) (arg);
  int id = *aux;

  while(1){
    while( injectArray.array[id]->bucketSize || ( injectArray.array[id]->throughputExpected <= 0 ) ){
      pthread_mutex_lock(&injectArray.array[id]->lock);
      send_packet(injectArray.array[id]->socket, injectArray.array[id]->pkt->packet_ptr, injectArray.array[id]->pkt->size, (struct sockaddr *)injectArray.array[id]->pkt->saddr);
      injectArray.array[id]->bucketSize--;
      injectArray.array[id]->pktCounter++;
      pthread_mutex_unlock(&injectArray.array[id]->lock);
    }
  };

  return NULL; //Keep compiler quiet
}

static void injectionStart(int *id)
{

  if(*id < 0 && *id > injectArray.size && injectArray.free[*id] != 1){
    return;
  }

  pthread_mutex_init(&injectArray.array[*id]->lock, NULL);
  InjectionPause(*id); //Started by default

  if( 0 != pthread_create(&(injectArray.array[*id]->threads_id[ID_Injector]), NULL, _injectorHandler, id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }

  if( 0 != pthread_create(&(injectArray.array[*id]->threads_id[ID_Monitor]), NULL, _throughputMonitorHandler, id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }

  if( 0 != pthread_create(&(injectArray.array[*id]->threads_id[ID_TrafficShaping]), NULL, _trafficshapingHandler, id) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    handle_fatal(buffer);
  }
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
    //TODO:realloc?error?
  }

  memalloc( (void *)&newInject,  sizeof( Injection ), __func__); 
  memalloc( (void *)&(newInject->threads_id), sizeof(int) * MAX_INJECTOR_THREADS, __func__);

  newInject->throughputExpected = thp;
  newInject->throughputCurrent = 0;
  newInject->bucketSize = 0;
  newInject->pkt = pkt;
  newInject->socket = create_socket();
  newInject->pktCounter = 0;
  newInject->injectorId = newInjectId;

  injectArray.array[newInjectId] = newInject;
  injectArray.free[newInjectId] = SPACE_OCCUP;
  
  injectionStart( &(newInject->injectorId) );
  return newInjectId;

}

void InjectionPause(int id)
{
  if( injectArray.free[id] != SPACE_OCCUP){
    return;
  }

  pthread_mutex_lock(&injectArray.array[id]->lock);
  injectArray.array[id]->injectionStatus = INJECTION_PAUSED;
}

void InjectionResume(int id)
{
  if( injectArray.free[id] != SPACE_OCCUP){
    return;
  }

  pthread_mutex_unlock(&injectArray.array[id]->lock);
  injectArray.array[id]->injectionStatus = INJECTION_RUNNING;
}

void InjectionDestroy(int id)
{
  if( injectArray.free[id] != SPACE_OCCUP){
    return;
  }

  //The pkt pointer was not crated here and is not a good ideia free it here.
  
  for(int i = 0; i < MAX_INJECTOR_THREADS; i++)
  {
    pthread_cancel(injectArray.array[id]->threads_id[ID_TrafficShaping]); 
  }

  pthread_mutex_destroy(&injectArray.array[id]->lock);

  free(injectArray.array[id]);

  injectArray.free[id] = SPACE_FREE;
}

void InjectionNewThroughput(int id, int newThroughput)
{
  if( injectArray.free[id] != SPACE_OCCUP){
    return;
  }
  injectArray.array[id]->throughputExpected = newThroughput;
}

float InjectionCurrentThroughput(int id)
{
  if( injectArray.free[id] != SPACE_OCCUP){
    return 0;
  }
  return injectArray.array[id]->throughputCurrent; 
}
