#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <systemd/sd-daemon.h>

#define CHUNK_SIZE (1 << 14)

static inline void freep(void *p) {
	free(*(void**) p);
}

static inline void closep(int *fd) {
	close(*fd);
}

static inline void fclosep(FILE **fd) {
	if (*fd) {
		fclose(*fd);
	}
}

#define _cleanup_(c) __attribute__((cleanup(c)))
#define _cleanup_close_ _cleanup_(closep)
#define _cleanup_free_ _cleanup_(freep)
#define _cleanup_fclose_ _cleanup_(fclosep)

#define _malloc_ __attribute__((malloc))

int write_to_file(int sock, char *path);
int redirect_from_file(char *path, int to);
int redirect_to_file(int from, char *path);
int redirect(int from, int to);
