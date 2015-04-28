#pragma once

typedef enum ServerMode {
	SYSTEMD,
	MANUAL
} ServerMode;

int server_mode(ServerMode);
