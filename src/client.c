#include "utils.h"
#include "client.h"
#include "protocol.h"

int client_mode(char *hostname, char *path, ServerAction action) {
	_cleanup_close_ int sock = -1;
	ssize_t wrote_bytes = 0;
	InitPacket init_packet;
	struct addrinfo *results = NULL;
	struct addrinfo hints, *p = NULL;

	int r = 0;

	assert(hostname);
	assert(path);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((r = getaddrinfo(hostname, "1234", &hints, &results)) < 0) {
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
	    return r;
	}

	for(p = results; p != NULL; p = p->ai_next) {
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	    if (sock < 0) {
	        perror("socket");
	        continue;
	    }

	    if (connect(sock, p->ai_addr, p->ai_addrlen) < 0) {
	        close(sock);
	        perror("connect");
	        continue;
	    }

	    break;
	}

	freeaddrinfo(results);

	if (p == NULL) {
		fprintf(stderr, "Failed to connect");
		return -EINVAL;
	}

	memset(&init_packet, 0, sizeof(init_packet));
	init_packet.action = action;
	strcpy(init_packet.path, path);

	wrote_bytes = write(sock, &init_packet, sizeof(init_packet));
	if (wrote_bytes <= 0) {
		perror("write init packet");
		return -errno;
	}

	switch (action) {
	case READ:
		return redirect(sock, STDOUT_FILENO);

	case WRITE:
		return redirect(STDIN_FILENO, sock);
	}

	return -EINVAL;
}
