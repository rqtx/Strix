
#ifndef
  #define

typedef struct {
  int atkId;
  float currentThroughput;
  float expectedThroughput;
  int status;
}AtkInfo;

typedef struct {
  AtkInfo *data;
  int size;
}Monitor;

Monitor * GetMonitor();
#endif
