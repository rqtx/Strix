
#ifndef PLANNER_H
  #define PLANNER_H

#include "strix.h"

AttackPlan * Planner( AttackDraft *draft, int draftSize );
void DestroyPlan( AttackPlan **plan );

#endif
