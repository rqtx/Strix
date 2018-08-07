
#ifndef INJECTOR_H
  #define INJECTOR_H

#include <pthread.h>

#include "strix.h"

typedef struct InjectionData {
  pthread_t *threads_id;
  float throughputExpected; //Mb/s (MAX == 0)
  float throughputCurrent;
  int bucketSize;
  Packet *pkt;
  int socket;
  int pktCounter;
}Injection;

typedef struct {
  Injection **array;
  int *free;
  int size;
}InjectionArray;

int CreateInjection(Packet *pkt, float thp);
void InjectionStart(int id);

#endif          /*INJECTOR_H*/
