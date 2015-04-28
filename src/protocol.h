#pragma once

typedef enum ServerAction {
	READ,
	WRITE
} ServerAction;

typedef struct InitPacket {
	ServerAction action;
	char path[512];
} InitPacket;
