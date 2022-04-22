#include <stdio.h>
#include <stdlib.h>
#include "readfile.h"
#include "grid.h"

Grid *createGrid(int row, int col, Block *BlockArray, int NumBlock, Net *NetArray, int NumNet){
	Grid *newGrid = (Grid *)malloc(sizeof(Grid));
	newGrid->row = row;
	newGrid->col = col;

	//malloc the memory for the whole grid
	newGrid->gridState = (int **)malloc(sizeof(int *) * row);
	for(int i = 0; i < row; i++){
		newGrid->gridState[i] = (int *)malloc(sizeof(int) * col);
	}

	//init all of the grid to EMPTY_OCCUPIED (0)
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			newGrid->gridState[i][j] = EMPTY_OCCUPIED;
		}
	}

	//update the gridState and mark the grid as BLOCK_OCCUPIED (-1) if that grid is occupied by the block
	for(int i = 0; i < NumBlock; i++){

		int LeftDownX = BlockArray[i].LeftDownX;
		int RightUpX = BlockArray[i].RightUpX;
		int LeftDownY = BlockArray[i].LeftDownY;
		int RightUpY = BlockArray[i].RightUpY;

		for(int x = LeftDownX; x <= RightUpX; x++){
			for(int y = LeftDownY; y <= RightUpY; y++){
				newGrid->gridState[y][x] = BLOCK_OCCUPIED;
			}
		}
	}

	//mark the grid as PIN_OCCUPIED (-2) if that grid is occupied by the source or the target of the net
	for(int i = 0; i < NumNet; i++){
		int sourceX = NetArray[i].sourceX;
		int sourceY = NetArray[i].sourceY;
		int targetX = NetArray[i].targetX;
		int targetY = NetArray[i].targetY;

		newGrid->gridState[sourceY][sourceX] = PIN_OCCUPIED;
		newGrid->gridState[targetY][targetX] = PIN_OCCUPIED;
	}

	return newGrid;
}

//mark the path(with source and target) of the routing net grid as NET_OCCUPIED (-3) (without source and target)
void updateGridState(vector <Point> Path, Grid *grid, Net net){
	for(auto it = Path.begin(); it != Path.end(); it++){

		//just to avoid to change the PIN_OCCUPIED in the gridState
		if(((*it).x == net.sourceX && (*it).y == net.sourceY) || ((*it).x == net.targetX && (*it).y == net.targetY)){
			continue;
		}
		grid->gridState[(*it).y][(*it).x] = NET_OCCUPIED;
	}
}

//rip up all of the net marked in the gridState
void ripUpAllNet(Grid *grid, Net *NetArray){
	//init all of the grid to 0
	for(int i = 0; i < grid->row; i++){
		for(int j = 0; j < grid->col; j++){
			if(grid->gridState[i][j] == NET_OCCUPIED){
				grid->gridState[i][j] = EMPTY_OCCUPIED;
			}
		}
	}
}

//free the memory allocated
void freeGrid(Grid *grid){
	for(int i = 0; i < grid->row; i++){
		free(grid->gridState[i]);
	}
	free(grid->gridState);
	free(grid);
}


void printGrid(Grid *grid){
	printf("Grid Size <ROW> <COL>: %d %d\n\n", grid->row, grid->col);
	printf("Current GridState: (E: empty, X: Block, P: Pin, *: Path)\n");
	for(int i = grid->row - 1; i >= 0; i--){
		for(int j = 0; j < grid->col; j++){
			if(grid->gridState[i][j] == EMPTY_OCCUPIED) printf(" E ");
			else if(grid->gridState[i][j] == BLOCK_OCCUPIED) printf(" X ");
			else if(grid->gridState[i][j] == PIN_OCCUPIED) printf(" P ");
			else if(grid->gridState[i][j] == NET_OCCUPIED) printf(" * ");
			else printf("%2d ", grid->gridState[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
}