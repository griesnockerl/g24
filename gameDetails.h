#ifndef gameDetails
#define gameDetails

struct shm{
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
