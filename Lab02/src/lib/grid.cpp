#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

	//init all of the grid to 0
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			newGrid->gridState[i][j] = EMPTY_OCCUPIED;
		}
	}

	//update the gridState with mark the block
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

void freeGrid(Grid *grid){
	for(int i = 0; i < grid->row; i++){
		free(grid->gridState[i]);
	}
	free(grid->gridState);
	free(grid);
}


void printGrid(Grid *grid){
	printf("Grid Size <ROW> <COL>: %d %d\n\n", grid->row, grid->col);

	printf("Current GridState: (E: empty, X: Block, N: Net)\n");
	
	printf("(0, 0) @ leftDown version:\n");
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