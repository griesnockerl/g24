#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "gameDetails.h" 

void chomp(char *str) 
{
	size_t p=strlen(str);
	/* '\n' mit '\0' überschreiben */
	str[p-1]='\0';
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
	int flag1=0;
	int flag2=0;
	int flag3=0;
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
		{	flag1 +=1;
 			strcpy(conf->gamekindname, pwert);
		}
		else if (strcmp(pname,"HOSTNAME")==0) 
		{	flag2 +=1;
			strcpy(conf->hostname, pwert);
		}
		else if (strcmp(pname,"PORTNUMBER")==0) 
		{	flag3 +=1;
  			conf->portnumber = atoi(pwert);
        	}
	}
	if(flag1 !=1){
 			strcpy(conf->gamekindname, "MMorris");
	}
		
	if(flag2 !=1){			
			strcpy(conf->hostname, "sysprak.priv.lab.nm.ifi.lmu.de");
	}
	if(flag3 !=1){	
  			conf->portnumber = 1357;
        }
	return conf;
}
