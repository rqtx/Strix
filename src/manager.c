
#include "manager.h"
#include "planner.h"
#include "executor.h"
#include "injector/injector.h"

int * injectorsID = NULL;

void StrixManager( void * draft, int draftSize)
{
  AttackPlan * plan;
  
  int executionSize = draftSize;

  //TODO:validade draft
  
  plan = Planner( draft, draftSize );

  injectorsID = executeAttack( plan );
  
  while(1){
    //check for interruption
  }

//TODO:release_memory:
  
}


int *GetInjectorsId()
{
  return injectorsID;
}
