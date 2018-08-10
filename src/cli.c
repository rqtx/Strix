#include <argp.h>
#include <argz.h> 

#include "strix.h"
#include "cli.h"
#include "manager.h"
#include "planner.h"

#define DELIM "."

const char *argp_program_bug_address = "rqtx@protonmail.com";
const char *argp_program_version = "version 1.0"; 

struct argp_option options[] =
  {
    { "target", 't', "target_ipv4", 0, "Attack target IPV4"},
    { "amplifier", 'a', "amp_ipv4", 0, "Memcached amplifier IPV4"},
    { "throughput", 'p', "thp", 0, "Attack throughput"},
    { 0 }
  };

AttackDraft * draft = NULL;

static int
parse_opt (int key, char *arg, struct argp_state *state)
{ 
  switch (key)
  {
    case 't': 
      draft->target_ip = arg;   
      break;
    case 'a': 
      draft->amp_ip = arg;
      break;
    case 'p':
      draft->initialThroughput = (int) *arg;
      break;
  }
  return 0;
}

AttackDraft * 
createAttackDraft(int argc, char ** argv)
{

  struct argp argp = { options, parse_opt, 0, 0 };
  draft = (AttackDraft *) CreateDefaultDraft( "", "" );
  argp_parse (&argp, argc, argv, 0, 0, 0);
  return draft;
}
