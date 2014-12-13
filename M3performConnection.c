#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "gameDetails.h"

#define BUFFER	2048

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
    char *readBuffer, *gameServerVersion, *gameKindName, *gameName, *playerNrAndName, *playerTotalCount, char *strGarbage1;

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
	error(gameKindName[0], "Fehler im Prolog: Art des Spiels 	kann nicht festgelegt werden!");
	strGarbage1 = malloc(BUFFER);
	strGarbage1 = strtok(gameKindName, " ");
	strGarbage1 = strtok(NULL, " ");
	gameKindName = strtok(NULL, " ");
	if(!(gameKindName == "NMMorris")) {
		printf("Fehler: Client kann nur Mühle spielen! Erwarte NMMorris als Spiel!");
		exit(0); /* Anmerkung zu allen exits in perform Connection : normalerweise wird fuer fehlerhafte termination exit(1) verwendet, exit(0) fuer fehlerfreie termination. */
	}

    gameName = malloc(BUFFER);
    recv(sock, gameName, BUFFER, 0);
    printf("S: %s", gameName);

   
    error(gameName[0], "Fehler im Prolog: Spielname kann nicht festgelegt werden!");

    //Send playernumber,at first it must be empty
    char *sendPlayerNr = "PLAYER\n";
    send(sock, sendPlayerNr, strlen(sendPlayerNr), 0);
    printf("C: %s", sendPlayerNr);

    //Server send Player info: number, name, totalcount
    playerNrAndName = malloc(BUFFER);
    recv(sock, playerNrAndName, BUFFER, 0);
    printf("S: %s", playerNrAndName);

    error(playerNrAndName[0], "Fehler im Prolog: Spielernummer und Spielername kann nicht festgelegt werden!");

	char *strdel = strtok(playerNrAndName, " ");
	char *pnumber = malloc(sizeof(char));
	char *pname = malloc(sizeof(char)); /* ein char nur? ist das nicht zu wenig? */
	strdel = strtok(NULL, " ");
	strdel = strtok(NULL, " ");

	/* init gamedetails */

	strcpy(pnumber, strdel);
	strdel = strtok(NULL, " ");
	strcpy(pname, strdel);
	strcpy(playerName, pname);
	playerNumber = atoi(pnumber);

    playerTotalCount = malloc(BUFFER);
    recv(sock, playerTotalCount, BUFFER, 0);
    printf("S: %s", playerTotalCount);

    error(playerTotalCount[0], "Fehler im Prolog: Anzahl der Spieler kann nicht festgelegt werden!");

	recv(sock, readBuffer, BUFFER, 0);
	printf("S: %s", readBuffer);
	error(readBuffer[0], "Fehler im Prolog: Prologphase kann nicht korrekt abgeschlossen werden!");

	/*---------------SPIELVERLAUF-------------- */
	
	int testLoop = 1;

 /* solange testloop nicht auf 0 gesetzt wird, läuft Schleife durch um wieder neuen Befehl aufzunehmen. es wird 0 gesetzt wenn GAMEOVER phase abschließt, damit terminiert auch die Schleife */
	
	while(testLoop) {	

		char *readComm = malloc(BUFFER);
		recv(sock, readComm, BUFFER, 0);

		switch(readComm[2]) {

		case 'M': /* MOVE */
				char *maxTimeforMove, *piecesToHit, *playersCountpiecesCount,	*playerNamepieceNrAndPos;
		
				maxTimeforMove = malloc(BUFFER);
				strcpy(maxTimeforMove, readComm);
				printf("S: %s", maxTimeforMove);
				error(maxTimeforMove[0], "Fehler im Spielverlauf: Maximale Zugzeit kann nicht festgelegt werden!");
				maxTimeMove = atoi(maxTimeforMove);

				piecesToHit = malloc(BUFFER);
				recv(sock, piecesToHit, BUFFER, 0);
				printf("S: %s", piecesToHit);
				error(piecesToHit[0], "Fehler im Spielverlauf: Anzahl zu schlagender Steine kann nicht festgelegt werden!");
				remainToHit = atoi(piecesToHit);
				
				playersCountpiecesCount = malloc(BUFFER);
				recv(sock, playersCountpiecesCount, BUFFER, 0);
				printf("S: %s", playersCountpiecesCount);
				error(playersCountpiecesCount[0], "Fehler im Spielverlauf: Anzahl der Spieler und Anzahl der Steine kann nicht festgelegt 					werden!");
				
				char *playCount = malloc(sizeof(char));
				char *piecCount = malloc(sizeof(char));
				char *strgarbage2 = malloc(BUFFER);

				strgarbage2 = strtok(playersCountpiecesCount, " ,");
				strgarbage2 = strtok(NULL, " ,");
				playCount = strtok(NULL, " ,");
				piecCount = strtok(NULL, " ,");
				
				/* Vars aus Gamedetails werden geschrieben */
				playerCount = atoi(playCount);
				piecesCount = atoi(piecCount);

				/* weiß nicht wie folgender bereich am besten implementeirt werden soll, muss mir das noch anschauen */
				/* ----------------- */
				char **piecelist;
				piecelist = malloc(playerCount * sizeof(int *));
				char *help = malloc(BUFFER);
			/*	char *playNr = malloc(sizeof(char));
				char *stoneNr = malloc(sizeof(char));
				char *stonePos = malloc(2 * sizeof(char)); 				*/

				for (int m = 0; m < playerCount; m++) {

					piecelist[m] = malloc(piecesCount * sizeof(?));

				for (int k = 0; k < piecesCount; k++) {

					recv(sock, readBuffer, BUFFER, 0);
					printf("S: %s", readBuffer);
					error(readBuffer[0], "Fehler im Spielverlauf: Fehler beim ausgeben der Spielernummer, Steinnummer und Position!");
			
					/* trenne readBuffer auf , und extrahiere die werte als variablen, diese dann in piecelist array speichern, damit sie später in SHM */

				/*	help = strtok(readBuffer, ".");
					strncpy(
					
					piecelist[m][k] = ...	*/
			
				}
				}
        /* ---------------------*/

				/* anfang fuer SHM2 for PIECELIST, der Shm , größe gemäß anzahl der steine und spieler allociieren. */ 

				int shm_id2 = shmget(IPC_PRIVATE, sizeof(?), IPC_CREAT | 0666);
					
				int *shmptr = (int *) shmat(shm_id, NULL, 0);

				if (shm_id < 0)
				{
				fprintf(stderr, "Fehler bei shmget().\n");
				return EXIT_FAILURE;
				}

				if (*shmptr == -1)
				{
				fprintf(stderr, "Fehler bei shmat().\n");
				return EXIT_FAILURE;
    				}
				
				/* Ende SHM 2 	*/
	

				recv(sock, readBuffer, BUFFER, 0);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Ausgabe der Liste der	Spielernummer, Steinnummer und Position kann nicht korrekt abgeschlossen werden!");

				char *think = "THINKING\n";
				send(sock, think, strlen(think), 0);
				printf("C: %s", think);

				recv(sock, readBuffer, BUFFER, 0);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Spielverlaufphase kann nicht abgeschlossen werden!");

				/* SPIELZUG */

				/* thinker soll dann berechnen .. etc s. M3-2. dann muss noch fehlermeldung falls	spielzug ungueltig ist rein. */
				send(sock, ? , ? , 0);
				printf("C: %s", );
				recv(sock, readBuffer, BUFFER, 0);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler beim Spielzug!");
			
				break;
		
		case 'W': /* WAIT */

				strcpy(readBuffer, readComm);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Fehler bei wait!");
	
				char *waitack = "OKWAIT\n";
				send(sock, waitack, strlen(waitack), 0);

				break;

		case 'G': /* GAMEOVER */
	
				char *winnerNrandName = malloc(BUFFER);
	
				strcpy(winnerNrandName, readComm);
				printf("S: %s", winnerNrandName);
				error(winnerNrandName[0], "Fehler im Spielverlauf: Spielernummer und Spielername des Gewinners kann nicht	festgelegt werden!");
	
				recv(sock, piecesToHit, BUFFER, 0);
				printf("S: %s", piecesToHit);
				error(piecesToHit[0], "Fehler im Spielverlauf: 
				Anzahl zu schlagender Steine kann nicht	festgelegt werden!");
				
				recv(sock, playerCountpiecesCount, BUFFER, 0);
				printf("S: %s", playerCountpiecesCount);
				error(playerCountpiecesCount[0], "Fehler im Spielverlauf: Anzahl der Spieler und Steine kann nicht festgelegt werden!");
	
				for(int o = 0; o < playerCount; o++) {
				  for(int p = 0; p < piecesCount; p++) {
		
					  recv(sock, readBuffer, BUFFER, 0);
					  printf("S: %s", readBuffer);
				  	error(readBuffer[0], "Fehler im Spielverlauf: Anzahl der Spieler und Steine kann nicht festgelegt werden!");

				}}

				recv(sock, readBuffer, BUFFER, 0);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Ausgabe der Liste der Spielernummer, Steinnummer und Position kann nicht korrekt abgeschlossen 						werden!");

				recv(sock, readBuffer, BUFFER, 0);
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: 
				TCP-Verbindung konnte nicht korrekt abgebaut werden???");
				
				
				testLoop = 0;
				break;


		default: /* - Fehlermeldung */
			error(readComm[0], "Fehler im 							Spielverlauf: Fehler beim Lesen des Befehls!");

	} 

	
}

	

}
