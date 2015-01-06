#include <stdio.h>
#include <stdlib.h>

#include "gameDetails.h"
void think(struct shm *shmptr){
	char feld[3][8];
	int o, p;

	for(o = 0; o < 3; o++) {
		for(p = 0; p < 8; p++) {
			feld[o][p]='+';
		}
	}
	
	
	printf("Capture %d pieces\n",shmptr->remainToHit);

	for(o = 0; o < shmptr->playerCount; o++) {
		for(p = 0; p < shmptr->piecesCount; p++) {
			if ((o == 0)&&(strlen(shmptr->p0[p])>1)){
				switch (shmptr->p0[p][0]){
					case 'A': feld[0][shmptr->p0[p][1]]='*';break;
					case 'B': feld[1][shmptr->p0[p][1]]='*';break;
					case 'C': feld[2][shmptr->p0[p][1]]='*';break;
					default:break;
					}
			}
			else{
				switch (shmptr->p1[p][0]){
					case 'A': feld[0][shmptr->p1[p][1]]='#';break;
					case 'B': feld[1][shmptr->p1[p][1]]='#';break;
					case 'C': feld[2][shmptr->p1[p][1]]='#';break;
					default:break;
					}		
			}
		}
	}
	printf("\t\t%c----------%c----------%c\n",feld[0][0],feld[0][1],feld[0][2]);
	printf("\t\t|          |          |\n");
	printf("\t\t|   %c------%c------%c   |\n",feld[1][0],feld[1][1],feld[1][2]);
	printf("\t\t|   |      |      |   |\n");
	printf("\t\t|   |   %c--%c--%c   |   |\n",feld[2][0],feld[2][1],feld[2][2]);
	printf("\t\t|   |   |  |  |   |   |\n");
	printf("\t\t%c---%c---%c     %c---%c---%c\n",feld[0][7],feld[1][7],feld[2][7],feld[2][3],feld[1][3],feld[0][3]);
	printf("\t\t|   |   |  |  |   |   |\n");
	printf("\t\t|   |   %c--%c--%c   |   |\n",feld[2][6],feld[2][5],feld[2][4]);
	printf("\t\t|   |      |      |   |\n");
	printf("\t\t|   %c------%c------%c   |\n",feld[1][6],feld[1][5],feld[1][4]);
	printf("\t\t|          |          |\n");
	printf("\t\t%c----------%c----------%c\n",feld[0][6],feld[0][5],feld[0][4]);


}
