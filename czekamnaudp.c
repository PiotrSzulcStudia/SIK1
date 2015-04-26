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
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	uint64_t curtime;

	char buffer[BUFFER_SIZE];
	socklen_t snda_len, rcva_len;
	ssize_t len, snd_len;

	sock = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP socket
	if (sock < 0)
		syserr("socket");

	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
	server_address.sin_port = htons(PORT_NUM); // default port for receiving is PORT_NUM

	// bind the socket to a concrete address
	if (bind(sock, (struct sockaddr *) &server_address,
			(socklen_t) sizeof(server_address)) < 0)
		syserr("bind");

	snda_len = (socklen_t) sizeof(client_address);
	for (;;) {
		do {
		        
			rcva_len = (socklen_t) sizeof(client_address);
			flags = 0; // we do not request anything special
			len = recvfrom(sock, &curtime, sizeof(curtime), flags,
					(struct sockaddr *) &client_address, &rcva_len);
			
			int64_t current_time = GetTimeStamp();
			
			if (len < 0)
				syserr("error on datagram from client socket");
			else {
				(void) printf("%" PRId64 "\n", be64toh(curtime));
				(void) printf("read from socket: %zd bytes: %.*s\n", len,
						(int) len, buffer);
				
				sflags = 0;
				snd_len = sendto(sock, buffer, (size_t) len, sflags,
						(struct sockaddr *) &client_address, snda_len);
				if (snd_len != len)
					syserr("error on sending datagram to client socket");
			}
		} while (len > 0);
		(void) printf("finished exchange\n");
	}

	return 0;
}
