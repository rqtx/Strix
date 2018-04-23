/*
 *  netio.h
 *    Socket interface functions
 */

#ifndef STRIX_H
  #define STRIX_H

#include <netinet/in.h>
#include "strix_defines.h"
#include "c.h"
#include "error.h"

typedef struct AttackPlanData {
  char * target;
  char * amp;
}AttackPlan;

typedef struct  PacketData {
  Pointer packet_ptr;
  struct sockaddr_in sin;
  int socket;
  size_t size;
  uint16_t type;

}Packet;

void executeAttackPlan( AttackPlan * plan );
int create_socket( void );
void close_socket( int );
bool send_packet( Packet * pkt);

#endif
