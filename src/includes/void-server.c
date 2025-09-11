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

void serverInit(struct serverInfo *serverInfo){
	serverInfo->sDesc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	serverInfo->sAddr;
	serverInfo->sAddr.sin_family = AF_INET;
	serverInfo->sAddr.sin_port = htons(serverInfo->port);
	serverInfo->sAddr.sin_addr.s_addr = inet_addr(serverInfo->ip);
	bind(serverInfo->sDesc, (struct sockaddr*)&serverInfo->sAddr, sizeof(serverInfo->sAddr));
}

void sendStrServer(struct serverInfo *serverInfo, const char* str, int strLength, struct sockaddr_in *clientAddr){
	int clientStructLen = sizeof(*clientAddr);
	sendto(serverInfo->sDesc, str, strLength, 0, (struct sockaddr*)clientAddr, clientStructLen);
}

void receiveStrServer(struct serverInfo *serverInfo, char* str, int strSize){
	int clientStructLen = sizeof(serverInfo->cAddr);
	recvfrom(serverInfo->sDesc, str, strSize, 0, (struct sockaddr*)&serverInfo->cAddr, &clientStructLen);
}

void stopServer(struct serverInfo *serverInfo){
	close(serverInfo->sDesc);	
}
