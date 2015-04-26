/* Piotr Szulc ps347277 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

#include "err.h"
#include "timer.h"

#define BUFFER_SIZE   1000

static const char* usage_error = "Usage: $./czekamnaudp <port>";

int main(int argc, char *argv[]) {
	int sock;
	int flags, sflags;
	unsigned short int port;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	char buffer[BUFFER_SIZE];
	socklen_t snda_len, rcva_len;
	ssize_t len, snd_len;
	uint64_t message;
	
	if (argc != 2)
		fatal(usage_error);
	
	int port = atoi(argv[1]);
	if ( (PORT_LOW_BOUND <= port ) || (port >= PORT_UP_BOUND))
		fatal("invalid port number");
	
	sock = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP socket
	if (sock < 0)
		syserr("socket");

	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
	server_address.sin_port = htons(port); // default port for receiving is PORT_NUM

	// bind the socket to a concrete address
	if (bind(sock, (struct sockaddr *) &server_address,
			(socklen_t) sizeof(server_address)) < 0)
		syserr("bind");

	snda_len = (socklen_t) sizeof(client_address);
	for (;;) {
		do {
		        
			rcva_len = (socklen_t) sizeof(client_address);
			flags = 0; // we do not request anything special
			len = recvfrom(sock, &message, sizeof(message), flags,
					(struct sockaddr *) &client_address, &rcva_len);
			int64_t current_time = GetTimeStamp();
			int64_t received_time = be64toh(message);
			
			PrintTimeStamp("message arival", current_time);
			
			if (len < 0)
				syserr("error on datagram from client socket");
			else {
				(void) printf("read from socket: %zd bytes: %" PRId64 "\n", len, received_time);
				sprintf(buffer, "%" PRId64 " %" PRId64"\0", htobe64(received_time), htobe64(current_time)); // no buffer overflow
				(void) fprintf(stderr, "sending to client: %s\n", buffer);
				sflags = 0;
				snd_len = sendto(sock, buffer, (size_t) strlen(buffer), sflags,
						(struct sockaddr *) &client_address, snda_len);
				if (snd_len != strlen(buffer))
					syserr("error on sending datagram to client socket");
			}
		} while (len > 0);
		(void) printf("finished exchange\n");
	}

	return 0;
}