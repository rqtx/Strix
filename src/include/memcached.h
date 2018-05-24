
/*
 *  memcached_masks.h
 *    Memcached packet masks structs
 *    https://github.com/memcached/memcached/wiki/BinaryProtocolRevamped
 */

#ifndef MEMCACHED_H
  #define MEMCACHED_H

#define MEMCACHED_DEFAULT_PORT 11211
#define MEMCACHED_KEY 0xABADF00D
#define MAXSIZE_MEMCACHED_KEY 0x0004
#define MAXSIZE_MEMCACHED_VALUE 0x500 //0x100000

typedef struct MemcachedRequestHeaderData {
  uint8_t magic;            //Magic number identifying the package
  uint8_t opcode;           //Command code
  uint16_t key_length;      //Key length
  uint8_t extras_length;    //Extras length
  uint8_t data_type;        //Data type
  uint16_t vbucket_id;      //vbucket id
  uint32_t tot_length;      //Total body length
  uint32_t opaque;          //Opaque
  uint64_t cas;             //CAS
}MemcachedRequestHeader;

typedef struct MemcachedSetExtraData {
  uint32_t flags;
  uint32_t expiration;
}MemcachedSetExtra;

#endif
