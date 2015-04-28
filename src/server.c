#include "utils.h"
#include "server.h"
#include "protocol.h"

static int setup_socket() {
	int listener = -1;
	struct sockaddr_in6 sa;
	int r = 0;

	listener = socket(PF_INET6, SOCK_STREAM|SOCK_CLOEXEC, IPPROTO_TCP);
	if (listener < 0) {
		perror("socket");
		return -errno;
	}

	sa.sin6_port = htons(1234);
	sa.sin6_family = AF_INET6;
	sa.sin6_addr = in6addr_any;
	r = bind(listener, (struct sockaddr *)&sa, sizeof(sa));
	if (r < 0) {
		perror("bind");
		return -errno;
	}

	r = listen(listener, 0);
	if (r < 0) {
		perror("listen");
		return -errno;
	}
	return listener;
}

static int get_socket(ServerMode mode) {
	if (mode == SYSTEMD) {
		return SD_LISTEN_FDS_START;
	} else {
		return setup_socket();
	}
}

static int get_init_packet(int sock, InitPacket *packet) {
	ssize_t read_bytes = 0;

	read_bytes = read(sock, packet, sizeof(InitPacket));
	if (read_bytes < 0) {
		return -errno;
	} if (read_bytes == 0) {
		return -EINVAL;
	}

	return 0;
}

int server_mode(ServerMode mode) {
	_cleanup_close_ int sock = -1;

	InitPacket init_packet;

	sock = get_socket(mode);
	if (sock < 0) {
		perror("get_socket");
		return -errno;
	}

	sd_notify(0, "READY=1");

	sock = accept4(sock, NULL, NULL, SOCK_CLOEXEC);
	if (sock < 0) {
		perror("accept");
		return -errno;
	}

	memset(&init_packet, 0, sizeof(init_packet));
	if (get_init_packet(sock, &init_packet) < 0) {
		perror("get_init_packet");
		return -errno;
	}

	switch (init_packet.action) {
	case WRITE:
		printf("Writing to file: %s", init_packet.path);

		return redirect_to_file(sock, init_packet.path);

	case READ:
		printf("Reading from file: %s", init_packet.path);

		return redirect_from_file(init_packet.path, sock);

	default:
		fprintf(stderr, SD_ERR "Garbage in init_packet.action");
		break;
	}

	return 0;
}
