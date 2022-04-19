#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "../src/lib/readfile.h"
#include "../src/lib/grid.h"
#include "../src/lib/route.h"

using namespace std;
using std::queue;

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
	Grid *grid = NULL;


	readfile(input, &ROW, &COL, &NumBlock, &BlockArray, &NumNet, &NetArray);

	//sort the netarray due to the distance between the source and target
	sortNetArray(NetArray, NumNet);
	printInputFile(ROW, COL, NumBlock, BlockArray, NumNet, NetArray);


	grid = createGrid(ROW, COL, BlockArray, NumBlock, NetArray, NumNet);
	printGrid(grid);

	//start to route
	for(int i = 0; i < NumNet; i++){
		bool isValidRouting = false;
		vector <Point> Path = routeOneNet(grid, NetArray[i], &isValidRouting);
		if(isValidRouting) printf("It's ValidRouting!\n");
		else printf("It's not a ValidRouting!\n");
		printf("grid usage: %lu\n", (Path.size()-1));
		printPath(Path);
		updateGridState(Path, grid, NetArray[i]);
		printGrid(grid);
	}

	freeGrid(grid);
	free(BlockArray);
	free(NetArray);

	fclose(input);
	return 0;
}