all: clean compile
compile:
	gcc ./src/voidChat.c ./src/includes/void-server.c ./src/includes/void-client.c -o vc -pthread -Wall -Wextra -g
clean:
	rm -rf server client vc
