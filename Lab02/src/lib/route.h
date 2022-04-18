#include "grid.h"
#include "readfile.h"

#ifndef _ROUTE_H_
#define _ROUTE_H_

typedef struct point{
	int x;
	int y;
} Point;

void routeOneNet(Grid *grid, Net net);

#endif