#ifndef VOIDSERVER_H
#define VOIDSERVER_H

void serverInit(struct serverInfo *serverInfo);
void sendStrServer(struct serverInfo *serverInfo, const char* str, int strLength, struct sockaddr_in *clientAddr);
void receiveStrServer(struct serverInfo *serverInfo, char* str, int strSize);
void stopServer(struct serverInfo *serverInfo);

#endif
