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

void clientInit(struct clientInfo *clientInfo){
	clientInfo->sDesc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	clientInfo->cAddr.sin_family = AF_INET;
	clientInfo->cAddr.sin_port = htons(clientInfo->port);
	clientInfo->cAddr.sin_addr.s_addr = inet_addr(clientInfo->ip);
}

void receiveStrClient(struct clientInfo *clientInfo, char* str, int strSize){
	socklen_t serverStructLen = sizeof(clientInfo->cAddr);
	recvfrom(clientInfo->sDesc, str, strSize, 0, (struct sockaddr*)&clientInfo->cAddr, &serverStructLen);
}

void sendStrClient(struct clientInfo *clientInfo, const char* str, int strLength){
	int serverStructLen = sizeof(clientInfo->cAddr);
	sendto(clientInfo->sDesc, str, strLength, 0, (struct sockaddr*)&clientInfo->cAddr, serverStructLen);
}
