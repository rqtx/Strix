
#ifndef NETBASIC_H
  #define NETBASIC_H

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "strix.h"

int create_socket( void );

void close_socket( int fd);

bool send_packet(int socket, const void *buf, size_t len, struct sockaddr *saddr);

bool send_data(const void *buf, size_t len, struct sockaddr *saddr);

#endif      //NETBASIC_H
