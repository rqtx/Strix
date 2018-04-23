#include <argp.h>
#include <argz.h> 

#include "strix.h"
#include "cli.h"

#define DELIM "."

const char *argp_program_bug_address = "rqtx@protonmail.com";
const char *argp_program_version = "version 1.0"; 

struct argp_option options[] =
  {
    { "target", 't', "target_ipv4", 0, "Attack target IPV4"},
    { "amplifier", 'a', "amp_ipv4", 0, "Memcached amplifier IPV4"},
    { 0 }
  };

AttackPlan * plan = NULL;

static int
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
  {
  case 't': 
    plan->target = arg;   
    break;
  case 'a': 
    plan->amp = arg;
    break;
 }
 return 0;
}


/* return true if string contain only digits, else return false */
static int 
valid_digit(Pointer ip_str)
{
  while (*ip_str) {
    if (*ip_str >= '0' && *ip_str <= '9'){
      ++ip_str;
    }
    else{
      return false;
    }
  }
  return true;
}
 
/* return true if IP string is valid, else return false */
static bool 
is_valid_ipv4(Pointer ip_str)
{
  int num, dots = 0;
  char ip_copy[16];
  Pointer ptr;
  
  if (ip_str == NULL){
    return false;
  }
  
  memcpy(ip_copy, ip_str, strlen(ip_str));
  ptr = strtok(ip_copy, DELIM);
 
  if (ptr == NULL){
    return false;
  }
 
  while (ptr) {
 
    /* after parsing string, it must contain only digits */
    if (!valid_digit(ptr)){
      return false;
    }

    num = atoi(ptr);
 
    /* check for valid IPV4 */
    if (num >= 0 && num <= 255) {
    /* parse remaining string */
    ptr = strtok(NULL, DELIM);
    if (ptr != NULL)
      ++dots;
    } 
    else{
      return false;
    }
    
  }
 
  /* valid IPV4 string must contain 3 dots */
  if (dots != 3){
    return false;
  }

  return true;
}

static void
plan_validate( void )
{
  assert( NULL != plan );

  if( NULL == plan->target || !is_valid_ipv4(plan->target )){
    fatal_error("FATAL:Invalid target IPV4\n");
  }

  if( NULL == plan->amp || !is_valid_ipv4(plan->amp )){
    fatal_error("FATAL:Invalid amplifier IPV4\n");
  }
}

AttackPlan * 
createAttackPlan(int argc, char ** argv)
{

  struct argp argp = { options, parse_opt, 0, 0 };
  memalloc((void *)&plan, sizeof(AttackPlan), __func__);
  argp_parse (&argp, argc, argv, 0, 0, 0);
  plan_validate(); 
  return plan;
}