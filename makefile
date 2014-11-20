CC = gcc
CFLAGS  = -g -Wall -Werror

default: client

client: Connector.o
	$(CC) $(CFLAGS) -o Client Connector.o

client: performConnection.o
	$(CC) $(CFLAGS) -o performConnection.o

clean:
	$(RM) count *.o *~
