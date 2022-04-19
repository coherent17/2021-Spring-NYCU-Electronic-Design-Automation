#include <stdio.h>
#include <stdlib.h>
#include "readfile.h"

void readfile(FILE *input, int *ROW, int *COL, int *NumBlock, Block **BlockArray, int *NumNet, Net **NetArray){
	fscanf(input, "%*s %d", &(*ROW));
	fscanf(input, "%*s %d", &(*COL));
	fgetc(input);

	fscanf(input, "%*s %d", &(*NumBlock));
	(*BlockArray) = (Block *)malloc(sizeof(Block) * (*NumBlock));
	for(int i = 0; i < *NumBlock; i++){
		fscanf(input, "%d %d %d %d", &((*BlockArray)[i].LeftDownX), &((*BlockArray)[i].RightUpX), &((*BlockArray)[i].LeftDownY), &((*BlockArray)[i].RightUpY));
	}
	fgetc(input);

	fscanf(input, "%*s %d", &(*NumNet));
	(*NetArray) = (Net *)malloc(sizeof(Net) * (*NumNet));
	for(int i = 0; i < *NumNet; i++){
		fscanf(input, "%s %d %d %d %d",(*NetArray)[i].NetName, &((*NetArray)[i].sourceX), &((*NetArray)[i].sourceY), &((*NetArray)[i].targetX), &((*NetArray)[i].targetY));
	}
}


void printInputFile(int ROW, int COL, int NumBlock, Block *BlockArray, int NumNet, Net *NetArray){
	printf("Grid size <ROW> <COL>: %d %d\n", ROW, COL);

	printf("\nNumBlock <blockCount>: %d\n", NumBlock);
	for(int i = 0; i < NumBlock; i++){
		printf("\tBlock %d: <LeftDownX> <RightUpX> <LeftDownY> <RightUpY>: %d %d %d %d\n", i + 1, BlockArray[i].LeftDownX, BlockArray[i].RightUpX, BlockArray[i].LeftDownY, BlockArray[i].RightUpY);
	}

	printf("\nNumNet <netCount>: %d\n", NumNet);
	for(int i = 0; i < NumNet; i++){
		printf("\tNet %d: <NetName> <sourceX> <sourceY> <targetX> <targetY>: %s %d %d %d %d\n", i + 1, NetArray[i].NetName, NetArray[i].sourceX, NetArray[i].sourceY, NetArray[i].targetX, NetArray[i].targetY);
	}
}


//quicksort the net by the distance from source to target
int comparator(const void *p, const void *q){
	Net n1 = *(const Net *)p;
    Net n2 = *(const Net *)q;
	int distance_n1 = abs(n1.sourceX - n1.targetX) + abs(n1.sourceY - n1.targetY);
	int distance_n2 = abs(n2.sourceX - n2.targetX) + abs(n2.sourceY - n2.targetY);
	return distance_n1 > distance_n2;
}

void sortNetArray(Net *NetArray, int NumNet){
	qsort((void*)NetArray, NumNet, sizeof(NetArray[0]), comparator);
}