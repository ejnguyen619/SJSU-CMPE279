# Makefile for Assignment 1 and 2

# Overview
# Compile client and server programs

all: client.c server.c
	gcc client.c -o client
	gcc server.c -o server

client: client.c
	gcc client.c -o client
	
server: server.c
	gcc server.c -o server

client_target:
	./client
	
server_target:
	./server

clean: server client
	rm server
	rm client