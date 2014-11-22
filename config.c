#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct config
	{
	char hostname[100];
	uint16_t portnumber;
	char gamekindname[100];
	};

void chomp(char *str) 
	{
	size_t p=strlen(str);
	/* '\n' mit '\0' überschreiben */
	str[p-1]='\0';
	}

struct config *confparam(FILE *datei)
	{
	struct config *conf = malloc(sizeof(struct config *));
	char **text = malloc(sizeof(char*));
   	*text = NULL;
	size_t *t = malloc(0);
	int nRet;
	char *pname = malloc(sizeof(char*));
	char *pwert = malloc(sizeof(char*));
	while( (nRet=getline(text, t, datei)) > 0)//Hier wird jede Zeile der Datei eingelesen und in text gespeichert.
		{
      		chomp(*text);
		//Hier wird jede Zeile in den zwei Teilen aufgespaltet.
		pname = strtok(*text,"=");
		pwert = strtok(NULL," ");
		pname = strtok(pname," ");
		//printf("%s %s\n",pname,pwert);
		if (strcmp(pname,"GAMEKINDNAME")==0) 
			{
     			strcpy(conf->gamekindname, pwert);
			} 
		else if (strcmp(pname,"HOSTNAME")==0) 
			{
			strcpy(conf->hostname, pwert);
			}
		else if (strcmp(pname,"PORTNUMBER")==0) 
			{	
      			conf->portnumber = atoi(pwert);
              		}
		}
		
	return conf;
	}

int main(int argc, const char *argv[])
{
	
	//-------------Meilstein 2---------------------------
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
		struct config *conf=malloc(sizeof(struct config *));
		conf = confparam(confdatei);
		fclose(confdatei);
		printf("%s\n",conf->gamekindname);
		printf("%d\n",conf->portnumber);
		printf("%s\n",conf->hostname);
  	}
	else 
	{
      		printf("Konnte Datei nicht finden bzw. öffnen!\n");
      		return EXIT_FAILURE;
   	}
//------------------------------------------------------------
	
	


	return EXIT_SUCCESS;
}

