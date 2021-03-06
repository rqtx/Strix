/*
 *  strix.h
 */

#ifndef STRIX_H
  #define STRIX_H

#include <netinet/in.h>
#include "strix.h"
#include "planner.h"
#include "packetforge.h"
#include "error.h"

void ExecuteAttack( AttackData * atkData );
void DestroyAttack( AttackData * atkData );

#endif
