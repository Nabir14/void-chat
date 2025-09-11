#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "includes/void-server.c"
#include "includes/void-client.c"

#define DEFAULT_PORT 16384
#define VERSION 1.0
#define AUTHOR "Nabir14"

struct serverInfo server;
struct clientInfo client;

void *serverRead(){
	char msg[256];
	while(true){
		receiveStrServer(&server, msg, sizeof(msg));
		printf("[LOG]: %s\n", msg);
		sendStrServer(&server, msg, strlen(msg));
	}	
	return NULL;
}

void *clientRead(){
	char msg[256];
	while(true){
		receiveStrClient(&client, msg, sizeof(msg));
		printf("\n%s\n", msg);
	}
	return NULL;
}

void runServer(){	
	// Initialize Server
	strcpy(server.ip, "127.0.0.1");
	server.port = DEFAULT_PORT;
	serverInit(&server);

	// Host Server (Multi-Thread)
	pthread_t serverThread;
	pthread_create(&serverThread, NULL, serverRead, NULL);

	// Log
	printf("[LOG]: Server Is Running\n[Press Enter To Close Server]\n");

	// Get Input
	getchar();

	// Close On Enter
	pthread_cancel(serverThread);
	stopServer(&server);
}

void runClient(){
	char alias[8];	
	char msg[256];
	bool connected = true;
	
	// Get Info
	printf("IP: ");
	scanf("%[^\n]", client.ip);
	getchar();

	printf("Alias: ");
	scanf("%[^\n]", alias);
	getchar();

	client.port = DEFAULT_PORT;

	// Initialize Client
	clientInit(&client);	

	pthread_t clientThread;
	pthread_create(&clientThread, NULL, clientRead, NULL);
	while(connected){
		// Scan Message
		printf("$[%s]: ", alias);
		scanf("%[^\n]", msg);
		getchar();
		if(!strcmp(msg, "/exit")){
			// Disconnect
			pthread_cancel(clientThread);
			connected = false;

			// Log
			printf("[LOG]: Disconnected From Server!\n");
		}else{
			char message[256];
			strcpy(message, alias); // user
			strcat(message, ":");   // user:
			strcat(message, msg);   // user: msg
			strcat(message, "\0");  // user: msg\0
			sendStrClient(&client, message, strlen(message));
		}
	}
}

int main(){
	printf("▒█░░▒█ █▀▀█ ░▀░ █▀▀▄ ▒█▀▀█ █░░█ █▀▀█ ▀▀█▀▀ \n");
	printf("░▒█▒█░ █░░█ ▀█▀ █░░█ ▒█░░░ █▀▀█ █▄▄█ ░░█░░ \n");
	printf("░░▀▄▀░ ▀▀▀▀ ▀▀▀ ▀▀▀░ ▒█▄▄█ ▀░░▀ ▀░░▀ ░░▀░░\n");

	char cmd[8];

	bool run = true;

	while(run){
		printf("> ");
		scanf("%[^\n]", cmd);
		getchar();

		if(!strcmp(cmd, "/quit")){
			printf("QUIT!\n");
			run = false;
		}else if(!strcmp(cmd, "/host")){
			runServer();
		}else if(!strcmp(cmd, "/connect")){
			runClient();
		}else if(!strcmp(cmd, "/info")){
			printf("+--------Info--------+\n");
			printf("Version: %.1f\n", VERSION);
			printf("Author: %s\n", AUTHOR);
		}else if(!strcmp(cmd, "/help")){
			printf("+--------Help--------+\n");
			printf("[/host]: Host A Server.\n");
			printf("[/connect]: Connect To Server.\n");
			printf("[/exit]: Exit From Connected Server.\n");
			printf("[/quit]: Quit VoidChat.\n");	
		}else{
			printf("[ERROR]: Invalid Command!\n");
		}
	}
	return 0;
}
