CC=g++
CFLAGS=-c -Wall

all: daytime_client

daytime_client: daytime_client.o
	$(CC) daytime_client.o -o daytime_client

daytime_client.o: daytime_client.cpp
	$(CC) $(CFLAGS) daytime_client.cpp

clean:
	rm -f *.o daytime_client
