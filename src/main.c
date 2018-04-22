
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "c.h"
#include "PacketForge.h"
#include "netio.h"

int main(int argc, char *argv[]){
    
    int s;
    struct sockaddr_in sin;

    fprintf(stdout, "Strix\n");
     
    Packet * pkt = forge("192.168.0.145", "192.168.0.1", 6666);
    //struct iphdr * ip_header = GetIpHeader(pkt->packet_ptr);
    //struct udphdr * udp_header = GetUdpHeader(pkt->packet_ptr);
    
    s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(6666);
    sin.sin_addr.s_addr = inet_addr ("192.168.0.1");
    
    if(sendto(s, pkt->packet_ptr, pkt->size, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0){
        perror("sendto failed"); 
    }
    
    free(pkt->packet_ptr);
    free(pkt);
    return 0;
}
