#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/readfile.h"

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	char *outputName = *(argv + 2);

	FILE *input = fopen(inputName, "r");
	int ROW;
	int COL;
	int NumBlock;
	Block *BlockArray;
	int NumNet;
	Net *NetArray;

	readfile(input, &ROW, &COL, &NumBlock, &BlockArray, &NumNet, &NetArray);
	printInputFile(ROW, COL, NumBlock, BlockArray, NumNet, NetArray);
	fclose(input);
	return 0;
}