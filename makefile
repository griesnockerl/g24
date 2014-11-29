CC = gcc
CFLAGS  = -g -Wall -Werror

default: client

client: Main.o
	$(CC) $(CFLAGS) -o Client Main.o

clean:
	$(RM) count *.o *~
