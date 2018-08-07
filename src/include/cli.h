
/*
 * cli.h
 *  Command Line Interface routines
 */

#ifndef CLI_H
  #define CLI_H

#include "strix.h"
#include "executor.h"

#define THROUGHPUT_DEFAULT 0

AttackDraft * createAttackDraft(int argc, char ** argv);

#endif
