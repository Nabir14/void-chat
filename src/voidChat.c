#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "includes/void-server.c"
#include "includes/void-client.c"

#define DEFAULT_PORT 16384
#define VERSION 1.0
#define AUTHOR "Nabir14"

struct serverReadArgs {
	int connectedClients;
	struct sockaddr_in clients[];
};

struct clientReadArgs {
	bool isExiting;
};

int serverMaxClients = 2;
char alias[8];	
struct serverInfo server;
struct clientInfo client;

void appendClient(struct sockaddr_in *clientList, int *clientCount){
	if(*clientCount > 0){
		bool isInList = false;
		for(int i = 0; i < *clientCount; i++){
			// Check If Client Exists
			// like: Server Recived Client Addr == Addr of Client at index (i)
			// and: Server Recived Client Port == Port of Client at index (i)
			if(server.cAddr.sin_addr.s_addr == clientList[i].sin_addr.s_addr && server.cAddr.sin_port == clientList[i].sin_port){
				isInList = true;
				break;
			}
		}
		// Append If Only Not Found
		if(!isInList && *clientCount < serverMaxClients){
			clientList[*clientCount] = server.cAddr;
			(*clientCount)++;
		}

	}else{
		clientList[0] = server.cAddr;
		(*clientCount)++;
	}
}

void *serverRead(void* arguments){
	char msg[256];
	struct serverReadArgs *args = (struct serverReadArgs*)arguments;
	while(true){
		// Clear Previous Buffer
		memset(msg, 0, sizeof(msg));

		// Read Messages From Client
		receiveStrServer(&server, msg, sizeof(msg));

		printf("[LOG]: %s\n", msg);
		appendClient(args->clients, &args->connectedClients);

		// Echo Back To All Connected Clients
		for(int i = 0; i < args->connectedClients; i++){
			sendStrServer(&server, msg, strlen(msg), &args->clients[i]);
		}
	}	
	return NULL;
}

void *clientRead(void* arguments){
	char msg[256];
	struct clientReadArgs *args = (struct clientReadArgs*)arguments;
	while(true){
		// Clear Previous Buffer
		memset(msg, 0, sizeof(msg));
		
		// Read Messages From Server
		receiveStrClient(&client, msg, sizeof(msg));
		// Remove: Current Line (Input)
		// Print: The Message
		// Note: This actually moves the input feild down.
		printf("\r%s\n", msg);
		if(args->isExiting == false){
			printf("[%s]: ", alias);
		}

		// Force Update Terminal Screen
		fflush(stdout);
	}
	return NULL;
}

void runServer(){
	char isGlobalChoice = 'n';
	struct serverReadArgs readInfo;
	readInfo.clients[serverMaxClients];
	readInfo.connectedClients = 0;

	// Get Info
	printf("Host Server Globally? [y/N]: ");
	scanf("%c", &isGlobalChoice);
	getchar();
	printf("Max Clients: ");
	scanf("%d", &serverMaxClients);
	getchar();

	// Initialize Server
	if(tolower(isGlobalChoice) == 'y'){
		strcpy(server.ip, "0.0.0.0");
	}else{
		strcpy(server.ip, "127.0.0.1");
	}
	server.port = DEFAULT_PORT;
	serverInit(&server);

	// Host Server (Multi-Thread)
	pthread_t serverThread;
	pthread_create(&serverThread, NULL, serverRead, (void*)&readInfo);

	// Log
	printf("[LOG]: Server Is Running\n[Press Enter To Close Server]\n");

	// Get Input
	getchar();

	// Echo Exit Message Back To All Connected Clients
	char serverExitMsg[32] = "[SERVER]: Server Has Stopped!\n";
	for(int i = 0; i < readInfo.connectedClients; i++){
		sendStrServer(&server, serverExitMsg, strlen(serverExitMsg), &readInfo.clients[i]);
	}

	// Close On Enter
	pthread_cancel(serverThread);
	stopServer(&server);
}

void runClient(){
	memset(alias, 0, sizeof(alias));

	// Define Function Vars
	char msg[256];
	bool connected = true;
	struct clientReadArgs readInfo;
	readInfo.isExiting = false;
	
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
	pthread_create(&clientThread, NULL, clientRead, (void*)&readInfo);

	// Define Join Message	
	char serverConnectMsg[43];
	strcpy(serverConnectMsg, "[SERVER]: ");
	strcat(serverConnectMsg, alias);
	strcat(serverConnectMsg, " has joined the server.");

	// Establish A Connection With Server Using First Message
	sendStrClient(&client, serverConnectMsg, strlen(serverConnectMsg));

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
			readInfo.isExiting = true;

			// Define Server Disconnect Message
			char serverDisconnectMsg[42];
			strcpy(serverDisconnectMsg, "[SERVER]: ");
			strcat(serverDisconnectMsg, alias);
			strcat(serverDisconnectMsg, " has left the server.");
			// Send Disconnect Message
			sendStrClient(&client, serverDisconnectMsg, strlen(serverDisconnectMsg));

			// Disconnect
			pthread_cancel(clientThread);
			connected = false;
		}else{
			// Define Message For Send
			char message[256];
			strcpy(message, alias); // user
			strcat(message, ": ");   // user:
			strcat(message, msg);   // user: msg

			// Define Temp Message For Clear
			char tempMessage[256];
			strcpy(tempMessage, "["); // [
			strcat(tempMessage, alias); // [user
			strcat(tempMessage, "]: ");   // [user]:
			strcat(tempMessage, msg);   // [user]: msg

			// Remove Input Line
			printf("\r");
			printf("\x1b[1A");
			for(int i = 0; i < strlen(tempMessage); i++){
				printf(" ");
			}
			fflush(stdout);


			// Send Message If There Is Any
			if(strcmp(message, "")){
				sendStrClient(&client, message, strlen(message));
			}
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
