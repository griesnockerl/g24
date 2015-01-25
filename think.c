#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameDetails.h"

char* think(struct shm *shmptr){
	char *msg=malloc(8*sizeof(char));
	char *msg2 = malloc(11*sizeof(char));

	char feld[3][8];
	int o, p;

	for(o = 0; o < 3; o++) {
		for(p = 0; p < 8; p++) {
			feld[o][p]='+';
		}
	}
	


		
	
	int flag1=0,flag2=0;
	int count1=0, count2=0;


	void printFeld() {
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
	}


	printf("Capture %d pieces\n",shmptr->remainToHit);

	for(o = 0; o < shmptr->playerCount; o++) {
		for(p = 0; p < shmptr->piecesCount; p++) {
			if (o == 0){
				if(strcmp(shmptr->p0[p],"A")==0){ flag1=1;}

				if (strlen(shmptr->p0[p])>1){

					switch (shmptr->p0[p][0]){
					
						case 'A': feld[0][shmptr->p0[p][1]- '0']='*'; count1 += 1; break;
						case 'B': feld[1][shmptr->p0[p][1]- '0']='*'; count1 += 1; break;
						case 'C': feld[2][shmptr->p0[p][1]- '0']='*'; count1 += 1; break;
						default:break;
						}
				}
			}
			else if (o == 1){
				if(strcmp(shmptr->p1[p],"A")==0) {flag2=1;}

				if (strlen(shmptr->p1[p])>1){

					switch (shmptr->p1[p][0]){
				
						case 'A': feld[0][shmptr->p1[p][1]- '0']='#'; count2 += 1; break;
						case 'B': feld[1][shmptr->p1[p][1]- '0']='#'; count2 += 1; break;
						case 'C': feld[2][shmptr->p1[p][1]- '0']='#'; count2 += 1; break;
						default:break;
					}		
				}
			}
		}
	}
	
	if(shmptr->playerNumber == 1) {

		if(shmptr->remainToHit == 0) {

			if(flag2==1){

				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
		
				while(feld[((int) test1)-65][test2]!='+'){
					test1=(char) (rand()%3)+65;
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
		
				printFeld();
				return msg;
		
		
			} else if ( (flag2 == 0) && (count2 > 3) ) { 

				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
				char test3 = test1;
				int test4 = test2;
				int random;
		
				while( (feld[((int) test1)-65][test2]!='#') || (feld[((int) test3)-65][test4]!='+') ){
					
					test1=(char) (rand()%3)+65;
					test2 = rand()%8;
			
					test3 = test1;
					test4 = test2;

					switch(test1){
						case 'A': //gleicht Fall 'C'
		
							switch(test2%2) {
		
								case 0:	
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									} break;
								case 1: 
									if((random = rand()%3) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 								
										test4 = test2 + 1; }
									} else if (random == 1) {
										test3 = 'B';
									} else { 
										test4 = test2 - 1;
									} break;
								default: break;
							} break;
		
						case 'B':
							switch(test2%2) {
								case 0:	
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									}
									break;
								case 1:
									if((random = rand()%4) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 
									test4 = test2 + 1; }
									}  else if (random == 1) {
									test3 = 'C';
									} else if (random ==2) { 
									test4 = test2 - 1;
									} else { 
									test3 = 'A';
								}
								break;	
								default: break;
							} break;
						
						case 'C':
							switch(test2%2) {
								case 0: 
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									}
									break;
								case 1:
									if((random = rand()%3) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 
									test4 = test2 + 1; }
									}  else if (random == 1) {
									test3 = 'B';
									} else { 
									test4 = test2 - 1;
									}
									break; 
								default: break;
							} break;
						default: break;
					}
					
	
				}
				
				msg2[0]='P';
				msg2[1]='L';
				msg2[2]='A';
				msg2[3]='Y';
				msg2[4]=' ';
				msg2[5]=test1;
				msg2[6]=(char)(((int)'0')+test2);
				msg2[7]= ':';
				msg2[8]=test3;
				msg2[9]=(char)(((int)'0')+test4);
				msg2[10]='\n';
				msg2[11]='\0';
		
				printFeld();
				return msg2;		
	
	
			} else  {
				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
				char test3=(char) (rand()%3)+65;
				int test4= rand()%8;
				while(feld[((int) test1)-65][test2]!='#'){
					test1=(char) (rand()%3)+65;
					test2=rand()%8;
				}
				while(feld[((int) test3)-65][test4]!='+'){
					test3=(char) (rand()%3)+65;
					test4=rand()%8;
				}

				msg2[0]='P';
				msg2[1]='L';
				msg2[2]='A';
				msg2[3]='Y';
				msg2[4]=' ';
				msg2[5]=test1;
				msg2[6]=(char)(((int)'0')+test2);
				msg2[7]= ':';
				msg2[8]=test3;
				msg2[9]=(char)(((int)'0')+test4);
				msg2[10]='\n';
				msg2[11]='\0';
		
				printFeld();
				return msg2;
		
			}  
	
	
        	}
		else if ((shmptr->remainToHit) == 1) {
			char test1= ' ';
			int test2= 0;
			int gay = 0;
			for(o = 0; (o < 3) && (gay == 0) ; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='*') {
						test1 = (char) o +  65;
						test2 = p;
						gay = 1;
						break;
					
					}
				}
	
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
			
			printFeld();
			return msg;
	
		} else  {
			char test1= ' ';
			int test2= 0;
			char test3 = ' ';
			int test4 = 0;
	
			for(o = 0; o < 3; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='*') {
						test1 = (char) o +  65;
						test2 = p;
					
					}
				}
			}
	
			for(o = 0; o < 3; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='*') {
						test3 = (char) o +  65;
						test4 = p;
					
					}
				}
			}
		
			msg2[0]='P';
			msg2[1]='L';
			msg2[2]='A';
			msg2[3]='Y';
			msg2[4]=' ';
			msg2[5]=test1;
			msg2[6]=(char)(((int)'0')+test2);
			msg2[7]= ':';
			msg2[8]=test3;
			msg2[9]=(char)(((int)'0')+test4);
			msg2[10]='\n';
			msg2[11]='\0';
	
			printFeld();
			return msg2;
	
		} 
	} else { /* if we have playerNumber 0 instead of 1 */

		if(shmptr->remainToHit == 0) {
			if(flag1==1){
				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
		
				while(feld[((int) test1)-65][test2]!='+'){
					test1=(char) (rand()%3)+65;
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
		
				printFeld();
				return msg;
		
		
			} else if ( (flag1 == 0) && (count1 > 3) ) { 
				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
				char test3 = test1;
				int test4 = test2;
				int random;
				
		
				while( (feld[((int) test1)-65][test2]!='*') || (feld[((int) test3)-65][test4]!='+') ){
				
					test1=(char) (rand()%3)+65;
					test2 = rand()%8;
				
					test3 = test1;
					test4 = test2;
				
	
					switch(test1){
						case 'A': //gleicht Fall 'C'
		
							switch(test2%2) {
		
								case 0:	
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									} break;
								case 1: 
									if((random = rand()%3) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 								
										test4 = test2 + 1; }
									} else if (random == 1) {
										test3 = 'B';
									} else { 
										test4 = test2 - 1;
									} break;
								default: break;
							} break;
		
						case 'B':
							switch(test2%2) {
								case 0:	
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									}
									break;
								case 1:
									if((random = rand()%4) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 
									test4 = test2 + 1; }
									}  else if (random == 1) {
									test3 = 'C';
									} else if (random ==2) { 
									test4 = test2 - 1;
									} else { 
									test3 = 'A';
								}
								break;	
								default: break;
							} break;
						
						case 'C':
							switch(test2%2) {
								case 0: 
									if ((rand() % 2) == 0) {
										test4 = test2 + 1; 
									} else { if (test2 == 0) {test4 = 7;} 
										else {test4 = test2 - 1;}
									}
									break;
								case 1:
									if((random = rand()%3) == 0) {
									if(test2 == 7) {test4 = 0;} else{ 
									test4 = test2 + 1; }
									}  else if (random == 1) {
									test3 = 'B';
									} else { 
									test4 = test2 - 1;
									}
									break; 
								default: break;
							} break;
						default: break;
					}
					
	
				}
	
				msg2[0]='P';
				msg2[1]='L';
				msg2[2]='A';
				msg2[3]='Y';
				msg2[4]=' ';
				msg2[5]=test1;
				msg2[6]=(char)(((int)'0')+test2);
				msg2[7]= ':';
				msg2[8]=test3;
				msg2[9]=(char)(((int)'0')+test4);
				msg2[10]='\n';
				msg2[11]='\0';
		
				printFeld();
				return msg2;		
	
	
			} else  {

				char test1=(char) (rand()%3)+65;
				int test2= rand()%8;
				char test3=(char) (rand()%3)+65;
				int test4= rand()%8;

				while(feld[((int) test1)-65][test2]!='*'){
					test1=(char) (rand()%3)+65;
					test2=rand()%8;
				}
				while(feld[((int) test3)-65][test4]!='+'){
					test3=(char) (rand()%3)+65;
					test4=rand()%8;
				}
				msg2[0]='P';
				msg2[1]='L';
				msg2[2]='A';
				msg2[3]='Y';
				msg2[4]=' ';
				msg2[5]=test1;
				msg2[6]=(char)(((int)'0')+test2);
				msg2[7]= ':';
				msg2[8]=test3;
				msg2[9]=(char)(((int)'0')+test4);
				msg2[10]='\n';
				msg2[11]='\0';
		
				printFeld();
				return msg2;
		
		
			}  
	
	
        	}
		else if ((shmptr->remainToHit) == 1) {

			char test1= ' ';
			int test2= 0;
			int gay = 0;

			for(o = 0; (o < 3) && (gay == 0) ; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='#') {
						test1 = (char) o +  65;
						test2 = p;
						gay = 1;
						break;
					
					}
				}
	
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
			
			printFeld();
			return msg;
	
		} else  {

			char test1= ' ';
			int test2= 0;
			char test3 = ' ';
			int test4 = 0;
	
			for(o = 0; o < 3; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='#') {
						test1 = (char) o +  65;
						test2 = p;
					
					}
				}
			}
	
			for(o = 0; o < 3; o++) {
				for(p = 0; p < 8; p++) {
					if (feld[o][p]=='#') {
						test3 = (char) o +  65;
						test4 = p;
					
					}
				}
			}
		
			msg2[0]='P';
			msg2[1]='L';
			msg2[2]='A';
			msg2[3]='Y';
			msg2[4]=' ';
			msg2[5]=test1;
			msg2[6]=(char)(((int)'0')+test2);
			msg2[7]= ':';
			msg2[8]=test3;
			msg2[9]=(char)(((int)'0')+test4);
			msg2[10]='\n';
			msg2[11]='\0';
	
			printFeld();
			return msg2;
	
		} 			

	}
			
}					
