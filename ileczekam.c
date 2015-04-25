#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <inttypes.h>

#include "err.h"

#define _DEBUG 1
#define BUFFER_SIZE 1000

#define ARG_CONNECTION 1
#define ARG_HOSTNAME   2
#define ARG_PORT       3

const char* usage_error = "Usage: $./ileczekam -<connection_type: u, t> <hostname> <port>";

void test_tcp_connection(char *hostname, const int port);
void test_udp_connection(char *hostname, const int port);

uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

int main(int argc, char *argv[])
{   
    
    int i;
    ssize_t len, rcv_len;
    
    if (argc != 4)
        fatal(usage_error);
    
    
    
    test_udp_connection(argv[2], atoi(argv[3]));
    
    
    return 0;
}

void test_udp_connection(char *hostname, const int port)
{
    int sock;
    struct addrinfo addr_hints;
    struct addrinfo *addr_result;
    
    int i, flags, sflags;
    char buffer[BUFFER_SIZE];
    
    
    /* hello */
    
    size_t len;
    ssize_t snd_len, rcv_len;
    struct sockaddr_in my_address;
    struct sockaddr_in srvr_address;
    socklen_t rcva_len;
    
    /* converting host/port in string to struct addrinfo */
    (void) memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;
    if (getaddrinfo(hostname, NULL, &addr_hints, &addr_result) != 0)
    {
	syserr("getaddrinfo");
    }
    
    my_address.sin_family = AF_INET; // IPv4
    my_address.sin_addr.s_addr =
	((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    my_address.sin_port = htons((uint16_t) port);
    
    freeaddrinfo(addr_result);
    
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
	syserr("socket");
    
    /* code */
    sflags = 0;
    rcva_len = (socklen_t) sizeof(my_address);
    
    printf("%" PRId64 "\n", GetTimeStamp());
    printf("%d\n", sizeof(uint64_t));
    
    uint64_t curtime = htonl(GetTimeStamp());
    snd_len = sendto(sock, &curtime, sizeof(uint64_t), sflags, (struct sockaddr *) &my_address, rcva_len);
    if (snd_len != (ssize_t) sizeof(uint64_t)) {
      syserr("partial / failed write");
    }
    
    (void) memset(buffer, 0, sizeof(buffer));
    flags = 0;
    len = (size_t) sizeof(buffer) - 1;
    rcva_len = (socklen_t) sizeof(srvr_address);
    rcv_len = recvfrom(sock, buffer, len, flags,
        (struct sockaddr *) &srvr_address, &rcva_len);

    if (rcv_len < 0) {
      syserr("read");
    }
    (void) printf("read from socket: %zd bytes: %s\n", rcv_len, buffer);
    
    
    
    if (close(sock) == -1)
	syserr("close");
    
}