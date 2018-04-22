#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "c.h"
#include "PacketForge.h"

 uint16_t ip_checksum(const void *buf, size_t hdr_len)
{
        unsigned long sum = 0;
        const uint16_t *ip1;
 
         ip1 = buf;
         while (hdr_len > 1)
         {
                 sum += *ip1++;
                 if (sum & 0x80000000)
                         sum = (sum & 0xFFFF) + (sum >> 16);
                 hdr_len -= 2;
         }
 
         while (sum >> 16)
                 sum = (sum & 0xFFFF) + (sum >> 16);
 
         return(~sum);
 }

PacketPlan * createUdpPlan(  char * ip_dest, char * ip_src, int port )
{
  PacketPlan * plan;
    
  plan = NULL; //keep compiler quiet
  return plan;
}

Packet * forge( char * ip_dest, char * ip_src, int port)
{


  Packet *pac;
  Pointer datagram;                              // Datagram to represent the packet
  Pointer payload;
    
  memalloc( (void *)&pac, sizeof(Packet), __func__ );
  memalloc( (void *)&datagram, sizeof(Pointer) * 4096, __func__ );

  struct iphdr *ip_header = (struct iphdr *)datagram;                                  // Pointer to the beginning of ip header
  struct udphdr *udp_header = (struct udphdr *)(datagram + sizeof(struct iphdr));   // Pointer to the beginning of udp header
    
  payload = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);               // Pointer to the beginning of payload
    
  strcpy(payload, "PacketFurnace");
    

  //IP Header
  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(payload);
  ip_header->tos = 0;
  ip_header->frag_off = 0;		    // no fragment = 0
  ip_header->ttl = 64;			    // default value = 64
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = ip_checksum(datagram, ip_header->tot_len);
  inet_pton(AF_INET, ip_dest, (struct in_addr *)&ip_header->daddr);
  inet_pton(AF_INET, ip_src, (struct in_addr *)&ip_header->saddr);

  //UDP header
  udp_header->source = htons(6666);
  udp_header->dest = htons(port);
  udp_header->len = htons(sizeof(struct udphdr) + strlen(payload));
  udp_header->check = 0;

  pac->packet_ptr = datagram;
  pac->size = ip_header->tot_len;
  pac->type = IPPROTO_UDP;

  return pac;
}

