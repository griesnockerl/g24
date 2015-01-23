#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

#include <string.h>


#include "gameDetails.h"
#include "performConnection.h"
#include "config.h"
#include "think.h"

struct shm *shmptr;
void my_handler(int sig){
	if((shmptr->flag==1)&&(SIGUSR1==sig)){
				
				shmptr->flag=0;
		
				char *firstmove= malloc(80);
				firstmove = think(shmptr);
printf("Firstmove should be: %s\n", firstmove);
				int n= strlen(firstmove);

				if ((write (shmptr->fd[1], firstmove , n)) != n) {
	
					perror("Fehler beim write().");
					exit(EXIT_FAILURE);
		  
				}
				//free(firstmove);printf("hallo");
	}
}



int main(int argc, const char *argv[])
{
	//GAMEID validation
	char *gameID = malloc(11*sizeof(char));

    if(argc < 2 || strlen(argv[1]) != 11)
    {
        perror("Bitte gib eine 11 stellige Game id ein!");
        return EXIT_FAILURE;
    }
	//Copy everything from argv[1] -> gameID
    	strcpy(gameID,argv[1]); 
	
	struct config *conf=malloc(sizeof(struct config));
	FILE *confdatei=malloc(10000);
	if(argc < 3)
	{
		//open client.conf
		confdatei=fopen("client.conf", "r");
	}
	else 
	{	
		//open argv[2] - optional Configfile
		if(strstr(argv[2], ".conf")!= NULL)
		{
			confdatei=fopen(argv[2], "r");
		}
		else
		{
			confdatei= NULL;
		}
	}
	
   	if(confdatei != NULL) 
	{
		conf = confparam(confdatei);
		fclose(confdatei);
  	}
	else 
	{
  		strcpy(conf->hostname,"sysprak.priv.lab.nm.ifi.lmu.de");
		conf->portnumber= 1357;
		strcpy(conf->gamekindname,"MMorris");
   	}

	//-------------SHM--------------//
	
	int shm_id = shmget(IPC_PRIVATE, sizeof(struct shm), IPC_CREAT | 0666);
	shmptr = (struct shm *) shmat(shm_id, NULL, 0);
	shmptr->flag=0; //SHM FLAG
	if (shm_id < 0)
	{
		fprintf(stderr, "Fehler bei shmget().\n");
		return EXIT_FAILURE;
	}

	if (shmptr == (struct shm *) -1)
	{
		fprintf(stderr, "Fehler bei shmat().\n");
		return EXIT_FAILURE;
    }
shmptr = (struct shm *) shmat(shm_id, NULL, 0);
	shmptr->shm_id=shm_id;
	shmptr->flag=0; //SHM FLAG
	


	//-------------FORK-------------//
	
	pid_t pid;
	int pip = pipe(shmptr->fd);
	if(pip < 0) {
		fprintf(stderr, "Fehler bei pipe().\n");
		return EXIT_FAILURE; 
	}	

	if ((pid = fork()) < 0) {
		fprintf(stderr, "Fehler bei fork().\n");
		shmdt((void *) shmptr);
		shmctl(shm_id,IPC_RMID,NULL);
		return EXIT_FAILURE; 
	} else if (pid == 0) {
	/* Connector */
		close(shmptr->fd[1]); //Schreibseite schließen	
		shmptr->pid = getpid();	
		performConnection(gameID, conf->hostname, conf->portnumber, conf->gamekindname, shmptr);

		
	} else {
	/* Thinker */
		close(shmptr->fd[0]); //Leseseite schließen
		shmptr->ppid = getpid();

		while(1){

			signal(SIGUSR1, my_handler);
			

			int status;
			pid_t return_pid = waitpid(pid, &status, WNOHANG); /* WNOHANG def'd in wait.h */
			if (return_pid == -1) {
    				perror("Fehler beim Warten auf Kindprozess!");
				shmdt((void *) shmptr);
				shmctl(shm_id,IPC_RMID,NULL);
				return EXIT_FAILURE;
			} else if (return_pid == 0) {
   				 continue;
			} else if (return_pid == pid) {

   		 		if(shmctl(shm_id, IPC_RMID, 0) == -1)
				{
					fprintf(stderr, "Fehler bei shmctl().\n");
					return EXIT_FAILURE;
				}
				shmdt((void *) shmptr);
				shmctl(shm_id,IPC_RMID,NULL);

				return EXIT_SUCCESS;
			}else {continue;}
		}

		
	}
	//-------------FORK-END---------//
	shmdt((void *) shmptr);
	shmctl(shm_id,IPC_RMID,NULL);
	return EXIT_SUCCESS;
}
