CC = gcc
CFLAGS  = -g -Wall -Werror
OBJ = Main.o performConnection.o config.o think.o

default: client

client:	$(OBJ)
	$(CC) $(CFLAGS) -o Client $(OBJ)

Main.o:	Main.c
	$(CC) $(CFLAGS) -c Main.c

performConnection.o:	performConnection.c
			$(CC) $(CFLAGS) -c performConnection.c

config.o:	config.c
		$(CC) $(CFLAGS) -c config.c

think.o:	think.c
		$(CC) $(CFLAGS) -c think.c

clean:
	$(RM) count *.o *~
