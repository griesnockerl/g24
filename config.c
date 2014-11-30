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
	str[p-2]='\0';
}

struct config *confparam(FILE *datei)
{
	struct config *conf = malloc(sizeof(struct config));
	char **text = malloc(sizeof(char));
   	*text = NULL;
	size_t *t = malloc(0);
	int nRet;
	char *pname = malloc(sizeof(char));
	char *pwert = malloc(sizeof(char));
	//Hier wird jede Zeile der Datei eingelesen und in text gespeichert.
	while( (nRet=getline(text, t, datei)) > 0)
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
