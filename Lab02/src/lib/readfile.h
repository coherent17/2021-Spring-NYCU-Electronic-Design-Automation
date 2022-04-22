#ifndef _READFILE_H_
#define _READFILE_H_

#define NET_NAME_SIZE 10

typedef struct _Block{
	int LeftDownX;
	int RightUpX;
	int LeftDownY;
	int RightUpY;
} Block;

typedef struct _Net{
	char NetName[NET_NAME_SIZE];
	int sourceX;
	int sourceY;
	int targetX;
	int targetY;
	int gridUsage;		//the distance of the path for routing the source to target
} Net;

void readfile(FILE *input, int *ROW, int *COL, int *NumBlock, Block **BloclArray, int *NumNet, Net **NetArray);
void printInputFile(int ROW, int COL, int NumBlock, Block *BlockArray, int NumNet, Net *NetArray);
void sortNetArray(Net *NetArray, int NumNet);

#endif