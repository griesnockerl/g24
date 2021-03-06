#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h> 
#include <stdio.h>
#include <signal.h>
#include "gameDetails.h" 
#include <sys/select.h>  
#define BUFFER	1024 
 
 void error(char errorCode, char *msg) 
 { 
     if(errorCode == '-') 
     { 
         perror(msg); 
         exit(1); 
     } 
 } 


 void performConnection(int sock, char *gameID, char *HOSTNAME, uint16_t PORTNUMBER, char *GAMEKINDNAME, struct shm *shmptr) 
 {
     struct sockaddr_in server; 
     struct hostent *host = gethostbyname(HOSTNAME); 
  
     if (sock < 0) 
     { 
         perror("Client: Socket fehler!\n"); 
         exit(1); 
     } 
  
     if(host == NULL) 
     { 
         perror("Server nicht gefunden!\n"); 
         exit(1); 
     } 
     
     memset(&server, 0, sizeof(server)); 
    memcpy( &(server.sin_addr.s_addr), host->h_addr, host->h_length); 
     server.sin_family = host->h_addrtype; 
     server.sin_port = htons(PORTNUMBER); 
  
     if(connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0) 
     { 
        perror("Verbindungsaufbau zum Game-Server fehlgeschlagen!"); 
         exit(1); 
    } 
     /*---------------PROLOG-------------- */ 
     char *readBuffer, *gameServerVersion, *gameKindName, *gameName, *playerNrAndName, *playerTotalCount; 
	//Server accept connection 
	gameServerVersion = malloc(BUFFER);

	
	int i;
        for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &gameServerVersion[i], 1, 0);
          if (gameServerVersion[i] == '\n') break;
	}
    printf("S: %s", gameServerVersion); 
      
    error(gameServerVersion[0], "Fehler im Prolog: Verbindungsaufbau fehlgeschlagen!"); 
  
     //Send client version 
     char *sendmsg = "VERSION 1.0\n"; 
     send(sock, sendmsg, strlen(sendmsg),0); 
     printf("C: %s", sendmsg); 
  
     //Server accept Client version 
     readBuffer = malloc(BUFFER);
     for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &readBuffer[i], 1, 0);
          if (readBuffer[i] == '\n') break;
	}
     printf("S: %s", readBuffer); 
  
     error(readBuffer[0], "Fehler im Prolog: Client version wird nicht akzeptiert!"); 
  	
     //Send game id 
     char id[] = "ID "; 
     char newLine[] = "\n"; 
     char *gameId = strcat(strcat(id, gameID), newLine); 
     send(sock, gameId, strlen(gameId), 0); 
     printf("C: %s", gameId); 
  
     //Server accept game id and ack with Gamekind name and game name 
     gameKindName = malloc(BUFFER);
      for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &gameKindName[i], 1, 0);
          if (gameKindName[i] == '\n') break;
	}
     printf("S: %s", gameKindName); 
     
     if((strstr(gameKindName, "NMMorris")) == NULL) {
		printf("Fehler: Client kann nur Mühle spielen! Erwarte NMMorris als Spiel!");
		exit(1);
	}
	
	 error(gameKindName[0], "Fehler im Prolog: Art des Spiels kann nicht festgelegt werden!"); 	
	
     gameName = malloc(BUFFER);
 for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &gameName[i], 1, 0);
          if (gameName[i] == '\n') break;
	}
    printf("S: %s", gameName); 
  
     error(gameName[0], "Fehler im Prolog: Spielname kann nicht festgelegt werden!"); 
  
     //Send playernumber,at first it must be empty 
     char *sendPlayerNr = "PLAYER\n"; 
     send(sock, sendPlayerNr, strlen(sendPlayerNr), 0); 
    printf("C: %s", sendPlayerNr); 
  
     //Server send Player info: number, name, totalcount 
     playerNrAndName = malloc(BUFFER);
     for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &playerNrAndName[i], 1, 0);
          if (playerNrAndName[i] == '\n') break;
	}	  
     printf("S: %s", playerNrAndName); 
  
     error(playerNrAndName[0], "Fehler im Prolog: Spielernummer und Spielername kann nicht festgelegt werden!"); 
  
     playerTotalCount = malloc(BUFFER); 
     
