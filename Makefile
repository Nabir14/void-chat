all: clean compile
compile:
	gcc voidChat.c -o vc
compile-server:
	gcc vc-server.c -o server
compile-client:
	gcc vc-client.c -o client
clean:
	rm -rf server client vc
