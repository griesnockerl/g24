#ifndef gameDetails
#define gameDetails
#include <stdint.h>
#include <stdlib.h>
struct config
{
	char hostname[100];
	uint16_t portnumber;
	char gamekindname[100];
};



struct shm{
	int shm_id;
	int fd[2];
	int flag;
	char *playerName;
	int playerCount;
	int playerNumber;
	int piecesCount;
	int maxTimeMove;
	int remainToHit;
	int ppid;
	int pid;
	char p0[9][2];
	char p1[9][2];
	
};


#endif
