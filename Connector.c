#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "performConnection.c"

int main(int argc, const char *argv[])
{
    char *gameID = malloc(11*sizeof(char));

    if(argc < 2 || strlen(argv[1]) != 11)
    {
        perror("Bitte gib eine 11 stellige Game id ein!");
        return EXIT_FAILURE;
    }

    //Copy everything from argv[1] -> gameID
    strcpy(gameID,argv[1]); 

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Fehler beim erzeugen des Sockets!");
        return EXIT_FAILURE;
    }

    performConnection(sock, gameID);

   return EXIT_SUCCESS;
}
