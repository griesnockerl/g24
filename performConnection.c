#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "gameDetails.h"

#define BUFFER	1024

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

    int recvtest = 0;

    //Server accept connection

	gameServerVersion = malloc(BUFFER);
	recvtest = recv(sock, gameServerVersion, BUFFER, 0);
	printf("S: %s", gameServerVersion);
	error(gameServerVersion[0], "Fehler im Prolog: Verbindungsaufbau fehlgeschlagen!");
	while(recvtest != BUFFER){
    
	    recvtest += recv(sock, gameServerVersion, BUFFER, 0);
		printf("%s", gameServerVersion);
    
	}


    //Send client version
    char *sendmsg = "VERSION 1.0\n";
    send(sock, sendmsg, strlen(sendmsg),0);
    printf("C: %s", sendmsg);

    //Server accept Client versioni
    recvtest = 0;
    readBuffer = malloc(BUFFER);
    recvtest = recv(sock, readBuffer, BUFFER, 0);
    printf("S: %s", readBuffer);

    error(readBuffer[0], "Fehler im Prolog: Client version wird nicht  akzeptiert!");
    while(recvtest != BUFFER) {

	recvtest += recv(sock, readBuffer, BUFFER, 0);
	printf("%s", readBuffer);
	
    }

    //Send game id
    char id[] = "ID ";
    char newLine[] = "\n";
    char *gameId = strcat(strcat(id, gameID), newLine);
    send(sock, gameId, strlen(gameId), 0);
    printf("C: %s", gameId);

    //Server accept game id and ack with Gamekind name and game name
    recvtest = 0;
    gameKindName = malloc(BUFFER);
    recvtest = recv(sock, gameKindName, BUFFER, 0);
    printf("S: %s", gameKindName);
    error(gameKindName[0], "Fehler im Prolog: Art des Spiels kann nicht festgelegt werden!");
    
    while(recvtest != BUFFER){
  
	recvtest += recv(sock, gameKindName, BUFFER, 0);
	printf("%s", gameKindName);
  
    }
    
    recvtest = 0;
    gameName = malloc(BUFFER);
    recvtest = recv(sock, gameName, BUFFER, 0);
    printf("S: %s", gameName);
    error(gameName[0], "Fehler im Prolog: Spielname kann nicht festgelegt werden!");
    while(recvtest != BUFFER){
  
	recvtest += recv(sock, gameName, BUFFER, 0);
	printf("%s", gameName);
  
    }
   
    

    //Send playernumber,at first it must be empty
    char *sendPlayerNr = "PLAYER\n";
    send(sock, sendPlayerNr, strlen(sendPlayerNr), 0);
    printf("C %s", sendPlayerNr);

    //Server send Player info: number, name, totalcount
    recvtest = 0;
    playerNrAndName = malloc(BUFFER);
    recvtest = recv(sock, playerNrAndName, BUFFER, 0);
    printf("S: %s", playerNrAndName);
    error(playerNrAndName[0], "Fehler im Prolog: Spielernummer und Spielername kann nicht festgelegt werden!");
    while(recvtest != BUFFER){
  
	recvtest += recv(sock, playerNrAndName, BUFFER, 0);
	printf("%s", playerNrAndName);
  
    }

    recvtest = 0;
    playerTotalCount = malloc(BUFFER);
    recvtest = recv(sock, playerTotalCount, BUFFER, 0);
    printf("S: %s", playerTotalCount);
    error(playerTotalCount[0], "Fehler im Prolog: Anzahl der Spieler kann nicht festgelegt werden!");
    while(recvtest != BUFFER){
  
	recvtest += recv(sock, playerTotalCount, BUFFER, 0);
	printf("%s", playerTotalCount);
  
    }

	//------SHM------//
	char *strdel = strtok(playerNrAndName, "+ YOU");
	char *pnumber = malloc(sizeof(char));
	char *pname = malloc(sizeof(char));

	strcpy(pnumber, strdel);
	strdel = strtok(NULL, "");
	strcpy(pname, strdel);

	//init vars in gameDetails.h
	strcpy(playerName, pname);
	playerNumber = atoi(pnumber);
	playerCount = atoi(playerTotalCount);
}
