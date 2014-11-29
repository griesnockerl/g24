#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "performConnection.c"

int setupConnection(char *gameID, char *hostname, uint16_t portnumber, char *gamekindname)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Fehler beim erzeugen des Sockets!");
        return EXIT_FAILURE;
    }

    performConnection(sock, gameID, hostname, portnumber, gamekindname);

   return EXIT_SUCCESS;
}
