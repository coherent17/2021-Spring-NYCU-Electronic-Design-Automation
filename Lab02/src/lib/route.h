#include <vector>

#include "grid.h"
#include "readfile.h"

#ifndef _ROUTE_H_
#define _ROUTE_H_

using namespace std;
using std::vector;

typedef struct point{
	int x;
	int y;
} Point;

vector <Point> routeOneNet(Grid *grid, Net net, bool *isValidRouting);
void printPath(vector <Point> Path);
void updateGridState(vector <Point> Path, Grid *grid, Net net);
void outputFile(FILE *output, Net *NetArray, int NumNet, vector <vector <Point>> PathArray);

#endif