#ifndef VOIDCLIENT_H
#define VOIDCLIENT_H

void clientInit(struct clientInfo *clientInfo);
void receiveStrClient(struct clientInfo *clientInfo, char* str, int strSize);
void sendStrClient(struct clientInfo *clientInfo, const char* str, int strLength);

#endif
