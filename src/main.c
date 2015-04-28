#define _GNU_SOURCE

#include "utils.h"
#include "server.h"
#include "client.h"
#include "protocol.h"

static bool use_systemd = false;

static void stopping() {
	static bool sent = false;
	if (!sent) {
		sd_notify(0, "STOPPING=1");
		sent = true;
	}
}

int main(int argc, char **argv) {
	int socks = -1;
	int r = 0;

	socks = sd_listen_fds(1);

	/*if (strcmp(argv[1], "srv") == 0) {*/
	if (socks == 1) {
		use_systemd = true;
		if (server_mode(SYSTEMD) < 0) {
			perror("server_mode");
			r = -1;
		}
		goto exit;
	} else if (socks <= 0 && argc == 1) {
		fprintf(stderr, "Failed to retrieve number of sockets. Falling back to manual socket creation\n");
		if (server_mode(MANUAL) < 0) {
			perror("server_mode");
			r = -1;
		}
		goto exit;
	} else if (socks > 1) {
		fprintf(stderr, "Too many sockets passed.\n");
		r = -1;
		goto exit;
	}

	if (socks == 0) {
		assert(argv[1]);
		assert(argv[2]);
		assert(argv[3]);
		if (client_mode(argv[1], argv[2], !strcmp(argv[3], "send") ? WRITE : READ) < 0) {
			r = -1;
			goto exit;
		}
	}

exit:
	stopping();
	return (r < 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
