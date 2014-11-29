#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "config.c"
#include "Connector.c"

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
		//open argv[2]
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
	
	
	//-------------FORK-------------//
	pid_t pid;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "Fehler bei fork().\n");
		return EXIT_FAILURE; 
	} else if (pid == 0) {
	/* Connector */
		setupConnection(gameID, conf->hostname, conf->portnumber, conf->gamekindname);
	} else {
	/* Thinker */
		if(waitpid(pid, NULL, 0) < 0)
		{
			perror("Fehler beim Warten auf Kindprozess!");
		 	return EXIT_FAILURE;
		}
		exit(0);
	}
	
	return EXIT_SUCCESS;
}
