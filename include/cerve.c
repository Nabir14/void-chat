#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define CERVE_VERSION 1.0
#define CERVE_AUTHOR "Nabir14"

struct serverInfo {
	int sDesc;
	struct sockaddr_in sAddr;
	int cSock;
	char ip[15];
	int port;
};

struct clientInfo {
	int sDesc;
	struct sockaddr_in sAddr;
	char ip[15];
	int port;
};

void cerveInfo(){
	printf("Cerve v%.1f by %s\n", CERVE_VERSION, CERVE_AUTHOR);
}

void serverInit(struct serverInfo *serverInfo){
	serverInfo->sDesc = socket(AF_INET, SOCK_STREAM, 0);
	serverInfo->sAddr;
	serverInfo->sAddr.sin_family = AF_INET;
	serverInfo->sAddr.sin_port = htons(serverInfo->port);
	serverInfo->sAddr.sin_addr.s_addr = inet_addr(serverInfo->ip);
	bind(serverInfo->sDesc, (struct sockaddr*)&serverInfo->sAddr, sizeof(serverInfo->sAddr));
	listen(serverInfo->sDesc, 1);
}

void serverAcceptConnection(struct serverInfo *serverInfo){
	struct sockaddr cAddr;
	int cSize = sizeof(cAddr);
	serverInfo->cSock = accept(serverInfo->sDesc, (struct sockaddr*)&cAddr, &cSize);
}

void sendStrServer(struct serverInfo *serverInfo, const char* str, int strLength){
	send(serverInfo->cSock, str, strLength, 0);
}

void receiveStrServer(struct serverInfo *serverInfo, char* str, int strSize){
	recv(serverInfo->cSock, str, strSize, 0);
}

void stopServer(struct serverInfo *serverInfo){
	close(serverInfo->cSock);
	close(serverInfo->sDesc);	
}

void clientInit(struct clientInfo *clientInfo){
	clientInfo->sDesc = socket(AF_INET, SOCK_STREAM, 0);
	clientInfo->sAddr.sin_family = AF_INET;
	clientInfo->sAddr.sin_port = htons(clientInfo->port);
	clientInfo->sAddr.sin_addr.s_addr = inet_addr(clientInfo->ip);
}

void clientConnectServer(struct clientInfo* clientInfo){
	connect(clientInfo->sDesc, (struct sockaddr*)&clientInfo->sAddr, sizeof(clientInfo->sAddr));
}

void receiveStrClient(struct clientInfo *clientInfo, char* str, int strSize){
	recv(clientInfo->sDesc, str, strSize, 0);
}

void sendStrClient(struct clientInfo *clientInfo, const char* str, int strLength){
	send(clientInfo->sDesc, str, strLength, 0);
}

void stopClient(struct clientInfo *clientInfo){
	close(clientInfo->sDesc);
}
