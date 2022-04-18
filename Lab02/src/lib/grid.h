#include "readfile.h"

#ifndef _GRID_H_
#define _GRID_H_

#define EMPTY_OCCUPIED 0
#define BLOCK_OCCUPIED -1
#define PIN_OCCUPIED -2
#define NET_OCCUPIED -3

typedef struct _grid{
	int row;
	int col;
	int **gridState;
} Grid;

Grid *createGrid(int row, int col, Block *BlockArray, int NumBlock, Net *NetArray, int NumNet);
void printGrid(Grid *grid);
void freeGrid(Grid *grid);

#endif
