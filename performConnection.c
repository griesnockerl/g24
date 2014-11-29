#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER          1024

void error(char errorCode, char *msg)
{
    if(errorCode == '-')
    {
        perror(msg);
        exit(0);
    }
}

void performConnection(int sock, char *gameID, char *HOSTNAME, uint16_t PORTNUMBER, char *GAMEKINDNAME)
{
    struct sockaddr_in server;
    struct hostent *host = gethostbyname(HOSTNAME);

    if (sock < 0)
    {
        perror("Client: Socket fehler!\n");
        exit(0);
    }

    if(host == NULL)
    {
        perror("Server nicht gefunden!\n");
        exit(0);
    }
   
    memset(&server, 0, sizeof(server));
    memcpy( &(server.sin_addr.s_addr), host->h_addr, host->h_length);
    server.sin_family = host->h_addrtype;
    server.sin_port = htons(PORTNUMBER);

    if(connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0)
    {
        perror("Verbindungsaufbau zum Game-Server fehlgeschlagen!");
        exit(0);
    }

    /*---------------PROLOG-------------- */
    char *readBuffer, *gameServerVersion, *gameKindName, *gameName, *playerNrAndName, *playerTotalCount;

    //Server accept connection
    gameServerVersion = malloc(BUFFER);
    recv(sock, gameServerVersion, BUFFER, 0);
    printf("S: %s", gameServerVersion);
    
    error(gameServerVersion[0], "Fehler im Prolog: Verbindungsaufbau fehlgeschlagen!");

    //Send client version
    char *sendmsg = "VERSION 1.0\n";
    send(sock, sendmsg, strlen(sendmsg),0);
    printf("C: %s", sendmsg);

    //Server accept Client versioni
    readBuffer = malloc(BUFFER);
    recv(sock, readBuffer, BUFFER, 0);
    printf("S: %s", readBuffer);

    error(readBuffer[0], "Fehler im Prolog: Client version wird nicht  akzeptiert!");

    //Send game id
    char id[] = "ID ";
    char newLine[] = "\n";
    char *gameId = strcat(strcat(id, gameID), newLine);
    send(sock, gameId, strlen(gameId), 0);
    printf("C: %s", gameId);

    //Server accept game id and ack with Gamekind name and game name
    gameKindName = malloc(BUFFER);
    recv(sock, gameKindName, BUFFER, 0);
    printf("S: %s", gameKindName);
    gameName = malloc(BUFFER);
    recv(sock, gameName, BUFFER, 0);
    printf("S: %s", gameName);

    error(gameKindName[0], "Fehler im Prolog: Art des Spiels kann nicht festgelegt werden!");
    error(gameName[0], "Fehler im Prolog: Spielname kann nicht festgelegt werden!");

    //Send playernumber,at first it must be empty
    char *sendPlayerNr = "PLAYER\n";
    send(sock, sendPlayerNr, strlen(sendPlayerNr), 0);
    printf("C %s", sendPlayerNr);

    //Server send Player info: number, name, totalcount
    playerNrAndName = malloc(BUFFER);
    recv(sock, playerNrAndName, BUFFER, 0);
    printf("S: %s", playerNrAndName);

    error(playerNrAndName[0], "Fehler im Prolog: Spielernummer und Spielername kann nicht festgelegt werden!");

    playerTotalCount = malloc(BUFFER);
    recv(sock, playerTotalCount, BUFFER, 0);
    printf("S: %s", playerTotalCount);

    error(playerTotalCount[0], "Fehler im Prolog: Anzahl der Spieler kann nicht festgelegt werden!");
}