for ( i = 0; i < BUFFER; i++) {
	  recv(sock, &playerTotalCount[i], 1, 0);
          if (playerTotalCount[i] == '\n') break;
	}
     printf("S: %s", playerTotalCount); 
  
     error(playerTotalCount[0], "Fehler im Prolog: Anzahl der Spieler kann nicht festgelegt werden!"); 

free(readBuffer);
readBuffer = malloc(BUFFER);
for ( i = 0; i < BUFFER - 1; i++) {
	  recv(sock, &readBuffer[i], 1, 0);
          if (readBuffer[i] == '\n') 
          	break;
	}
printf("S: %s", readBuffer);
	error(readBuffer[0], "Fehler im Prolog: Spielernr, Name und Zustand, kann nicht ausgegeben werden! ");
	
	free(readBuffer);
readBuffer = malloc(BUFFER);
for ( i = 0; i < BUFFER - 1; i++) {
	  recv(sock, &readBuffer[i], 1, 0);
          if (readBuffer[i] == '\n') 
          	break;
	}
printf("S: %s", readBuffer);
	error(readBuffer[0], "Fehler im Prolog: Prologphase kann nicht korrekt abgeschlossen werden!");

//------SHM------// 
 	char *strdel = strtok(playerNrAndName, "+ YOU"); 
 	char *pnumber = malloc(sizeof(char)); 
 	char *pname = malloc(sizeof(char)); 
  
 	strcpy(pnumber, strdel); 
 	strdel = strtok(NULL, ""); 
 	strcpy(pname, strdel); 
  
 	//init vars in gameDetails.h 
	shmptr->playerName= malloc(BUFFER);
 	strcpy(shmptr->playerName, pname); 
 	shmptr->playerNumber = atoi(pnumber); 
 	shmptr->playerCount = atoi(playerTotalCount); 

/*
---------------SPIELVERLAUF-------------- */
	char *maxTimeforMove, *piecesToHit, *playersCountpiecesCount, *playerNamepieceNrAndPos;
	char *playerCountpiecesCount, *winnerNrandName;
	int testLoop = 1;
	char *readComm;
	
