#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "err.h"

#define _DEBUG 1
#define BUFFER_SIZE 1000

#define ARG_CONNECTION 1
#define ARG_HOSTNAME   2
#define ARG_PORT       3

const char* usage_error = "Usage: $./ileczekam -<connection_type: u, t> <hostname> <port>";

void test_tcp_connection(char *hostname, const int port);
void test_udp_connection(char *hostname, const int port);

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
    
    if (close(sock) == -1)
	syserr("close");
    
}