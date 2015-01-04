#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "gameDetails.h"

#include "config.c"
#include "Connector.c"

/*struct gameDetails{
   char *playerName;
   int playerTotalCount;
   int playerNumber;
   int ppid;
   int pid;
}*game;
*/

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
  		printf("Konnte Datei nicht finden bzw. öffnen!\n");
  		return EXIT_FAILURE;
   	}

	//-------------SHM--------------//
	struct shm *game;
	int shm_id = shmget(IPC_PRIVATE, sizeof(game), IPC_CREAT | 0666);
	struct shm *shmptr = (struct shm *) shmat(shm_id, NULL, 0);

	if (shm_id < 0)
	{
		fprintf(stderr, "Fehler bei shmget().\n");
		return EXIT_FAILURE;
	}

	if (shmptr == (struct shm *)-1)
	{
		fprintf(stderr, "Fehler bei shmat().\n");
		return EXIT_FAILURE;
    }
	shmptr=game;


	//-------------FORK-------------//
	
	pid_t pid;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "Fehler bei fork().\n");
		return EXIT_FAILURE; 
	} else if (pid == 0) {
	/* Connector */
		setupConnection(gameID, conf->hostname, conf->portnumber, conf->gamekindname, shmptr);
		game->pid = getpid();
	} else {
	/* Thinker */
		game->ppid = getpid();
		if(waitpid(pid, NULL, 0) < 0) {
			perror("Fehler beim Warten auf Kindprozess!");
		 	return EXIT_FAILURE;
		} else if(waitpid(pid, NULL, 0) == 0) {
			//further Code
			if(shmctl(shm_id, IPC_RMID, 0) == -1)
			{
				fprintf(stderr, "Fehler bei shmctl().\n");
				return EXIT_FAILURE;
			}
			shmdt((void *) shmptr);
			shmctl(shm_id,IPC_RMID,NULL);

			return EXIT_SUCCESS;
		}
	}
	//-------------FORK-END---------//

	return EXIT_SUCCESS;
}
