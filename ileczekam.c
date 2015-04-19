#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "err.h"

#define BUFFER_SIZE 1000

const char* usage_error = "Usage: $./ileczekam -<connection_type: u, t> <hostname> <port>";

void test_tcp_connection(char *hostname, const int port);
void test_udp_connection(char *hostname, const int port);

int main(int argc, char *argv[]) {
    
    if (argc != 4)
        fatal(usage_error);

}
