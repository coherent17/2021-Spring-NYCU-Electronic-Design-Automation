#include <stdio.h>
#include <stdlib.h>

#include "../src/lib/readfile.h"
#include "../src/lib/grid.h"
#include "../src/lib/route.h"

using namespace std;

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	char *outputName = *(argv + 2);

	FILE *input = fopen(inputName, "r");
	int ROW, COL, NumBlock, NumNet;
	Block *BlockArray;						// an array to store the block coordinate		
	Net *NetArray;							// an array to store nets
	Grid *grid = NULL;						// a grid pointer to the 2d array which store the state of the grids
	vector <vector <Point>> PathArray;		// a 2D array to store the path for each net

	readfile(input, &ROW, &COL, &NumBlock, &BlockArray, &NumNet, &NetArray);
	fclose(input);

	//sort the netarray due to the distance between the source and target by qsort
	sortNetArray(NetArray, NumNet);
	//printInputFile(ROW, COL, NumBlock, BlockArray, NumNet, NetArray);

	//initialize the grid with block-occupied, pin-occupied, net-occupied or empty-occupied
	//and return a pointer to that grid structure
	grid = createGrid(ROW, COL, BlockArray, NumBlock, NetArray, NumNet);

	//start to first route
	bool isValidRouting = false;
	while(!isValidRouting){

		PathArray.clear();

		for(int i = 0; i < NumNet; i++){
			isValidRouting = false;

			//route a single net and return the path if it's a validRouting
			vector <Point> Path = routeOneNet(grid, NetArray[i], &isValidRouting);

			//if the algorithm can't find the path because of the net-ordering issue,
			//rip up all of the net, and route that boken net first
			if(!isValidRouting){
				printf("Reroute for Net(%d): name(%s)\n", i, NetArray[i].NetName);
				ripUpAllNet(grid, NetArray);
				shuffleNetArray(NetArray, NumNet, i);
				break;
			}

			//if the algorithm does find the path for the net, then store the path into the patharray
			else{
				PathArray.emplace_back(Path);
				NetArray[i].gridUsage = (Path.size()-2);		  //grid usage exclude the source and the target
				updateGridState(PathArray[i], grid, NetArray[i]); //mark as Net-occupied in the grid state
				//printPath(PathArray[i]);
				//printGrid(grid);
			}
		}
	}

	//output the routing result
	FILE *output = fopen(outputName, "w");
	outputFile(output, NetArray, NumNet, PathArray);

	fclose(output);
	freeGrid(grid);
	free(BlockArray);
	free(NetArray);
	return 0;
}