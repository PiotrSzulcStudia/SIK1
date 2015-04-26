/* Piotr Szulc ps347277 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "err.h"
#include "timer.h"

#define ARG_CONNECTION      1
#define ARG_HOSTNAME        2
#define ARG_PORT            3
#define BUFFER_SIZE      1000


static const char* usage_error = "Usage: $./ileczekam -<connection_type: u, t> <hostname> <port>";

void safe_atoi(char * str);
void test_tcp_connection(char *hostname, char *port);
void test_udp_connection(char *hostname, const int port);

int main(int argc, char *argv[])
{   
    if (argc != 4)
	fatal(usage_error);
    
    int port = atoi(argv[ARG_PORT]);
    if ( (PORT_LOW_BOUND >= port ) || (port >= PORT_UP_BOUND))
	fatal("invalid port number");
    
    if (strcmp(argv[ARG_CONNECTION], "-u") == 0)
	test_udp_connection(argv[ARG_HOSTNAME], port);
    else if (strcmp(argv[ARG_CONNECTION], "-t") == 0)
	test_tcp_connection(argv[ARG_HOSTNAME], argv[ARG_PORT]);
    else
	fatal(usage_error);
    
    return 0;
}

void test_tcp_connection(char *hostname, char *port)
{
    int sock, err;
    struct addrinfo addr_hints;
    struct addrinfo *addr_result;

    memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;
    err = getaddrinfo(hostname, port, &addr_hints, &addr_result);
    if (err != 0)
	syserr("getaddrinfo: %s\n", gai_strerror(err));
  
    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0)
	syserr("socket");
  
    /* Starting timer before trying to connect */
    uint64_t start_time = GetTimeStamp();
    
    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
	syserr("connect");
  
    /* Stoping timer after establishing connection */
    uint64_t end_time = GetTimeStamp();
    PrintTimeDiff(start_time, end_time);
  
    freeaddrinfo(addr_result);
    (void) close(sock);
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
	syserr("getaddrinfo");
    
    my_address.sin_family = AF_INET; // IPv4
    my_address.sin_addr.s_addr =
	((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    my_address.sin_port = htons((uint16_t) port);
    
    freeaddrinfo(addr_result);
    
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
	syserr("socket");
    
    sflags = 0;
    rcva_len = (socklen_t) sizeof(my_address);
    
    /* Starting timer */
    uint64_t start_time = GetTimeStamp();
    uint64_t message = htobe64(start_time);
    
    /* Sending timestamp in 64BE form */
    snd_len = sendto(sock, &message, sizeof(uint64_t), sflags, (struct sockaddr *) &my_address, rcva_len);
    if (snd_len != (ssize_t) sizeof(uint64_t))
      syserr("partial / failed write");
    
    (void) memset(buffer, 0, sizeof(buffer));
    flags = 0;
    len = (size_t) sizeof(buffer) - 1;
    rcva_len = (socklen_t) sizeof(srvr_address);
    rcv_len = recvfrom(sock, buffer, len, flags,
        (struct sockaddr *) &srvr_address, &rcva_len);

    /* Stoping timer */
    uint64_t end_time = GetTimeStamp();
   
    if (rcv_len < 0)
	syserr("read");
    
    (void) printf("read from socket: %zd bytes\n", rcv_len);
    (void) fprintf(stderr, "read from socket: %zd bytes: %s\n", rcv_len, buffer);
    
    PrintTimeStamp("sendto", start_time); // outputs to stderr
    PrintTimeStamp("reply", end_time);    // outputs to stderr
    PrintTimeDiff(start_time, end_time);
    
    if (close(sock) == -1)
	syserr("close");
}