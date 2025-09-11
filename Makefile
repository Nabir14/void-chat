all: clean compile
compile:
	gcc ./src/voidChat.c -o vc
clean:
	rm -rf server client vc
