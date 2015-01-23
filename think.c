#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameDetails.h"
char* think(struct shm *shmptr){
	char *msg=malloc(8*sizeof(char));

	char feld[3][8];
	int o, p;

	for(o = 0; o < 3; o++) {
		for(p = 0; p < 8; p++) {
			feld[o][p]='+';
		}
	}
	
	
	int flag1=0,flag2=0;

	printf("Capture %d pieces\n",shmptr->remainToHit);

	for(o = 0; o < shmptr->playerCount; o++) {
		for(p = 0; p < shmptr->piecesCount; p++) {
			if (o == 0){
				if(strcmp(shmptr->p0[p],"A")==0){ flag1=1;}

				if (strlen(shmptr->p0[p])>1){

					switch (shmptr->p0[p][0]){
					
						case 'A': feld[0][shmptr->p0[p][1]- '0']='*';break;
						case 'B': feld[1][shmptr->p0[p][1]- '0']='*';break;
						case 'C': feld[2][shmptr->p0[p][1]- '0']='*';break;
						default:break;
						}
				}
			}
			else if (o == 1){
				if(strcmp(shmptr->p1[p],"A")==0) {flag2=1;}

				if (strlen(shmptr->p1[p])>1){

					switch (shmptr->p1[p][0]){
				
						case 'A': feld[0][shmptr->p1[p][1]- '0']='#';break;
						case 'B': feld[1][shmptr->p1[p][1]- '0']='#';break;
						case 'C': feld[2][shmptr->p1[p][1]- '0']='#';break;
						default:break;
					}		
				}
			}
		}
	}
	if(flag2==1){
		char test1=(char) (rand()%2)+65;
		int test2= rand()%8;

		while(feld[((int) test1)-65][test2]!='+'){
			test1=(char) (rand()%2)+65;
			test2=rand()%8;
		}

		msg[0]='P';
		msg[1]='L';
		msg[2]='A';
		msg[3]='Y';
		msg[4]=' ';
		msg[5]=test1;
		msg[6]=(char)(((int)'0')+test2);
		msg[7]='\n';
		msg[8]='\0';
	
	}
	printf("\n%s\n\n",msg);
		

	printf("\t\t%c----------%c----------%c\n",feld[0][0],feld[0][1],feld[0][2]);
	printf("\t\t|          |          |\n");
	printf("\t\t|   %c------%c------%c   |\n",feld[1][0],feld[1][1],feld[1][2]);
	printf("\t\t|   |      |      |   |\n");
	printf("\t\t|   |   %c--%c--%c   |   |\n",feld[2][0],feld[2][1],feld[2][2]);
	printf("\t\t|   |   |     |   |   |\n");
	printf("\t\t%c---%c---%c     %c---%c---%c\n",feld[0][7],feld[1][7],feld[2][7],feld[2][3],feld[1][3],feld[0][3]);
	printf("\t\t|   |   |     |   |   |\n");
	printf("\t\t|   |   %c--%c--%c   |   |\n",feld[2][6],feld[2][5],feld[2][4]);
	printf("\t\t|   |      |      |   |\n");
	printf("\t\t|   %c------%c------%c   |\n",feld[1][6],feld[1][5],feld[1][4]);
	printf("\t\t|          |          |\n");
	printf("\t\t%c----------%c----------%c\n",feld[0][6],feld[0][5],feld[0][4]);
	printf("%d\n",flag1);
	return msg;
}


