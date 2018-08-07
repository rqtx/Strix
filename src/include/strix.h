/*
 *  strix_defines.h
 *    
 */

#ifndef STRIX_DEFINES_H
  #define STRIX_DEFINES_H

#include <arpa/inet.h>
#include "c.h"
#include "error.h"
#include "memutils.h"

#ifndef unlikely
#define unlikely(c) __builtin_expect(!!(c), 0)
#endif
#ifndef likely
#define likely(c) __builtin_expect(!!(c), 1)
#endif

typedef struct  PacketData {
  Pointer packet_ptr;
  size_t size;
  int dest_port;
  struct sockaddr_in *saddr;
  uint16_t type;
}Packet;


#endif