while(testLoop) {	

		readComm = malloc(BUFFER);
		for ( i = 0; i < BUFFER; i++) {
	  		recv(sock, &readComm[i], 1, 0);
			
		if (readComm[i] == '\n')
		{
		  break;
		}
	}

		

switch(readComm[2]) {

		case 'M': /*  MOVE */
				
		
				
				
				printf("S: %s", readComm);
				error(readComm[0], "Fehler im Spielverlauf: Maximale Zugzeit kann nicht festgelegt werden!");
				maxTimeforMove=strtok(readComm," ");
				maxTimeforMove=strtok(NULL," ");
				maxTimeforMove=strtok(NULL,"");
				shmptr->maxTimeMove = atoi(maxTimeforMove);

				printf("%d\n",shmptr->maxTimeMove);

				piecesToHit = malloc(BUFFER);
				for ( i = 0; i < BUFFER; i++) {
	 			 recv(sock, &piecesToHit[i], 1, 0);
          			if (piecesToHit[i] == '\n') break;
				}

				printf("S: %s", piecesToHit);
				error(piecesToHit[0], "Fehler im Spielverlauf: Anzahl zu schlagender Steine kann nicht festgelegt werden!");
				piecesToHit=strtok(piecesToHit," ");
				piecesToHit=strtok(NULL," ");
				piecesToHit=strtok(NULL,"");
				shmptr->remainToHit = atoi(piecesToHit);

				printf("%d\n",shmptr->remainToHit);

				playersCountpiecesCount = malloc(BUFFER);

				for ( i = 0; i < BUFFER; i++) {
				  recv(sock, &playersCountpiecesCount[i], 1, 0);
        			  if (playersCountpiecesCount[i] == '\n') break;
				}

				printf("S: %s", playersCountpiecesCount);
				error(playersCountpiecesCount[0], "Fehler im Spielverlauf: Anzahl der Spieler und Anzahl der Steine kann nicht festgelegt werden!");
				
				char *playCount = malloc(sizeof(char));
				char *piecCount = malloc(sizeof(char));
				char *strgarbage2 = malloc(BUFFER);

				strgarbage2 = strtok(playersCountpiecesCount, " ,");
				strgarbage2 = strtok(NULL, " ,");
				playCount = strtok(NULL, " ,");
				piecCount = strtok(NULL, " ,");
				
				/* Vars aus Gamedetails werden geschrieben */
				shmptr->playerCount = atoi(playCount);
				
				shmptr->piecesCount = atoi(piecCount);
				
				printf("%d, %d\n",shmptr->playerCount,shmptr->piecesCount);
				int o, p;
				char *temp1,*temp2,*position;
				temp1= malloc(BUFFER);
				temp2= malloc(BUFFER);

				position= malloc(BUFFER);
				int spielernummer,steinnummer;

				for(o = 0; o < shmptr->playerCount; o++) {
				for(p = 0; p < shmptr->piecesCount; p++) {
					free(readBuffer);
					readBuffer = malloc(BUFFER);
					for ( i = 0; i < BUFFER; i++) {
	  					recv(sock, &readBuffer[i], 1, 0);
          					if (readBuffer[i] == '\n') break;
					}
					printf("S: %s", readBuffer);
					error(readBuffer[0], "Fehler im Spielverlauf: Anzahl der Spieler und Steine kann nicht festgelegt werden!");

					temp1 = strtok(readBuffer, ".");
					temp2 = strtok(NULL, " ");
					position = strtok(NULL, "\n");
					steinnummer = atoi(temp2);
					
					temp1 = strtok(temp1, "E");
					temp1 = strtok(NULL, "E");
					temp1 = strtok(NULL, "");
					spielernummer= atoi(temp1);
					printf("%d,%d,%s\n",spielernummer,steinnummer,position);	
					

					if (spielernummer == 0){
						strcpy(shmptr->p0[steinnummer], position);
					}
					else{
						strcpy(shmptr->p1[steinnummer], position);
					}
				}}


				free(readBuffer);
				readBuffer = malloc(BUFFER);
				for ( i = 0; i < BUFFER; i++) {
				  recv(sock, &readBuffer[i], 1, 0);
        			  if (readBuffer[i] == '\n') break;
				}
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Ausgabe der Liste der Spielernummer, Steinnummer und Position kann nicht korrekt abgeschlossen werden!");

				char *think = "THINKING\n";
				send(sock, think, strlen(think), 0);
				printf("C: %s", think);

				free(readBuffer);
				readBuffer = malloc(BUFFER);	
				for ( i = 0; i < BUFFER; i++) {
				  recv(sock, &readBuffer[i], 1, 0);
        			  if (readBuffer[i] == '\n') break;
				}
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Spielverlaufphase kann nicht abgeschlossen werden!");
				shmptr->flag=1;//SET FLAG
				kill(shmptr->ppid,SIGUSR1);
				
				/* SPIELZUG
				 dann muss noch fehlermeldung falls spielzug ungueltig ist rein. */
				fd_set mySet;
				FD_ZERO(&mySet);
				FD_SET(sock, &mySet);
				FD_SET(shmptr->fd[0], &mySet);		
				int sback;
				int sock_max;
	
				if (shmptr->fd[0] > sock) {
					sock_max = shmptr->fd[0] + 1;
				} else {
					sock_max = sock + 1;
				}

				if ((sback = (select(sock_max , &mySet, NULL, NULL, NULL ))) < 0) {

					perror("Fehler bei select().");
					exit(1);

				} else if (sback == 0) {
					perror("Timeout bei select().");
					exit(1);
				}		
				
				while (1) {
					if(FD_ISSET(sock, &mySet)) {
						free(readBuffer);
						readBuffer = malloc(BUFFER);
						for ( i = 0; i < BUFFER; i++) {
				 		recv(sock, &readBuffer[i], 1, 0);
        			 		 if (readBuffer[i] == '\n') break;
						
						}
						printf("S: %s", readBuffer);
						error(readBuffer[0], "Fehler beim Warten auf Berechnung des Spielzugs."); 


					} else if (FD_ISSET(shmptr->fd[0], &mySet)) {

							free(readBuffer);
							readBuffer = malloc(BUFFER);	
							int n = read(shmptr->fd[0], readBuffer, sizeof(readBuffer));
							
							send(sock, readBuffer, strlen(readBuffer), 0);
		
							printf("C: %s", readBuffer);
							break;
					}
				}
			
				

				free(readBuffer);
				readBuffer = malloc(BUFFER);
				for ( i = 0; i < BUFFER; i++) {
				  recv(sock, &readBuffer[i], 1, 0);
        			  if (readBuffer[i] == '\n') break;
				}
				// MOVEOK
				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler beim Spielzug!");
				
				free(readBuffer);
				readBuffer = malloc(BUFFER);

				
				
				
				free(readComm);
				
				break; 
		
		case 'W': /* WAIT */

				printf("S: %s", readComm);
				error(readComm[0], "Fehler im Spielverlauf: Fehler bei wait!");
	
				
				send(sock, "OKWAIT\n", 7, 0);
				printf("C: OKWAIT\n");

				free(readComm);
				break;

		case 'G': /* GAMEOVER */
				
				playerCountpiecesCount = malloc(BUFFER);
				winnerNrandName = malloc(BUFFER);
	
				strcpy(winnerNrandName, readComm);
				printf("S: %s", winnerNrandName);
				error(winnerNrandName[0], "Fehler im Spielverlauf: Spielernummer und Spielername des Gewinners kann nicht festgelegt werden!");
				
				for ( i = 0; i < BUFFER; i++) {
	  				recv(sock, &piecesToHit[i], 1, 0);
        				if (piecesToHit[i] == '\n') break;
				}

				printf("S: %s", piecesToHit);
				error(piecesToHit[0], "Fehler im Spielverlauf: 	Anzahl zu schlagender Steine kann nicht festgelegt werden!");
				
				for ( i = 0; i < BUFFER; i++) {
					recv(sock, &playerCountpiecesCount[i], 1, 0);
          				if (playerCountpiecesCount[i] == '\n') break;
				}

				printf("S: %s", playerCountpiecesCount);
				error(playerCountpiecesCount[0], "Fehler im Spielverlauf: Anzahl der Spieler und Steine kann nicht festgelegt werden!");
				
				
				for(o = 0; o < shmptr->playerCount; o++) {
				for(p = 0; p < shmptr->piecesCount; p++) {
		
					for ( i = 0; i < BUFFER; i++) {
	  				recv(sock, &readBuffer[i], 1, 0);
          				if (readBuffer[i] == '\n') break;
					}

					printf("S: %s", readBuffer);
					error(readBuffer[0], "Fehler im Spielverlauf: Anzahl der Spieler und Steine kann nicht festgelegt werden!");

				}}

				for ( i = 0; i < BUFFER; i++) {
	  				recv(sock, &readBuffer[i], 1, 0);
        				if (readBuffer[i] == '\n') break;
				}

				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: Ausgabe der Liste der Spielernummer, Steinnummer und Position kann nicht korrekt abgeschlossen werden!");

				for ( i = 0; i < BUFFER; i++) {
	  				recv(sock, &readBuffer[i], 1, 0);
          				if (readBuffer[i] == '\n') break;
				}

				printf("S: %s", readBuffer);
				error(readBuffer[0], "Fehler im Spielverlauf: TCP-Verbindung konnte nicht korrekt abgebaut werden???");

				free(readComm);
				testLoop = 0;
				break;
				
		

		default:  /*- Fehlermeldung */
		                printf("ERROR: %s\n", readComm);
				error(readComm[0], "Fehler im Spielverlauf: Fehler beim Lesen des Befehls!");
				break;
	} 

} 

  
 	
 } 
