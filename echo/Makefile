CC=g++
CFLAGS=-c -Wall -std=c++11

all: echo_server

echo_server: main.o echo_server.o
	$(CC) main.o echo_server.o -o echo_server

main.o: main.cpp echo_server.h
	$(CC) $(CFLAGS) main.cpp

echo_server.o: echo_server.cpp echo_server.h
	$(CC) $(CFLAGS) echo_server.cpp

clean:
	rm -f *.o echo_server
