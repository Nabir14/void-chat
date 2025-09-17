#ifndef VOIDSERVER_H
#define VOIDSERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct serverInfo {
	int sDesc;
	struct sockaddr_in sAddr;
	struct sockaddr_in cAddr;
	char ip[15];
	int port;
};

void serverInit(struct serverInfo *serverInfo);
void sendStrServer(struct serverInfo *serverInfo, const char* str, int strLength, struct sockaddr_in *clientAddr);
void receiveStrServer(struct serverInfo *serverInfo, char* str, int strSize);
void stopServer(struct serverInfo *serverInfo);

#endif
