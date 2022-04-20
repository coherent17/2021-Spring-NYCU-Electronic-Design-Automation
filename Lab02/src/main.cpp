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
	
	vector <vector <Point>> PathArray;

	readfile(input, &ROW, &COL, &NumBlock, &BlockArray, &NumNet, &NetArray);
	fclose(input);

	//sort the netarray due to the distance between the source and target
	sortNetArray(NetArray, NumNet);
	printInputFile(ROW, COL, NumBlock, BlockArray, NumNet, NetArray);


	grid = createGrid(ROW, COL, BlockArray, NumBlock, NetArray, NumNet);
	printGrid(grid);

	//start to route
	bool finishRouting = true;
	for(int i = 0; i < NumNet; i++){
		bool isValidRouting = false;
		vector <Point> Path;
		Path = routeOneNet(grid, NetArray[i], &isValidRouting);
		PathArray.emplace_back(Path);
		if(!isValidRouting) finishRouting = false;
		NetArray[i].gridUsage = (Path.size()-2);
		printPath(PathArray[i]);
		updateGridState(PathArray[i], grid, NetArray[i]);
		//printGrid(grid);
	}

	//output the routing result
	FILE *output = fopen(outputName, "w");

	if(finishRouting) outputFile(output, NetArray, NumNet, PathArray);

	freeGrid(grid);
	free(BlockArray);
	free(NetArray);
	return 0;
}