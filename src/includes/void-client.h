#ifndef VOIDCLIENT_H
#define VOIDCLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct clientInfo {
	int sDesc;
	struct sockaddr_in cAddr;
	char ip[15];
	int port;
};

void clientInit(struct clientInfo *clientInfo);
void receiveStrClient(struct clientInfo *clientInfo, char* str, int strSize);
void sendStrClient(struct clientInfo *clientInfo, const char* str, int strLength);

#endif
