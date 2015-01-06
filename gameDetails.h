#ifndef gameDetails
#define gameDetails

struct shm{
	char *playerName;
	int playerCount;
	int playerNumber;
	int piecesCount;
	int maxTimeMove;
	int remainToHit;
	int ppid;
	int pid;
	char p0[9][3];
	char *p1[];
};


#endif
