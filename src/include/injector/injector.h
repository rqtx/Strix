
#ifndef INJECTOR_H
  #define INJECTOR_H

#include <pthread.h>

#include "strix.h"

#define INJECTION_RUNNING 0
#define INJECTION_PAUSED 1


typedef struct InjectionData {
  int injectorId;
  pthread_t *threads_id;
  float throughputExpected; //Mb/s (MAX == 0)
  float throughputCurrent;
  int bucketSize;
  Packet *pkt;
  int socket;
  int pktCounter;
  int injectionStatus;
  pthread_mutex_t lock;
}Injection;

typedef struct {
  Injection **array;
  int *free;
  int size;
}InjectionArray;

/**
 * @brief Create an injection.
 *
 * @param pkt[in] Packet pointer to inject.
 * @param thp[in] Throughput expected
 * @return Injection Id
 */
int CreateInjection(Packet *pkt, float thp);
void InjectionResume(int id);
void InjectionPause(int id);
void InjectionDestroy(int id);
void InjectionNewThroughput(int id, int newThroughput);
float InjectionCurrentThroughput(int id);

#endif          /*INJECTOR_H*/
