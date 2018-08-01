/*
 *  strix_defines.h
 *    
 */

#ifndef STRIX_DEFINES_H
  #define STRIX_DEFINES_H


#ifndef unlikely
#define unlikely(c) __builtin_expect(!!(c), 0)
#endif
#ifndef likely
#define likely(c) __builtin_expect(!!(c), 1)
#endif

#define THROUGHPUT_DEFAULT 1000000 // bytes por s

#endif
