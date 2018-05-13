/*
 *  strix.h
 */

#ifndef STRIX_H
  #define STRIX_H

#include <netinet/in.h>
#include "strix_defines.h"
#include "c.h"
#include "packetforge.h"
#include "error.h"

typedef struct AttackPlanData {
  char * target_ip;
  char * amp_ip;
  int target_port;
  int amp_port;
  bool harakiri; 
}AttackPlan;

typedef struct AttackData {
  Packet * getPacket;
  Packet * setPacket;
  AttackPlan * atkPlan;
}AttackData;

void executeAttack( AttackPlan * plan );

#endif
