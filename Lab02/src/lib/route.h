#include <vector>

#include "grid.h"
#include "readfile.h"

#ifndef _ROUTE_H_
#define _ROUTE_H_

using namespace std;
using std::vector;

vector <Point> routeOneNet(Grid *grid, Net net, bool *isValidRouting);
void printPath(vector <Point> Path);
void shuffleNetArray(Net *NetArray, int NumNet, int critical_NetNumber);
void outputFile(FILE *output, Net *NetArray, int NumNet, vector <vector <Point>> PathArray);

#endif