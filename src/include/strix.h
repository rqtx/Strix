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

#define Attack_Status_Created 0
#define Attack_Status_Running 1
#define Attack_Status_Finished 2

#define MEMCACHED_DEFAULT_PORT 11211
#define THROUGHPUT_DEFAULT 0

typedef struct  PacketData {
  Pointer packet_ptr;
  size_t size;
  int dest_port;
  struct sockaddr_in *saddr;
  uint16_t type;
}Packet;

typedef struct {
  char * target_ip;
  char * amp_ip;
  int target_port;
  int amp_port;
  float initialThroughput;
  float incrementThroughput;
  int timeFrequency;
  int nInjections;
}AttackDraft;

typedef struct {
  pthread_t executorId;
  pthread_t injectorId;
  int atkStatus;
  Packet * getPacket;
  Packet * setPacket; 
  char * target_ip;
  char * amp_ip;
  float initialThroughput;
  int incrementThroughput;
  int currentThroughput;
  int timeFrequency;          //Time of throughput increment frequency in seconds
  int nInjections;            //Number of injections, INF == 0 
}AttackData;

typedef struct {
  AttackData **atkPlans;
  int totalPlans;
}AttackPlan; 

#endif
