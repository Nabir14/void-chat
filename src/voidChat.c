#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "includes/void-server.c"
#include "includes/void-client.c"

#define DEFAULT_PORT 16384
#define VERSION 1.0
#define AUTHOR "Nabir14"

char alias[8];	
struct serverInfo server;
struct clientInfo client;

void appendClient(struct sockaddr_in *clientList, int *clientCount){
	if(*clientCount > 0){
		for(int i = 0; i < *clientCount; i++){
			// Check If Client Doesnt Exists
			// like: Server Recived Client Addr == Addr of Client at index (i)
			// and: Server Recived Client Port == Port of Client at index (i)
			if(!(server.cAddr.sin_addr.s_addr == clientList[i].sin_addr.s_addr && server.cAddr.sin_port == clientList[i].sin_port)){
				if(*clientCount < 16){
					clientList[*clientCount] = server.cAddr;
					(*clientCount)++;
				}
			}
		}
	}else{
		clientList[0] = server.cAddr;
		(*clientCount)++;
	}
}

void *serverRead(){
	char msg[256];
	struct sockaddr_in clients[16];
	int connectedClients = 0;
	while(true){
		// Clear Previous Buffer
		memset(msg, 0, sizeof(msg));

		// Read Messages From Client
		receiveStrServer(&server, msg, sizeof(msg));

		printf("[LOG]: %s\n", msg);
		appendClient(clients, &connectedClients);

		// Echo Back To All Connected Clients
		for(int i = 0; i < connectedClients; i++){
			sendStrServer(&server, msg, strlen(msg), &clients[i]);
		}
	}	
	return NULL;
}

void *clientRead(){
	char msg[256];
	while(true){
		// Clear Previous Buffer
		memset(msg, 0, sizeof(msg));
		
		// Read Messages From Server
		receiveStrClient(&client, msg, sizeof(msg));
		// Remove: Current Line (Input)
		// Print: The Message
		// Note: This actually moves the input feild down.
		printf("\r%s\n", msg);
		printf("[%s]: ", alias);

		// Force Update Terminal Screen
		fflush(stdout);
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

	// Define Function Vars
	char msg[256];
	bool connected = true;
	
	// Get Server Info
	printf("IP: ");
	scanf("%[^\n]", client.ip);
	getchar();

	// Get User Info
	printf("Alias: ");
	scanf("%[^\n]", alias);
	getchar();

	// Set Default Port
	client.port = DEFAULT_PORT;

	// Initialize Client
	clientInit(&client);	

	// Start Reading Messages From Server (Multi-Thread)
	pthread_t clientThread;
	pthread_create(&clientThread, NULL, clientRead, NULL);
	
	// Print Input Label ([alias]:)
	printf("[%s]: ", alias);

	// Handle Messages
	while(connected){
		// Clear Previous Buffer
		memset(msg, 0, sizeof(msg));

		// Scan Message
		scanf("%[^\n]", msg);
		getchar();

		if(!strcmp(msg, "/exit")){
			// Disconnect
			pthread_cancel(clientThread);
			connected = false;

			// Log
			printf("[LOG]: Disconnected From Server!\n");
		}else{
			// Define And Send Message
			char message[256];
			strcpy(message, alias); // user
			strcat(message, ": ");   // user:
			strcat(message, msg);   // user: msg
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
