CC=g++
CFLAGS=-c -Wall

all: echo_client

echo_client: echo_client.o
	$(CC) echo_client.o -o echo_client

echo_client.o: echo_client.cpp
	$(CC) $(CFLAGS) echo_client.cpp

clean:
	rm -f *.o echo_client
