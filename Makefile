all: clean compile
compile:
	gcc ./src/voidChat.c -o vc -Wall -Wextra -g
clean:
	rm -rf server client vc
