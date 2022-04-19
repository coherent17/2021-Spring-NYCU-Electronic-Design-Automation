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
	printInputFile(ROW, COL, NumBlock, BlockArray, NumNet, NetArray);


	grid = createGrid(ROW, COL, BlockArray, NumBlock, NetArray, NumNet);
	printGrid(grid);

	//start to route

	bool isValidRouting = false;
	
	vector <Point> Path1 = routeOneNet(grid, NetArray[0], &isValidRouting);
	if(isValidRouting) printf("It's ValidRouting!\n");
	else printf("It's not a ValidRouting!\n");
	printf("grid usage: %lu\n", (Path1.size()-1));
	printPath(Path1);
	updateGridState(Path1, grid, NetArray[0]);
	printGrid(grid);


	// isValidRouting = false;

	// vector <Point> Path2 = routeOneNet(grid, NetArray[0], &isValidRouting);
	// if(isValidRouting) printf("It's ValidRouting!\n");
	// else printf("It's not a ValidRouting!\n");
	// printf("grid usage: %lu\n", (Path2.size()-1));
	// printPath(Path2);
	// updateGridState(Path2, grid, NetArray[0]);
	// printGrid(grid);

	freeGrid(grid);
	free(BlockArray);
	free(NetArray);

	fclose(input);
	return 0;
}