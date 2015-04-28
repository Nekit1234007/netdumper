AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = netdumper

netdumper_SOURCES = \
	src/main.c \
	src/server.c \
	src/client.c \
	src/utils.c

netdumper_CFLAGS = $(LIBSYSTEMD_CFLAGS)
netdumper_LDFLAGS = $(LIBSYSTEMD_LIBS)
