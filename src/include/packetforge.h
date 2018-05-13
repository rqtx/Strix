
/*
 *  packetforge.h
 *          
 */

#ifndef PACKETFURNACE_H
  #define PACKETFURNACE_H

#define GetIpHeader(X)  ( (struct iphdr *) (X) )
#define GetUdpHeader(X) ( (struct udphdr *) (X + sizeof(struct iphdr)) )

/* Standard well-defined IP protocols.  
 *        netinet/in.h
 
 Protocols RFC 790
 
IPPROTO_IP		    0		dummy for IP
IPPROTO_ICMP		1	    control message protocol 
IPPROTO_GGP		    3		gateway^2 (deprecated) 
IPPROTO_TCP		    6		tcp 
IPPROTO_EGP		    8		exterior gateway protocol 
IPPROTO_PUP		    12		pup 
IPPROTO_UDP		    17		user datagram protocol 
IPPROTO_IDP		    22		xns idp 
IPPROTO_TP		    29 		tp-4 w/ class negotiation 
IPPROTO_EON		    80		ISO cnlp 

IPPROTO_RAW		    255		raw IP packet 
IPPROTO_MAX		    256
*/
#define MEMCACHED_GET 200
#define MEMCACHED_SET 201

typedef struct  PacketData {
  Pointer packet_ptr;
  size_t size;
  uint16_t type;
}Packet;

void release_packet( Packet ** pkt);

Packet * forgeUDP(char * ip_dest, char * ip_src, int dest_port, int src_port, char *payload_data, int payload_size);
Packet * forgeMemcached( int opcode );

#endif
