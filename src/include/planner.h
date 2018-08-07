
#ifndef PLANNER_H
  #define PLANNER_H

#include "strix.h"

typedef struct {
  char * target_ip;
  char * amp_ip;
  int target_port;
  int amp_port;
  float throughput; 
}AttackDraft;

typedef struct {
  Packet * getPacket;
  Packet * setPacket;
  float throughput; 
  char * target_ip;
  char * amp_ip;
}AttackData;

typedef struct {
  AttackData **atkPlans;
  int totalPlans;
}AttackPlan; 

AttackPlan * Planner( AttackDraft *draft, int draftSize );

#endif
